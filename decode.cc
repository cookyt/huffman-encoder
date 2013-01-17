#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdint.h>

#include "huffman/huffman_tree.h"
#include "util/util.h"

using namespace std;

void help_and_die(char *program_path)
{
    printf(
        "Huffman Decoder:\n"
        "Decodes the given file. Decoded file is written to standard\n"
        "output by default.\n\n"
        "Usage: %s input-file [output-file]\n",
        program_path);
    exit(1);
}

int main(int argc, char **argv)
{
    FILE *fin;
    FILE *fout = stdout;

    if (argc > 1)
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
            help_and_die(argv[0]);

        fin = sfopen(argv[1], "r");
        if (argc > 2)
            fout = sfopen(argv[2], "w");
    }
    else
    {
        help_and_die(argv[0]);
    }

    huffman_tree tree = huffman_tree::parse(fin);

    string decoded = tree.decode(fin);
    fwrite(decoded.data(), sizeof(char), decoded.size(), fout);

    fclose(fin);
    fclose(fout);

    return 0;
}
