#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <string>
#include "util/bitvector.h"

class huffman_node;

// Class encapsulating a huffman tree. Once a tree is constructed, it can
// be used to encode and decode files. Default constructor is hidden
// because the internals of how a tree is represented is hidden, so it
// makes little sense to construct an empty tree. Instead, use the static
// methods from_input_file and from_tree_string.
// Ex:
//     huffman_tree encoding_tree = from_input_file(fin);
//     huffman_tree decoding_tree = from_tree_string(tree_str);
// from_input_file is usually used to construct trees to encode plain
// files, from_tree_string is usually used to construct trees to decode
// files.
//
//
class huffman_tree
{
  public:
    ~huffman_tree();

    static huffman_tree from_input_file(FILE *fin);
    static huffman_tree from_tree_string(std::string tree_str);

    bitvector encode(FILE *fin);
    std::string decode(FILE *fin);
    std::string to_string();

  private:
    huffman_node *root;

    // An index mapping characters to their corresponding bit strings It's
    // indexed by unsigned character values, for example: index['a']
    bitvector *index[256];

    huffman_tree();
    static void freq_list(FILE *fin, int *out);
    void build_index();
    static void readNode(huffman_node * &node, bitvector &data, int &pos,
                         int str_len);
    struct HuffLess;
};

#endif
