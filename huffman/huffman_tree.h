#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <string>
#include "util/bitvector.h"

class huffman_node;

class huffman_tree
{
  public:
    huffman_node *root;
    bitvector *index[256];

    ~huffman_tree();

    static huffman_tree from_input_file(FILE *fin);
    static huffman_tree from_tree_file(FILE *fin);

    bitvector encode(FILE *fin);
    std::string decode(FILE *fin);
    std::string to_string();

  private:
    huffman_tree();
    static void freq_list(FILE *fin, int *out);
    void build_index();
};

#endif
