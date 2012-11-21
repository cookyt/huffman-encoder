#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include "huffman_node.hpp"

class huffman_tree
{
  private:
    void load_freq_list(FILE *fin, int *out);

  public:
    huffman_node *root;

    huffman_tree();

    void from_input_file(FILE *fin);
    void from_tree_file(FILE *fin);
};

#endif
