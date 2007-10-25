#include <stdio.h>
#include <stdlib.h>

#include "spectre-page.h"

struct SpectrePage
{
	SpectreStatus status;

	unsigned int  index;
};

SpectrePage *
_spectre_page_new (unsigned int page_index)
{
	SpectrePage *page;

	page = calloc (1, sizeof (SpectrePage));
	if (!page)
		return NULL;

	page->index = page_index;

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
	/* TODO */
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
