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
	const char *format;

	format = spectre_document_get_format (document);
	printf ("Document format: %s\n",
		format ? format : "Unknown");
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
	printf ("For: %s\n",
		spectre_document_get_for (document));
	printf ("Creation date: %s\n",
		spectre_document_get_creation_date (document));
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

static void
test_export (SpectreDocument      *document,
	     SpectreExporterFormat format,
	     const char           *output_dir)
{
	SpectreExporter *exporter;
	SpectreStatus status;
	char *filename;
	const char *format_str = format == SPECTRE_EXPORTER_FORMAT_PS ? "ps" : "pdf";
	int i;

	exporter = spectre_exporter_new (document, format);

	filename = _spectre_strdup_printf ("%s/output.%s", output_dir, format_str);
	status = spectre_exporter_begin (exporter, filename);
	free (filename);

	if (status) {
		printf ("Error exporting document as %s: %s\n", format_str,
			spectre_status_to_string (status));
		spectre_exporter_free (exporter);
		
		return;
	}

	/* Export even pages in reverse order */
	for (i = spectre_document_get_n_pages (document) - 1; i >= 0; i--) {
		if (i % 2 != 0)
			continue;
		
		status = spectre_exporter_do_page (exporter, i);
		if (status) {
			printf ("Error exporting page %d as %s: %s\n", i, format_str,
				spectre_status_to_string (status));
			break;
		}
	}
			
	spectre_exporter_end (exporter);
	spectre_exporter_free (exporter);
}

void
test_save (SpectreDocument *document,
	   const char      *output_dir)
{
	char *filename;

	filename = _spectre_strdup_printf ("%s/document-copy.ps", output_dir);
	spectre_document_save (document, filename);
	if (spectre_document_status (document)) {
		printf ("Error saving document as %s: %s\n", filename,
			spectre_status_to_string (spectre_document_status (document)));
	}
	free (filename);
}

void
test_save_to_pdf (SpectreDocument *document,
		  const char      *output_dir)
{
	char *filename;

	filename = _spectre_strdup_printf ("%s/document-copy.pdf", output_dir);
	spectre_document_save_to_pdf (document, filename);
	if (spectre_document_status (document)) {
		printf ("Error saving document as %s: %s\n", filename,
			spectre_status_to_string (spectre_document_status (document)));
	}
	free (filename);
}

int main (int argc, char **argv)
{
	SpectreDocument      *document;
	SpectreRenderContext *rc;
	int                   i;

	/* TODO: check argv */

	document = spectre_document_new ();
	spectre_document_load (document, argv[1]);
	if (spectre_document_status (document)) {
		printf ("Error loading document %s: %s\n", argv[1],
			spectre_status_to_string (spectre_document_status (document)));
		spectre_document_free (document);

		return 1;
	}

	test_export (document, SPECTRE_EXPORTER_FORMAT_PDF, argv[2]);
	test_export (document, SPECTRE_EXPORTER_FORMAT_PS, argv[2]);
	test_save (document, argv[2]);
	test_save_to_pdf (document, argv[2]);
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
		printf ("Page %d\n", i);
		printf ("\tPage label: %s\n", spectre_page_get_label (page));
		printf ("\tPage size: %d x %d\n", width, height);
		printf ("\tPage orientation: %s\n", 
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
