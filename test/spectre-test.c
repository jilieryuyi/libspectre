#include <libspectre/spectre.h>

int main (int argc, char **argv)
{
	SpectreDocument *document;

	document = spectre_document_new (NULL);

	spectre_document_free (document);
	
	return 0;
}
