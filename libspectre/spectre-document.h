#ifndef SPECTRE_DOCUMENT_H
#define SPECTRE_DOCUMENT_H

#include <libspectre/spectre-macros.h>
#include <libspectre/spectre-status.h>
#include <libspectre/spectre-page.h>

SPECTRE_BEGIN_DECLS

typedef struct SpectreDocument SpectreDocument;

SpectreDocument *spectre_document_new         (const char      *file);
SpectreStatus    spectre_document_status      (SpectreDocument *document);
void             spectre_document_free        (SpectreDocument *document);

unsigned int     spectre_document_get_n_pages (SpectreDocument *document);
SpectrePage     *spectre_document_get_page    (SpectreDocument *document,
					       unsigned int     page_index);

SPECTRE_END_DECLS

#endif /* SPECTRE_DOCUMENT_H */
