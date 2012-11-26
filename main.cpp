#include <queue>
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "huffman_tree.hpp"

using namespace std;

void DFS(huffman_node *node)
{
    if (node == NULL)
        return;

    if (node->left == NULL)
    {
        printf("%c: %s\n", node->ch, node->bits.c_str());
        return;
    }

    DFS(node->left);
    DFS(node->right);
}

FILE *sfopen(const char *path, const char *mode)
{
    FILE *fil = fopen(path, mode);
    if (fil == NULL)
    {
        printf("Cannot open file '%s' with mode '%s'\n", path, mode);
        exit(1);
    }
    return fil;
}

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

    fputs(tree.encode(fin).c_str(), fout);
    fputs(tree.to_string(fin).c_str(), ftree);

    fclose(fin);
    fclose(fout);
    fclose(ftree);

    return 0;
}
