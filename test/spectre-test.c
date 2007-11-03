#include <stdio.h>
#include <stdlib.h>

#include <cairo.h>

#include <libspectre/spectre.h>

#include "../libspectre/spectre-utils.h"

static const char *
orientation_to_string (SpectreOrientation orientation)
{
	switch (orientation) {
	case SPECTRE_ORIENTATION_PORTRAIT:
		return "Portrait";
	case SPECTRE_ORIENTATION_LANDSCAPE:
		return "Landscape";
	case SPECTRE_ORIENTATION_REVERSE_LANDSCAPE:
		return "Reverse Landscape";
	case SPECTRE_ORIENTATION_REVERSE_PORTRAIT:
		return "Reverse Portrait";
	}

	return "Unknown Orientation";
}

static void
test_metadata (SpectreDocument *document)
{
	printf ("Postscript language level: %d\n",
		spectre_document_get_language_level (document));
	printf ("Encapsulated PostScript: %s\n",
		spectre_document_is_eps (document) ? "Yes" : "No");
	printf ("Number of pages: %d\n",
		spectre_document_get_n_pages (document));
	printf ("Title: %s\n",
		spectre_document_get_title (document));
	printf ("Creator: %s\n",
		spectre_document_get_creator (document));
	printf ("Document Orientation: %s\n",
		orientation_to_string (spectre_document_get_orientation (document)));
}

static void
test_rotation (SpectreDocument *document,
	       const char      *output)
{
	SpectrePage          *page;
	SpectreRenderContext *rc;
	int                   i;
	int                   width_points;
	int                   height_points;

	page = spectre_document_get_page (document, 0);

	spectre_page_get_size (page, &width_points, &height_points);

	rc = spectre_render_context_new ();
	
	for (i = 0; i < 4; i++) {
		cairo_surface_t *surface;
		char            *filename;
		int              rotation;
		int              width, height;
		unsigned char   *data = NULL;
		int              row_length;

		rotation = 90 * i;

		if (rotation == 90 || rotation == 270) {
			width = height_points;
			height = width_points;
		} else {
			width = width_points;
			height = height_points;
		}

		spectre_render_context_set_rotation (rc, rotation);
		spectre_page_render (page, rc, &data, &row_length);
		if (spectre_page_status (page)) {
			printf ("Error rendering page %d at rotation %d: %s\n", i, rotation, 
				spectre_status_to_string (spectre_page_status (page)));
			free (data);
			continue;
		}

		surface = cairo_image_surface_create_for_data (data,
							       CAIRO_FORMAT_RGB24,
							       width, height,
							       row_length);

		filename = _spectre_strdup_printf ("%s/page-rotated-%d.png", output, rotation);
		cairo_surface_write_to_png (surface, filename);
		free (filename);

		cairo_surface_destroy (surface);
		free (data);
	}

	spectre_render_context_free (rc);
	spectre_page_free (page);
}

int main (int argc, char **argv)
{
	SpectreDocument      *document;
	SpectreRenderContext *rc;
	int                   i;

	/* TODO: check argv */

	document = spectre_document_new ();
	spectre_document_load (document, argv[1]);

	test_metadata (document);

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
		printf ("Page %d orientation: %s\n", i,
			orientation_to_string (spectre_page_get_orientation (page)));

		spectre_render_context_set_page_size (rc, width, height);
		spectre_page_render (page, rc, &data, &row_length);
		if (spectre_page_status (page)) {
			printf ("Error rendering page %d: %s\n", i,
				spectre_status_to_string (spectre_page_status (page)));
			free (data);
			spectre_page_free (page);
			continue;
		}

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

	test_rotation (document, argv[2]);
	
	spectre_document_free (document);
	
	return 0;
}
