/* This file is part of Libspectre.
 *
 * Copyright (C) 2007 Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2007 Carlos Garcia Campos <carlosgc@gnome.org>
 *
 * Libspectre is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * Libspectre is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spectre-device.h"
#include "spectre-utils.h"
#include "spectre-private.h"

/* ghostscript stuff */
#include "ghostscript/iapi.h"
#include "ghostscript/ierrors.h"
#include "ghostscript/gdevdsp.h"

struct SpectreDevice {
	struct document *doc;
	
	int width, height;
	int row_length; /* !Size of a horizontal row (y-line) in the image buffer */
	unsigned char *gs_image; /* !Image buffer we received from Ghostscript library */
	unsigned char **user_image;
};


static int
spectre_open (void *handle, void *device)
{
	return 0;
}

static int
spectre_preclose (void *handle, void *device)
{
	return 0;
}

static int
spectre_close (void *handle, void *device)
{
	return 0;
}

static int
spectre_presize (void *handle, void *device, int width, int height,
		 int raster, unsigned int format)
{
	return 0;
}

static int
spectre_size (void *handle, void *device, int width, int height, int raster,
	      unsigned int format, unsigned char *pimage)
{
	SpectreDevice *sd;
	
	if (!handle)
		return 0;
	
	sd = (SpectreDevice *)handle;
	sd->width = width;
	sd->height = height;
	sd->row_length = raster;
	sd->gs_image = pimage;

	return 0;
}

static int
spectre_sync (void *handle, void *device )
{
	return 0;
}

static int
spectre_page (void *handle, void * device, int copies, int flush)
{
	SpectreDevice *sd;
	
	if (!handle)
		return 0;
	
	sd = (SpectreDevice *)handle;
	*sd->user_image = malloc (sd->row_length * sd->height);
	memcpy (*sd->user_image, sd->gs_image, sd->row_length * sd->height);
	
	return 0;
}

static display_callback spectre_device = {
	sizeof (display_callback),
	DISPLAY_VERSION_MAJOR,
	DISPLAY_VERSION_MINOR,
	&spectre_open,
	&spectre_preclose,
	&spectre_close,
	&spectre_presize,
	&spectre_size,
	&spectre_sync,
	&spectre_page
};

static void
handle_error_code (int code)
{
	if (code >= 0)
		return;
	
	if (code <= -100) {
		switch (code) {
			case e_Fatal:
				printf ("fatal internal error %d", code);
				break;

			case e_ExecStackUnderflow:
				printf ("stack overflow %d", code);
				break;

			/* no error or not important */
			default:
				return;
		}
	} else {
		const char *errors[] = { "", ERROR_NAMES };
		int x = (-1) * code;

		if (x < sizeof (errors) / sizeof (const char*)) {
			printf ("%s %d\n", errors[x], code);
		}
	}
}

#define BUFFER_SIZE 32768

static void
spectre_device_process (void       *ghostscript_instance,
			const char *filename,
			long        begin,
			long        end)
{
	FILE *fd;
	int error;
	static char buf[BUFFER_SIZE];
	unsigned int read;
	int wrote;
	size_t left = end - begin;
	
	fd = fopen (filename, "r");
	/* FIXME: errors */
	fseek (fd, begin, SEEK_SET);

	gsapi_run_string_begin (ghostscript_instance, 0, &error);
	handle_error_code (error);

	while (left > 0) {
		size_t to_read = BUFFER_SIZE;
		
		if (left < to_read)
			to_read = left;
		
		read = fread (buf, sizeof (char), to_read, fd);
		wrote = gsapi_run_string_continue (ghostscript_instance,
						   buf, read, 0, &error);
 		handle_error_code (error);
		left -= read;
	}
	
	fclose (fd);
	/* FIXME: errors */
	gsapi_run_string_end (ghostscript_instance, 0, &error);
	handle_error_code (error);
}

SpectreDevice *
spectre_device_new (struct document *doc)
{
	SpectreDevice *device;

	device = calloc (1, sizeof (SpectreDevice));
	if (!device)
		return NULL;

	device->doc = psdocreference (doc); 
	
	return device;
}

void
spectre_device_render (SpectreDevice        *device,
		       unsigned int          page,
		       SpectreRenderContext *rc,
		       unsigned char       **page_data,
		       int                  *row_length)
{
	void *ghostscript_instance;
	char **args;
	char *set;
	int n_args = 9;
	int arg = 0;
	int error;
	int has_size = (rc->width != -1 && rc->height != -1);
	
	device->user_image = page_data;
	
	error = gsapi_new_instance (&ghostscript_instance, device);
	handle_error_code (error);
	
	error = gsapi_set_display_callback (ghostscript_instance,
					    &spectre_device);
	handle_error_code (error);

	if (has_size)
		n_args++;
	if (rc->use_platform_fonts == FALSE)
		n_args++;
	
	args = calloc (sizeof (char *), n_args);
	args[arg++] = "-dMaxBitmap=10000000";
	args[arg++] = "-dDELAYSAFER";
	args[arg++] = "-dNOPAUSE";
	args[arg++] = "-dNOPAGEPROMPT";
	args[arg++] = _spectre_strdup_printf("-dTextAlphaBits=%d", rc->text_alpha_bits);
	args[arg++] = _spectre_strdup_printf("-dGraphicsAlphaBits=%d", rc->graphic_alpha_bits);
	if (has_size)
		args[arg++] = _spectre_strdup_printf ("-g%dx%d", rc->width, rc->height);
	args[arg++] = _spectre_strdup_printf ("-r%fx%f",
					      rc->scale * rc->x_dpi,
					      rc->scale * rc->y_dpi);
	args[arg++] = _spectre_strdup_printf ("-dDisplayFormat=%d",
					      DISPLAY_COLORS_RGB |
					      DISPLAY_UNUSED_LAST |
					      DISPLAY_DEPTH_8 |
					      DISPLAY_LITTLEENDIAN |
					      DISPLAY_TOPFIRST);
	args[arg++] = _spectre_strdup_printf ("-sDisplayHandle=16#%llx",
					      (unsigned long long int)device);
	if (rc->use_platform_fonts == FALSE)
		args[arg++] = "-dNOPLATFONTS";

	error = gsapi_init_with_args (ghostscript_instance, n_args, args);
	handle_error_code (error);
	
	set = _spectre_strdup_printf ("<< /Orientation %1 >> setpagedevice .locksafe",
				      rc->rotation);
	gsapi_run_string_with_length(ghostscript_instance, set, strlen (set), 0, &error);
	handle_error_code (error);
	
	spectre_device_process (ghostscript_instance,
				device->doc->filename,
				device->doc->beginprolog,
				device->doc->endprolog);
	spectre_device_process (ghostscript_instance,
				device->doc->filename,
				device->doc->beginsetup,
				device->doc->endsetup);
	spectre_device_process (ghostscript_instance,
				device->doc->filename,
				device->doc->pages[page].begin,
				device->doc->pages[page].end);
	
	*row_length = device->row_length;
	
	error = gsapi_exit (ghostscript_instance);
	handle_error_code (error);
	gsapi_delete_instance (ghostscript_instance);
}

void
spectre_device_free (SpectreDevice *device)
{
	if (!device)
		return;

	if (device->doc) {
		psdocdestroy (device->doc);
		device->doc = NULL;
	}

	free (device);
}
