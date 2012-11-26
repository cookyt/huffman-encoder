#include <string>
#include <cstdio>

#include "huffman/huffman_tree.h"
#include "util.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf(
            "Huffman Encoder:\n"
            "Encodes the given file. Tree representation is written\n"
            "to tree-file, encoded file is written to standard\n"
            "output.\n\n"
            "Usage: %s input-file tree-file\n",
            argv[0]);
        return 1;
    }
    FILE *fin = sfopen(argv[1], "r");
    FILE *fout = stdout;
    FILE *ftree = sfopen(argv[2], "w");

    huffman_tree tree = huffman_tree::from_input_file(fin);
    rewind(fin);

    string encoded = tree.encode(fin);
    string tree_str = tree.to_string();
    fwrite(encoded.data(), sizeof(char), encoded.size(), fout);
    fwrite(tree_str.data(), sizeof(char), tree_str.size(), ftree);

    fclose(fin);
    fclose(fout);
    fclose(ftree);

    return 0;
}
