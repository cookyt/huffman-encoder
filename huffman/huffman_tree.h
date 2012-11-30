#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <string>
#include "util/bitvector.h"

// Class encapsulating a huffman tree. Once a tree is constructed, it can
// be used to encode and decode files. Use the static
// methods from_input_file and from_tree_string to construct a tree.
// Ex:
//     huffman_tree encoding_tree = from_input_file(fin);
//     huffman_tree decoding_tree = from_tree_string(tree_str);
// from_input_file is usually used to construct trees to encode plain
// files, from_tree_string is usually used to construct trees to decode
// files.
class huffman_tree
{
  public:
    ~huffman_tree();

    static huffman_tree from_input_file(FILE *fin);
    static huffman_tree from_tree_string(std::string tree_str);

    std::string encode(FILE *fin, bool include_header=true);
    std::string decode(FILE *fin);
    std::string serialize();

  private:
    struct HuffLess;
    class node;

    node *root;

    // An index mapping characters to their corresponding bit strings It's
    // indexed by unsigned character values, for example: index['a']
    bitvector *index[256];

    // Default constructor is hidden because the internals of how a tree is
    // represented is hidden, so it makes little sense to construct an empty
    // tree. 
    huffman_tree();

    static void freq_list(FILE *fin, int *out);
    void build_index();
    static void readNode(node * &cur, bitvector &data, int &pos,
                         int str_len);
};

#endif
