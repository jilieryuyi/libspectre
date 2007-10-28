/* This file is part of Libspectre.
 * 
 * Copyright (C) 2007 Albert Astals Cid <aacid@kde.org>
 * Copyright (C) 2007 Carlos Garcia Campos <carlosgc@gnome.org>
 *
 * Libspectre is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2; or (at your option)
 * any later version.
 *
 * Libspectre is distributed in the hope that it will be useful;
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not; write to the Free Software
 * Foundation; Inc.; 59 Temple Place - Suite 330; Boston; MA 02111-1307; USA.
 */

#include <stdlib.h>

#include "spectre-render-context.h"

struct SpectreRenderContext {
	double       scale;
	unsigned int rotation;
	double       x_dpi;
	double       y_dpi;
	int          width;
	int          height;
	int          text_alpha_bits;
	int          graphic_alpha_bits;
	int          use_platform_fonts;
};

SpectreRenderContext *
spectre_render_context_new (void)
{
	SpectreRenderContext *rc;

	rc = malloc (sizeof (SpectreRenderContext));

	/* TODO: document default values */
	rc->scale = 1.0;
	rc->rotation = 0;
	rc->x_dpi = 72.0;
	rc->y_dpi = 72.0;
	rc->width = -1;
	rc->height = -1;
	rc->text_alpha_bits = 0;
	rc->graphic_alpha_bits = 0;
	rc->use_platform_fonts = 0;
	
	return rc;
}

void
spectre_render_context_free (SpectreRenderContext *rc)
{
	if (!rc)
		return;

	free (rc);
}

void
spectre_render_context_set_scale (SpectreRenderContext *rc,
				  double                scale)
{
	if (!rc)
		return;

	rc->scale = scale;
}

double
spectre_render_context_get_scale (SpectreRenderContext *rc)
{
	if (!rc)
		return 1.0;

	return rc->scale;
}

void
spectre_render_context_set_rotation (SpectreRenderContext *rc,
				     unsigned int          rotation)
{
	if (!rc)
		return;

	rc->rotation = rotation;
}
	
unsigned int
spectre_render_context_get_rotation (SpectreRenderContext *rc)
{
	if (!rc)
		return 0;

	return rc->rotation;
}

void
spectre_render_context_set_resolution (SpectreRenderContext *rc,
				       double                x_dpi,
				       double                y_dpi)
{
	if (!rc)
		return;

	rc->x_dpi = x_dpi;
	rc->y_dpi = y_dpi;
}

void
spectre_render_context_get_resolution (SpectreRenderContext *rc,
				       double               *x_dpi,
				       double               *y_dpi)
{
	if (!rc)
		return;

	if (x_dpi)
		*x_dpi = rc->x_dpi;
	if (y_dpi)
		*y_dpi = rc->y_dpi;
}

void
spectre_render_context_set_page_size (SpectreRenderContext *rc,
				      int                   width,
				      int                   height)
{
	if (!rc)
		return;

	rc->width = width;
	rc->height = height;
}

void
spectre_render_context_get_page_size (SpectreRenderContext *rc,
				      int                  *width,
				      int                  *height)
{
	if (!rc)
		return;

	if (width)
		*width = rc->width;
	if (height)
		*height = rc->height;
}

void
spectre_render_context_set_use_platform_fonts (SpectreRenderContext *rc,
					       int                   use_platform_fonts)
{
	if (!rc)
		return;

	rc->use_platform_fonts = use_platform_fonts;
}

int
spectre_render_context_get_use_platform_fonts (SpectreRenderContext *rc)
{
	if (!rc)
		return 0;

	return rc->use_platform_fonts;
}

