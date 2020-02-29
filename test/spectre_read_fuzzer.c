#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <inttypes.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../libspectre/spectre.h"
#include "../libspectre/spectre-utils.h"
#include "../libspectre/ps.h"

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size);

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    SpectreDocument *document;

    document = spectre_document_new();
    if(document == NULL) return 0;

    spectre_document_load_from_data(document, (void*)data, size);

    if(spectre_document_status(document))
    {
        spectre_document_free(document);
        return 0;
    }

    spectre_document_free(document);

    return 0;
}

#ifdef __cplusplus
}
#endif
