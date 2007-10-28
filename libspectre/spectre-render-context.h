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

#ifndef SPECTRE_RENDER_CONTEXT_H
#define SPECTRE_RENDER_CONTEXT_H

#include <libspectre/spectre-macros.h>

SPECTRE_BEGIN_DECLS

typedef struct SpectreRenderContext SpectreRenderContext;

SpectreRenderContext *spectre_render_context_new                    (void);
void                  spectre_render_context_free                   (SpectreRenderContext *rc);

void                  spectre_render_context_set_scale              (SpectreRenderContext *rc,
								     double                scale);
double                spectre_render_context_get_scale              (SpectreRenderContext *rc);
void                  spectre_render_context_set_rotation           (SpectreRenderContext *rc,
								     unsigned int          rotation);
unsigned int          spectre_render_context_get_rotation           (SpectreRenderContext *rc);
void                  spectre_render_context_set_resolution         (SpectreRenderContext *rc,
								     double                x_dpi,
								     double                y_dpi);
void                  spectre_render_context_get_resolution         (SpectreRenderContext *rc,
								     double               *x_dpi,
								     double               *y_dpi);
void                  spectre_render_context_set_page_size          (SpectreRenderContext *rc,
								     int                   width,
								     int                   height);
void                  spectre_render_context_get_page_size          (SpectreRenderContext *rc,
								     int                  *width,
								     int                  *height);
void                  spectre_render_context_set_use_platform_fonts (SpectreRenderContext *rc,
								     int                   use_platform_fonts);
int                   spectre_render_context_get_use_platform_fonts (SpectreRenderContext *rc);

SPECTRE_END_DECLS

#endif /* SPECTRE_PAGE_H */
