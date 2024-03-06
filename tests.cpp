#include "tests.h"


#include "files.h"

void compression_ratio (const std::string original_s, const std::string compressed_s)
{
    File original (original_s, true);
    File compressed (compressed_s, true);
    printf("Original size: %d\n", original.get_size());
    printf("Compressed size: %d\n", compressed.get_size());
    printf("Compression ratio: %f\n", (float)original.get_size() / (float)compressed.get_size());
}
