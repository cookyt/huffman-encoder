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
            "Huffman Decoder:\n"
            "Decodes the given file. Tree representation is read\n"
            "from tree-file, decoded file is written to standard\n"
            "output.\n\n"
            "Usage: %s input-file tree-file\n",
            argv[0]);
        return 1;
    }
    FILE *fin = sfopen(argv[1], "r");
    FILE *fout = stdout;
    FILE *ftree = sfopen(argv[2], "r");

    huffman_tree tree = huffman_tree::from_tree_file(ftree);

    string decoded = tree.decode(fin);
    fwrite(decoded.data(), sizeof(char), decoded.size(), fout);

    fclose(fin);
    fclose(fout);
    fclose(ftree);

    return 0;
}
