#include <queue>
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman/huffman_tree.h"
#include "util.h"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 4)
    {
        printf("Usage: %s input-file output-file tree-file\n", argv[0]);
        return 1;
    }
    FILE *fin = sfopen(argv[1], "r");
    FILE *fout = sfopen(argv[2], "w");
    FILE *ftree = sfopen(argv[3], "w");

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
