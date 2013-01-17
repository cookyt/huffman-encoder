#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdint.h>

#include "huffman/huffman_tree.h"
#include "util/util.h"
#include "util/bitvector.h"

using namespace std;

void help_and_die(char *program_path)
{
    printf(
        "Huffman Encoder/Decoder:\n"
        "Encodes or decodes the given file. Output is written to standard\n"
        "output by default.\n\n"
        "Usage:\n"
        "\t%s (-e | -d) input-file [output-file]\n"
        "Flags:\n"
        "\t-e : encode\n"
        "\t-d : decode\n"
        "\t-h or --help : display this help text\n",
        program_path);
    exit(1);
}

int main(int argc, char **argv)
{
    FILE *fin;
    FILE *fout = stdout;
    bool encode_mode;

    if (argc >= 3)
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
            help_and_die(argv[0]);

        if (strcmp(argv[1], "-e") == 0)
            encode_mode = true;
        else if (strcmp(argv[1], "-d") == 0)
            encode_mode = false;
        else
            help_and_die(argv[0]);

        fin = sfopen(argv[2], "r");
        if (argc >= 4)
            fout = sfopen(argv[3], "w");
    }
    else
    {
        help_and_die(argv[0]);
    }

    if (encode_mode)
    {
        huffman_tree tree = huffman_tree::generate(fin);
        rewind(fin);

        string encoded = tree.encode(fin);
        fwrite(encoded.data(), sizeof(char), encoded.size(), fout);
    }
    else
    {
        huffman_tree tree = huffman_tree::parse(fin);

        string decoded = tree.decode(fin);
        fwrite(decoded.data(), sizeof(char), decoded.size(), fout);
    }

    fclose(fin);
    fclose(fout);

    return 0;
}
