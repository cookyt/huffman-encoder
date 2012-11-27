#include <iostream>
#include <string>
#include <cstdio>

#include "huffman/huffman_tree.h"
#include "util/util.h"

using namespace std;

string read_header(FILE *fin)
{
    unsigned short tree_len;
    fread(&tree_len, sizeof(tree_len), 1, fin);

    char *tree_str = new char[tree_len];
    fread(tree_str, sizeof(*tree_str), tree_len, fin);
    
    string ret(tree_str, tree_len);

    delete[] tree_str;
    return ret;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf(
            "Huffman Decoder:\n"
            "Decodes the given file. Decoded file is written to standard\n"
            "output.\n\n"
            "Usage: %s input-file\n",
            argv[0]);
        return 1;
    }
    FILE *fin = sfopen(argv[1], "r");
    FILE *fout = stdout;

    string str = read_header(fin);
    huffman_tree tree = huffman_tree::from_tree_string(str);

    string decoded = tree.decode(fin);
    fwrite(decoded.data(), sizeof(char), decoded.size(), fout);

    fclose(fin);
    fclose(fout);

    return 0;
}
