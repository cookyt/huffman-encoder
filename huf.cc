#include <string>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdint.h>

#include "huffman/huffman_tree.h"
#include "util/util.h"
#include "util/bitvector.h"

const char *kDecodeBinName = "dehuf";
const char *kInputFileErrorMsg = "Must specify an input file when encoding.";

enum UsageMode
{
    USE_ENCODE,
    USE_DECODE
};

void help_and_die(char *program_path)
{
    fprintf(stderr,
        "Huffman Encoder/Decoder:\n"
        "Encodes or decodes the given file. Output is written to standard\n"
        "output by default.\n\n"
        "Usage:\n"
        "\t%s [-e | -d] input-file [output-file]\n"
        "Flags:\n"
        "\t-e : force encode\n"
        "\t-d : force decode\n"
        "\t-h or --help : display this help text\n",
        program_path);
    exit(EXIT_FAILURE);
}

void die(bool condition, const char *message)
{
    if (condition)
    {
        fputs(message, stderr);
        fputs("\n", stderr);
        exit(EXIT_FAILURE);
    }
}

bool streq(const char *str1, const char *str2)
{
    return (strcmp(str1, str2) == 0);
}

void choose_files_and_mode(int argc, char **argv, FILE *& fin, FILE *& fout,
                           UsageMode &mode)
{
    fin = stdin;
    fout = stdout;
    if (streq(argv[0], kDecodeBinName))
        mode = USE_DECODE;
    else
        mode = USE_ENCODE;

    if (argc >= 2)
    {
        // Check if user is passing cmd line switch to explicitly choose
        // encoding or decoding, or if user is asking for help
        bool triggered = true;
        if (streq(argv[1], "-e"))
            mode = USE_ENCODE;
        else if (streq(argv[1], "-d"))
            mode = USE_DECODE;
        else if (streq(argv[1], "-h") || streq(argv[1], "--help"))
            help_and_die(argv[0]);
        else
            triggered = false;

        // If user used a switch, then all expected arguments are shifted one
        // to the left.
        int arg_offset = 0;
        if (triggered)
            arg_offset = 1;
    
        // Encode mode requires a physical input file on disk to work; in
        // decode mode, the input is optional (default to stdin)
        die((mode == USE_ENCODE) && (argc < (2 + arg_offset)),
            kInputFileErrorMsg);

        if (argc >= 2 + arg_offset)
            fin = sfopen(argv[1 + arg_offset], "r");
        if (argc >= 3 + arg_offset)
            fout = sfopen(argv[2 + arg_offset], "w");

    }
    else if (mode == USE_ENCODE)
    {
        // mode is encode, and no input file specified
        die(true, kInputFileErrorMsg);
    }
    else {} // mode is decode and no args specified, treat like a stream cipher
}

int main(int argc, char **argv)
{
    FILE *fin;
    FILE *fout;
    UsageMode mode;

    choose_files_and_mode(argc, argv, fin, fout, mode);

    //Build huffman tree and process input file by either encoding it or
    //decoding it
    std::string output;
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
