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

#include "spectre-page.h"
#include "spectre-private.h"

struct SpectrePage
{
	struct document *doc;
	
	SpectreStatus    status;

	unsigned int     index;
};

SpectrePage *
_spectre_page_new (unsigned int     page_index,
		   struct document *doc)
{
	SpectrePage *page;

	page = calloc (1, sizeof (SpectrePage));
	if (!page)
		return NULL;

	page->index = page_index;
	page->doc = doc;

	printf ("spectre_page_new\n");

	return page;
}

void
spectre_page_free (SpectrePage *page)
{
	if (!page)
		return;

	free (page);
}

unsigned int
spectre_page_get_index (SpectrePage *page)
{
	return page->index;
}

void
spectre_page_get_size (SpectrePage *page,
		       double      *width,
		       double      *height)
{
	int urx, ury, llx, lly;
	
	psgetpagebox (page->doc, page->index,
		      &urx, &ury, &llx, &lly);

	if (width)
		*width = urx - llx;
	if (height)
		*height = ury - lly;
}

void
spectre_page_render (SpectrePage   *page,
		     double         scale,
		     unsigned int   rotation,
		     double         x_dpi,
		     double         y_dpi,
		     unsigned char *page_data)
{
	/* TODO */
}
