#include "spectre-status.h"

const char *
spectre_status_to_string (SpectreStatus status)
{
	switch (status) {
	case SPECTRE_STATUS_SUCCESS:
		return "success";
	case SPECTRE_STATUS_NO_MEMORY:
		return "out of memory";
	case SPECTRE_STATUS_LOAD_ERROR:
		return "error loading document";
	case SPECTRE_STATUS_DOCUMENT_NOT_LOADED:
		return "document is not loaded";
	case SPECTRE_STATUS_INVALID_PAGE:
		return "page is invalid";
	}

	return "unknown error status";
}
