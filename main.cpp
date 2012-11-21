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

    huffman_tree tree = huffman_tree::from_string("");

    fclose(fin);
    return 0;
}

