#include <stdio.h>
#include <stdlib.h>

#include "spectre-document.h"
#include "spectre-private.h"

struct SpectreDocument
{
	SpectreStatus status;

	unsigned int  n_pages;
};

SpectreDocument *
spectre_document_new (const char *file)
{
	SpectreDocument *doc = NULL;

	doc = calloc (1, sizeof (SpectreDocument));
		
	printf ("spectre_document_new\n");

	return doc;
}

void
spectre_document_free (SpectreDocument *document)
{
	if (!document)
		return;

	printf ("spectre_document_free\n");
	
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
	if (document->n_pages == 0) {
		/* TODO */
	}

	return document->n_pages;
}

SpectrePage *
spectre_document_get_page (SpectreDocument *document,
			   unsigned int     page_index)
{
	SpectrePage *page;

	if (page_index >= document->n_pages) {
		document->status = SPECTRE_STATUS_INVALID_PAGE;
		return NULL;
	}
	
	page = _spectre_page_new (page_index);
	if (!page) {
		document->status = SPECTRE_STATUS_NO_MEMORY;
		return NULL;
	}

	if (document->status != SPECTRE_STATUS_SUCCESS)
		document->status = SPECTRE_STATUS_SUCCESS;
	
	return page;
}
