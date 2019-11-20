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
	unsigned int i;

	int fd = open("doc", O_CREAT | O_TMPFILE | O_TRUNC);

    if(fd == -1) return 0;

    while(write(fd, data, size) > 0);

    close(fd);

	spectre_document_load(document, "doc");

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