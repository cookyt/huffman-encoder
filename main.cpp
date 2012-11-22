#include <queue>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "huffman_tree.hpp"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Usage: %s input-file\n", argv[0]);
        return 1;
    }
    FILE *fin = fopen(argv[1], "r");

    huffman_tree tree = huffman_tree::from_input_file(fin);
    cout << tree.to_string() << endl;

    fclose(fin);
    return 0;
}

