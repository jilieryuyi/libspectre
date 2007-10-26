#include <stdio.h>

#include <libspectre/spectre.h>

int main (int argc, char **argv)
{
	SpectreDocument *document;
	int              i;

	document = spectre_document_new (argv[1]);
	spectre_document_load (document);

	printf ("Number of pages: %d\n",
		spectre_document_get_n_pages (document));
	printf ("Title: %s\n",
		spectre_document_get_title (document));
	printf ("Creator: %s\n",
		spectre_document_get_creator (document));

	for (i = 0; i < spectre_document_get_n_pages (document); i++) {
		SpectrePage *page;
		double       width, height;

		page = spectre_document_get_page (document, i);
		if (spectre_document_status (document)) {
			printf ("Error getting page %d: %s\n", i, 
				spectre_status_to_string (spectre_document_status (document)));
			continue;
		}

		spectre_page_get_size (page, &width, &height);
		printf ("Page %d size: %.2f x %.2f\n", i, width, height);
		
		spectre_page_free (page);
	}

	spectre_document_free (document);
	
	return 0;
}
