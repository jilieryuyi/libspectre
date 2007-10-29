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

#ifndef SPECTRE_STATUS_H
#define SPECTRE_STATUS_H

#include <libspectre/spectre-macros.h>

SPECTRE_BEGIN_DECLS

typedef enum _SpectreStatus {
	SPECTRE_STATUS_SUCCESS = 0,
	SPECTRE_STATUS_NO_MEMORY,
	SPECTRE_STATUS_LOAD_ERROR,
	SPECTRE_STATUS_DOCUMENT_NOT_LOADED,
	SPECTRE_STATUS_INVALID_PAGE,
	SPECTRE_STATUS_RENDER_ERROR
} SpectreStatus;

const char *spectre_status_to_string (SpectreStatus status);

SPECTRE_END_DECLS

#endif /* SPECTRE_STATUS_H */
