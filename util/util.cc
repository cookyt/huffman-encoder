#include <cstdlib>
#include "util/util.h"

FILE *sfopen(const char *path, const char *mode)
{
    FILE *fil = fopen(path, mode);
    if (fil == NULL)
    {
        fprintf(stderr, "Cannot open file '%s' with mode '%s'\n", path, mode);
        exit(1);
    }
    return fil;
}
