#ifndef SPECTRE_PAGE_H
#define SPECTRE_PAGE_H

#include <libspectre/spectre-macros.h>
#include <libspectre/spectre-status.h>

SPECTRE_BEGIN_DECLS

typedef struct SpectrePage SpectrePage;

SpectreStatus spectre_page_status    (SpectrePage   *page);
void          spectre_page_free      (SpectrePage   *page);

unsigned int  spectre_page_get_index (SpectrePage   *page);
void          spectre_page_get_size  (SpectrePage   *page,
				      double        *width,
				      double        *height);
void          spectre_page_render    (SpectrePage   *page,
				      double         scale,
				      unsigned int   rotation,
				      double         x_dpi,
				      double         y_dpi,
				      unsigned char *page_data);

SPECTRE_END_DECLS

#endif /* SPECTRE_PAGE_H */
