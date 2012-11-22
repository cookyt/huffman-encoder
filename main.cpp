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

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        printf("Usage: %s input-file output-file\n", argv[0]);
        return 1;
    }
    FILE *fin = fopen(argv[1], "r");
    FILE *fout = fopen(argv[2], "w");
    if (fin == NULL)
    {
        printf("Cannot open '%s' for reading.\n", argv[1]);
        return 1;
    }
    if (fout == NULL)
    {
        printf("Cannot open '%s' for writing.\n", argv[2]);
        return 1;
    }

    huffman_tree tree = huffman_tree::from_input_file(fin);
    rewind(fin);

    fputs(tree.encode(fin).c_str(), fout);

    fclose(fin);
    fclose(fout);

    fin = fopen("out", "r");
    cout << tree.decode(fin);
    fclose(fin);
    return 0;
}
