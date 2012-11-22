#ifndef HUFFMAN_TREE_HPP
#define HUFFMAN_TREE_HPP

#include <string>
#include "huffman_node.hpp"

class huffman_tree
{
  private:
    huffman_tree() {}
    static void freq_list(FILE *fin, int *out);

  public:
    huffman_node *root;

    ~huffman_tree();

    static huffman_tree from_input_file(FILE *fin);
    static huffman_tree from_tree_file(FILE *fin);

    std::string encode(FILE *fin);
    std::string decode(FILE *fin);
    std::string to_string();
};

#endif
