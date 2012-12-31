#include <stdint.h>
#include <cstdio>

int main(int argc, char **argv)
{
    if (argc != 2)
        puts("Expects input file as argument");
    FILE *fin = fopen(argv[1], "r");
    uint16_t header_size;
    fread(&header_size, sizeof(header_size), 1, fin);
    printf("Header is %u bytes long.\n", header_size);
    return 0;
}
