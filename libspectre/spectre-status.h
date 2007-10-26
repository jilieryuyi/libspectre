#ifndef SPECTRE_STATUS_H
#define SPECTRE_STATUS_H

#include <libspectre/spectre-macros.h>

SPECTRE_BEGIN_DECLS

typedef enum _SpectreStatus {
	SPECTRE_STATUS_SUCCESS = 0,
	SPECTRE_STATUS_NO_MEMORY,
	SPECTRE_STATUS_LOAD_ERROR,
	SPECTRE_STATUS_DOCUMENT_NOT_LOADED,
	SPECTRE_STATUS_INVALID_PAGE
} SpectreStatus;

const char *spectre_status_to_string (SpectreStatus status);

SPECTRE_END_DECLS

#endif /* SPECTRE_STATUS_H */
