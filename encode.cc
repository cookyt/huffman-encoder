#include <string>
#include <cstdio>

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

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf(
            "Huffman Encoder:\n"
            "Encodes the given file. Encoded file is written to standard\n"
            "output.\n\n"
            "Usage: %s input-file\n",
            argv[0]);
        return 1;
    }
    FILE *fin = sfopen(argv[1], "r");
    FILE *fout = stdout;

    huffman_tree tree = huffman_tree::from_input_file(fin);
    rewind(fin);

    // write the encoded data out
    write_header(fout, tree);
    tree.encode(fin).to_file(fout);

    fclose(fin);
    fclose(fout);

    return 0;
}
