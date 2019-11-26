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

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    SpectreRenderContext *rc;
    SpectreDocument *document;
    char path[] = "/tmp/spectre_fuzz";

    int fd = creat(path, 0644);

    if(fd == -1) return 0;

    write(fd, data, size);

    close(fd);

    document = spectre_document_new();
    if(document == NULL) return 0;

    spectre_document_load(document, path);

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
