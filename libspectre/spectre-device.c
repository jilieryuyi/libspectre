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

#define BUFFER_SIZE 32768

typedef enum {
	CLEANUP_DELETE_INSTANCE = 1 << 0,
	CLEANUP_EXIT            = 1 << 1
} CleanupFlag;

struct SpectreDevice {
	struct document *doc;
	
	int width, height;
	int row_length; /*! Size of a horizontal row (y-line) in the image buffer */
	unsigned char *gs_image; /*! Image buffer we received from Ghostscript library */
	unsigned char *user_image;
	int page_called;
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
	SpectreDevice *sd;

	if (!handle)
		return 0;

	sd = (SpectreDevice *)handle;
	sd->width = width;
	sd->height = height;
	sd->row_length = raster;
	sd->gs_image = NULL;
	sd->user_image = malloc (sd->row_length * sd->height);
	
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
	sd->gs_image = pimage;

	return 0;
}

static int
spectre_sync (void *handle, void *device)
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
	sd->page_called = TRUE;
	memcpy (sd->user_image, sd->gs_image, sd->row_length * sd->height);
	
	return 0;
}

static int
spectre_update (void *handle, void *device, int x, int y, int w, int h)
{
	SpectreDevice *sd;
	int i;

	if (!handle)
		return 0;

	sd = (SpectreDevice *)handle;
	if (!sd->gs_image || sd->page_called || !sd->user_image)
		return 0;

	for (i = y; i < y + h; ++i) {
		memcpy (sd->user_image + sd->row_length * i + x * 4,
			sd->gs_image + sd->row_length * i + x * 4, w * 4);
	}
	
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
	&spectre_page,
	&spectre_update
};

static int
critic_error_code (int code)
{
	if (code >= 0)
		return FALSE;
	
	if (code <= -100) {
		switch (code) {
			case e_Fatal:
				fprintf (stderr, "fatal internal error %d", code);
				return TRUE;
				break;

			case e_ExecStackUnderflow:
				fprintf (stderr, "stack overflow %d", code);
				return TRUE;
				break;

			/* no error or not important */
			default:
				return FALSE;
		}
	} else {
		const char *errors[] = { "", ERROR_NAMES };
		int x = (-1) * code;

		if (x < sizeof (errors) / sizeof (const char*)) {
			fprintf (stderr, "%s %d\n", errors[x], code);
		}
		return TRUE;
	}
}

static SpectreStatus
spectre_device_process (void       *ghostscript_instance,
			const char *filename,
			int         x,
			int         y,
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
	if (!fd) {
		return SPECTRE_STATUS_RENDER_ERROR;
	}
	
	fseek (fd, begin, SEEK_SET);

	gsapi_run_string_begin (ghostscript_instance, 0, &error);
	if (critic_error_code (error)) {
		fclose (fd);
		return SPECTRE_STATUS_RENDER_ERROR;
	}

	if (x != 0 && y != 0) {
		char *set;

		set = _spectre_strdup_printf ("%d %d translate\n", -x, -y);
		gsapi_run_string_continue (ghostscript_instance, set, strlen (set),
					   0, &error);
		free (set);
		if (critic_error_code (error)) {
			fclose (fd);
			return SPECTRE_STATUS_RENDER_ERROR;
		}
	}

	while (left > 0 && !critic_error_code (error)) {
		size_t to_read = BUFFER_SIZE;
		
		if (left < to_read)
			to_read = left;
		
		read = fread (buf, sizeof (char), to_read, fd);
		wrote = gsapi_run_string_continue (ghostscript_instance,
						   buf, read, 0, &error);
		left -= read;
	}
	
	fclose (fd);
	if (critic_error_code (error))
		return SPECTRE_STATUS_RENDER_ERROR;
	
	gsapi_run_string_end (ghostscript_instance, 0, &error);
	if (critic_error_code (error))
		return SPECTRE_STATUS_RENDER_ERROR;

	return SPECTRE_STATUS_SUCCESS;
}

static void
spectre_device_cleanup (void *ghostscript_instance, CleanupFlag flag)
{
	if (ghostscript_instance == NULL)
		return;

	if (flag & CLEANUP_EXIT)
		gsapi_exit (ghostscript_instance);

	if (flag & CLEANUP_DELETE_INSTANCE)
		gsapi_delete_instance (ghostscript_instance);
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

SpectreStatus
spectre_device_render (SpectreDevice        *device,
		       unsigned int          page,
		       SpectreRenderContext *rc,
		       unsigned char       **page_data,
		       int                  *row_length)
{
	void *ghostscript_instance = NULL;
	char **args;
	int n_args = 10;
	int arg = 0;
	int error;
	char *text_alpha, *graph_alpha;
	char *size = NULL;
	char *resolution, *set;
	char *dsp_format, *dsp_handle;
	int urx, ury, llx, lly;
	int page_width, page_height;
	double x_scale, y_scale;
	int width, height;
	SpectreStatus status;
	
	error = gsapi_new_instance (&ghostscript_instance, device);
	if (critic_error_code (error)) {
		spectre_device_cleanup (ghostscript_instance, CLEANUP_DELETE_INSTANCE);
		return SPECTRE_STATUS_RENDER_ERROR;
	}
	
	error = gsapi_set_display_callback (ghostscript_instance,
					    &spectre_device);
	if (critic_error_code (error)) {
		spectre_device_cleanup (ghostscript_instance, CLEANUP_DELETE_INSTANCE);
		return SPECTRE_STATUS_RENDER_ERROR;
	}

	psgetpagebox (device->doc, page,
		      &urx, &ury, &llx, &lly);

	page_width = (urx - llx);
	page_height = (ury - lly);

	if (rc->width == -1 || rc->height == -1) {
		width = page_width * rc->scale;
		height = page_height * rc->scale;
	} else {
		width = rc->width;
		height = rc->height;
	}

	if (rc->scale == 1.0 && (width != page_width || height != page_height)) {
		x_scale = width / (double)page_width;
		y_scale = height / (double)page_height;
	} else {
		x_scale = y_scale = rc->scale;
	}

	if (rc->use_platform_fonts == FALSE)
		n_args++;
	
	args = calloc (sizeof (char *), n_args);
	args[arg++] = "-dMaxBitmap=10000000";
	args[arg++] = "-dDELAYSAFER";
	args[arg++] = "-dNOPAUSE";
	args[arg++] = "-dNOPAGEPROMPT";
	args[arg++] = text_alpha =_spectre_strdup_printf("-dTextAlphaBits=%d",
							 rc->text_alpha_bits);
	args[arg++] = graph_alpha = _spectre_strdup_printf("-dGraphicsAlphaBits=%d",
							   rc->graphic_alpha_bits);
	args[arg++] = size =_spectre_strdup_printf ("-g%dx%d", width, height);
	args[arg++] = resolution = _spectre_strdup_printf ("-r%fx%f",
							   x_scale * rc->x_dpi,
							   y_scale * rc->y_dpi);
	args[arg++] = dsp_format = _spectre_strdup_printf ("-dDisplayFormat=%d",
							   DISPLAY_COLORS_RGB |
							   DISPLAY_UNUSED_LAST |
							   DISPLAY_DEPTH_8 |
							   DISPLAY_LITTLEENDIAN |
							   DISPLAY_TOPFIRST);
	args[arg++] = dsp_handle = _spectre_strdup_printf ("-sDisplayHandle=16#%llx",
							   (unsigned long long int)device);
	if (rc->use_platform_fonts == FALSE)
		args[arg++] = "-dNOPLATFONTS";

	error = gsapi_init_with_args (ghostscript_instance, n_args, args);
	free (text_alpha);
	free (graph_alpha);
	free (size);
	free (resolution);
	free (dsp_format);
	free (dsp_handle);
	free (args);
	if (critic_error_code (error)) {
		spectre_device_cleanup (ghostscript_instance,
					CLEANUP_DELETE_INSTANCE | CLEANUP_EXIT);
		return SPECTRE_STATUS_RENDER_ERROR;
	}

	set = _spectre_strdup_printf ("<< /Orientation %d >> setpagedevice .locksafe",
				      rc->orientation);
	gsapi_run_string_with_length (ghostscript_instance, set, strlen (set), 0, &error);
	free (set);
	if (critic_error_code (error)) {
		spectre_device_cleanup (ghostscript_instance,
					CLEANUP_DELETE_INSTANCE | CLEANUP_EXIT);
		return SPECTRE_STATUS_RENDER_ERROR;
	}

	status = spectre_device_process (ghostscript_instance,
					 device->doc->filename,
					 llx, lly,
					 device->doc->beginprolog,
					 device->doc->endprolog);
	if (status != SPECTRE_STATUS_SUCCESS) {
		spectre_device_cleanup (ghostscript_instance,
					CLEANUP_DELETE_INSTANCE | CLEANUP_EXIT);
		return status;
	}
	
	status = spectre_device_process (ghostscript_instance,
					 device->doc->filename,
					 0, 0,
					 device->doc->beginsetup,
					 device->doc->endsetup);
	if (status != SPECTRE_STATUS_SUCCESS) {
		spectre_device_cleanup (ghostscript_instance,
					CLEANUP_DELETE_INSTANCE | CLEANUP_EXIT);
		return status;
	}

	if (device->doc->numpages > 0) {
		status = spectre_device_process (ghostscript_instance,
						 device->doc->filename,
						 0, 0,
						 device->doc->pages[page].begin,
						 device->doc->pages[page].end);
		if (status != SPECTRE_STATUS_SUCCESS) {
			spectre_device_cleanup (ghostscript_instance,
						CLEANUP_DELETE_INSTANCE | CLEANUP_EXIT);
			return status;
		}
	}
	
	status = spectre_device_process (ghostscript_instance,
					 device->doc->filename,
					 0, 0,
					 device->doc->begintrailer,
					 device->doc->endtrailer);
	if (status != SPECTRE_STATUS_SUCCESS) {
		spectre_device_cleanup (ghostscript_instance,
					CLEANUP_DELETE_INSTANCE | CLEANUP_EXIT);
		return status;
	}

	*page_data = device->user_image;
	*row_length = device->row_length;

	error = gsapi_exit (ghostscript_instance);
	gsapi_delete_instance (ghostscript_instance);
	
	if (critic_error_code (error))
		return SPECTRE_STATUS_RENDER_ERROR;

	return SPECTRE_STATUS_SUCCESS;
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
