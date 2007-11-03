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

#ifndef SPECTRE_DOCUMENT_H
#define SPECTRE_DOCUMENT_H

#include <libspectre/spectre-macros.h>
#include <libspectre/spectre-status.h>
#include <libspectre/spectre-page.h>

SPECTRE_BEGIN_DECLS

typedef struct SpectreDocument SpectreDocument;

SpectreDocument   *spectre_document_new             (void);
void               spectre_document_load            (SpectreDocument *document,
						     const char      *filename);
SpectreStatus      spectre_document_status          (SpectreDocument *document);
void               spectre_document_free            (SpectreDocument *document);

unsigned int       spectre_document_get_n_pages     (SpectreDocument *document);
SpectreOrientation spectre_document_get_orientation (SpectreDocument *document);
const char        *spectre_document_get_title       (SpectreDocument *document);
const char        *spectre_document_get_creator     (SpectreDocument *document);
int                spectre_document_is_eps          (SpectreDocument *document);
SpectrePage       *spectre_document_get_page        (SpectreDocument *document,
						     unsigned int     page_index);

SPECTRE_END_DECLS

#endif /* SPECTRE_DOCUMENT_H */
