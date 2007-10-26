#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ps.h"

#include "spectre-document.h"
#include "spectre-private.h"

struct SpectreDocument
{
	struct document *doc;
	
	SpectreStatus    status;

	char            *filename;
};

SpectreDocument *
spectre_document_new (const char *filename)
{
	SpectreDocument *doc;

	doc = calloc (1, sizeof (SpectreDocument));
	if (!doc)
		return NULL;

	doc->filename = filename ? strdup (filename) : NULL;

	return doc;
}

void
spectre_document_load (SpectreDocument *document)
{
	FILE *fd;
	
	if (document->doc) {
		document->status = SPECTRE_STATUS_SUCCESS;
		return;
	}

	fd = fopen (document->filename, "r");
	if (!fd) {
		document->status = SPECTRE_STATUS_LOAD_ERROR;
		return;
	}

	document->doc = psscan (fd, document->filename, SCANSTYLE_NORMAL);
	if (!document->doc) {
		/* FIXME: OOM | INVALID_PS */
		document->status = SPECTRE_STATUS_LOAD_ERROR;
		return;
	}

	if (!fclose (fd)) {
		document->status = SPECTRE_STATUS_LOAD_ERROR;
		return;
	}

	if (document->status != SPECTRE_STATUS_SUCCESS)
		document->status = SPECTRE_STATUS_SUCCESS;
}

void
spectre_document_free (SpectreDocument *document)
{
	if (!document)
		return;

	if (document->filename) {
		free (document->filename);
		document->filename = NULL;
	}

	if (document->doc) {
		psfree (document->doc);
		document->doc = NULL;
	}
	
	free (document);
}

SpectreStatus
spectre_document_status (SpectreDocument *document)
{
	return document->status;
}

unsigned int
spectre_document_get_n_pages (SpectreDocument *document)
{
	if (!document->doc) {
		document->status = SPECTRE_STATUS_DOCUMENT_NOT_LOADED;
		return 0;
	}
	
	return document->doc->numpages;
}

const char *
spectre_document_get_title (SpectreDocument *document)
{
	if (!document->doc) {
		document->status = SPECTRE_STATUS_DOCUMENT_NOT_LOADED;
		return NULL;
	}

	return document->doc->title;
}

const char *
spectre_document_get_creator (SpectreDocument *document)
{
	if (!document->doc) {
		document->status = SPECTRE_STATUS_DOCUMENT_NOT_LOADED;
		return NULL;
	}

	return document->doc->creator;
}

SpectrePage *
spectre_document_get_page (SpectreDocument *document,
			   unsigned int     page_index)
{
	SpectrePage *page;

	if (page_index >= document->doc->numpages) {
		document->status = SPECTRE_STATUS_INVALID_PAGE;
		return NULL;
	}
	
	page = _spectre_page_new (page_index, document->doc);
	if (!page) {
		document->status = SPECTRE_STATUS_NO_MEMORY;
		return NULL;
	}

	if (document->status != SPECTRE_STATUS_SUCCESS)
		document->status = SPECTRE_STATUS_SUCCESS;
	
	return page;
}
