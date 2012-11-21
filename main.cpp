#include <stdio.h>
#include <string.h>
#include "huffman_tree.hpp"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s input-file\n", argv[0]);
        return 1;
    }
    FILE *fin = fopen(argv[1], "r");

    fclose(fin);
    return 0;
}

