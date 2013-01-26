#include <string>
#include <cassert>
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
    fprintf(stderr,
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

enum UsageMode
{
    USE_ENCODE,
    USE_DECODE
};

int main(int argc, char **argv)
{
    FILE *fin;
    FILE *fout = stdout;
    UsageMode mode;

    //Parse options and display help text if needed.
    if (argc >= 3)
    {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
            help_and_die(argv[0]);

        if (strcmp(argv[1], "-e") == 0)
            mode = USE_ENCODE;
        else if (strcmp(argv[1], "-d") == 0)
            mode = USE_DECODE;
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

    //Build huffman tree and process input file by either encoding it or
    //decoding it
    string output;
    switch (mode)
    {
        case USE_ENCODE:
        {
            huffman_tree tree = huffman_tree::generate(fin);
            rewind(fin);
            output = tree.encode(fin);
            break;
        }
        case USE_DECODE:
        {
            huffman_tree tree = huffman_tree::parse(fin);
            output = tree.decode(fin);
            break;
        }
        default:
            //should never get here
            assert(false);
    }

    //Write processed data out to file
    fwrite(output.data(), sizeof(char), output.size(), fout);

    fclose(fin);
    fclose(fout);

    return 0;
}
