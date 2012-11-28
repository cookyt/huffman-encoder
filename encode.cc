#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "huffman/huffman_tree.h"
#include "util/util.h"
#include "util/bitvector.h"

using namespace std;

// Prepends the huffman tree and its length to the output file
void write_header(FILE *fout, huffman_tree& tree)
{
    string tree_str = tree.to_string();
    unsigned short tree_len = tree_str.size();
    fwrite(&tree_len, sizeof(tree_len), 1, fout);
    fwrite(tree_str.data(), sizeof(char), tree_str.size(), fout);
}

void help_and_die(char *program_path)
{
    printf(
        "Huffman Encoder:\n"
        "Encodes the given file. Encoded file is written to standard\n"
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

    huffman_tree tree = huffman_tree::from_input_file(fin);
    rewind(fin);

    // write the encoded data out
    write_header(fout, tree);
    tree.encode(fin).to_file(fout);

    fclose(fin);
    fclose(fout);

    return 0;
}
