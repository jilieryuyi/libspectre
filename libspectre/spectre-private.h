#ifndef SPECTRE_PRIVATE_H
#define SPECTRE_PRIVATE_H

#include "spectre-macros.h"
#include "spectre-page.h"

SPECTRE_BEGIN_DECLS

SpectrePage *_spectre_page_new (unsigned int page_index);

SPECTRE_END_DECLS

#endif /* SPECTRE_PRIVATE_H */
