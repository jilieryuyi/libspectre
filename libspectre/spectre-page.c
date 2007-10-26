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
