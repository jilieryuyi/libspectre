#include <stdio.h>

#include <libspectre/spectre.h>

int main (int argc, char **argv)
{
	SpectreDocument *document;

	document = spectre_document_new (argv[1]);
	spectre_document_load (document);

	printf ("Number of pages: %d\n",
		spectre_document_get_n_pages (document));
	printf ("Title: %s\n",
		spectre_document_get_title (document));
	printf ("Creator: %s\n",
		spectre_document_get_creator (document));

	spectre_document_free (document);
	
	return 0;
}
