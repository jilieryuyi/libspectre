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

#ifndef SPECTRE_PAGE_H
#define SPECTRE_PAGE_H

#include <libspectre/spectre-macros.h>
#include <libspectre/spectre-status.h>
#include <libspectre/spectre-render-context.h>

SPECTRE_BEGIN_DECLS

typedef enum {
	SPECTRE_ORIENTATION_PORTRAIT,
	SPECTRE_ORIENTATION_LANDSCAPE,
	SPECTRE_ORIENTATION_REVERSE_LANDSCAPE,
	SPECTRE_ORIENTATION_REVERSE_PORTRAIT
} SpectreOrientation;

typedef struct SpectrePage SpectrePage;

SpectreStatus      spectre_page_status          (SpectrePage          *page);
void               spectre_page_free            (SpectrePage          *page);

unsigned int       spectre_page_get_index       (SpectrePage          *page);
SpectreOrientation spectre_page_get_orientation (SpectrePage          *page);
void               spectre_page_get_size        (SpectrePage          *page,
						 int                  *width,
						 int                  *height);
void               spectre_page_render          (SpectrePage          *page,
						 SpectreRenderContext *rc,
						 unsigned char       **page_data,
						 int                  *row_length);

SPECTRE_END_DECLS

#endif /* SPECTRE_PAGE_H */
