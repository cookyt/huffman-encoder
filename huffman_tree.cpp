#include <stdio.h>
#include <string.h>
#include "huffman_tree.hpp"

void huffman_tree::load_freq_list(FILE *fin, int *out)
{
    bzero(out, 255*sizeof(int));
    for (;;)
    {
        char c = fgetc(fin);
        if (feof(fin))
            break;
        out[c]++;
    }
}

void huffman_tree::from_input_file(FILE *fin)
{
    int frequencies[255];
    load_freq_list(fin, frequencies);
}

void huffman_tree::from_tree_file(FILE *fin)
{
}
