#include <stdio.h>
#include <stdlib.h>

#include <cairo.h>

#include <libspectre/spectre.h>

#include "../libspectre/spectre-utils.h"

int main (int argc, char **argv)
{
	SpectreDocument      *document;
	SpectreRenderContext *rc;
	int                   i;

	/* TODO: check argv */

	document = spectre_document_new ();
	spectre_document_load (document, argv[1]);

	printf ("Number of pages: %d\n",
		spectre_document_get_n_pages (document));
	printf ("Title: %s\n",
		spectre_document_get_title (document));
	printf ("Creator: %s\n",
		spectre_document_get_creator (document));

	rc = spectre_render_context_new ();

	for (i = 0; i < spectre_document_get_n_pages (document); i++) {
		SpectrePage     *page;
		cairo_surface_t *surface;
		char            *filename;
		int              width, height;
		unsigned char   *data = NULL;
		int              row_length;

		page = spectre_document_get_page (document, i);
		if (spectre_document_status (document)) {
			printf ("Error getting page %d: %s\n", i, 
				spectre_status_to_string (spectre_document_status (document)));
			continue;
		}

		spectre_page_get_size (page, &width, &height);
		printf ("Page %d size: %d x %d\n", i, width, height);

		spectre_render_context_set_page_size (rc, width, height);
		spectre_page_render (page, rc, &data, &row_length);


		surface = cairo_image_surface_create_for_data (data,
							       CAIRO_FORMAT_RGB24,
							       width, height,
							       row_length);

		filename = _spectre_strdup_printf ("%s/page-%d.png", argv[2], i);
		cairo_surface_write_to_png (surface, filename);
		free (filename);

		cairo_surface_destroy (surface);
		free (data);
		spectre_page_free (page);
	}

	spectre_render_context_free (rc);
	spectre_document_free (document);
	
	return 0;
}
