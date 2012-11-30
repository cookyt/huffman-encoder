#include "huffman_tree.h"

#include <cassert>
#include <cstdio>
#include <cstring>
#include <stdint.h>
#include <queue>
#include <stack>
#include <vector>

using namespace std;

// Represents a single node in a huffman tree.
// A node is internal if neither of its children are NULL.
class huffman_tree::node
{
  public:
    node *left;
    node *right;

    // The character contained in this node
    char ch;

    // Frequency of this character (doesn't get set if tree is read from a
    // tree file)
    int freq;

    // Cached bitstring represented by this node
    bitvector bits;

    node(int frequency=0, char chr='\0')
    {
        left = NULL;
        right = NULL;
        freq = frequency;
        ch = chr;
    }
};

// Constructs an empty tree w/ empty index and NULL root.
huffman_tree::huffman_tree()
{
    bzero(index, 256*sizeof(*index));
    root = NULL;
}

    // Performs a DFS on the tree, deleting its nodes.
huffman_tree::~huffman_tree()
{
    stack<node *> S;
    S.push(root);
    while (!S.empty())
    {
        node *cur = S.top();
        S.pop();

        if (cur->left != NULL && cur->right != NULL)
        {
            S.push(cur->right);
            S.push(cur->left);
        }
        else
        {
            // For a huffman tree, either both nodes are NULL or both
            // nodes are not NULL, there can't be one NULL and another not
            // NULL.
            assert(cur->left == NULL && cur->right == NULL);
        }

        delete cur;
    }
}

// Goes over the file and counts the frequency of each byte.
//
// fin - the file to read from
// out - [out] an array of at least 256 ints
void huffman_tree::freq_list(FILE *fin, int *out)
{
    bzero(out, 256*sizeof(*out));
    for (;;)
    {
        unsigned char c = fgetc(fin);
        if (feof(fin))
            break;
        out[c]++;
    }
}

// Builds an index of characters to bitvectors to aid in the encoding.
// Traverses the tree with a DFS, and sets the bits member of each node to
// a bitvector containing the encoded bitvector for that node in the
// context of the tree.
void huffman_tree::build_index()
{
    bzero(index, 256*sizeof(*index));

    if (root == NULL)
        return;

    // DFS on the tree, and build an array index mapping chars to bit strings
    stack<node *> S;
    S.push(root);
    while (!S.empty())
    {
        node *cur = S.top();
        S.pop();

        if (cur->left != NULL && cur->right != NULL)
        {
            cur->right->bits += cur->bits;
            cur->left->bits += cur->bits;

            cur->right->bits += true;
            cur->left->bits += false;

            S.push(cur->right);
            S.push(cur->left);
        }
        else
        {
            assert(cur->left == NULL && cur->right == NULL);
            index[(unsigned char)cur->ch] = &(cur->bits);
        }
    }
}

// Comparator used in the priority queue implementation of
// huffman_tree::from_input_file
struct huffman_tree::HuffLess
{
    bool operator()(node *p1, node *p2) const 
    {
        return (p1->freq > p2->freq);
    }
};

// Constructs a huffman tree from any given input file. It does this by
// building a frequency list for the file and building the huffman tree
// from that. It also builds the index when it's done.
//
// fin - the file to read from.
huffman_tree huffman_tree::from_input_file(FILE *fin)
{
    huffman_tree tree;

    int freq[256];
    freq_list(fin, freq);

    priority_queue<node*, vector<node *>, HuffLess> Q;
    for (int i=0; i<256; i++)
    {
        if (freq[i] == 0)
            continue;
        node *cur = new node(freq[i], i);
        Q.push(cur);
    }

    while (Q.size() > 1)
    {
        node *lnode = Q.top(); Q.pop();
        node *rnode = Q.top(); Q.pop();
        node *cur = new node(lnode->freq + rnode->freq);
        cur->left = lnode;
        cur->right = rnode;
        Q.push(cur);
    }

    node *troot = Q.top();
    tree.root = troot;
    tree.build_index();

    return tree;
}

// Recursive parsing helper method. Reads a single node into the tree, and
// recursively adds its children too. It can be thought of the helper
// method to a DFS.
//
// node - pointer to where this node will be referenced in the tree.
// data - character buffer containing the huffman tree
// pos - the position of the next character in the buffer (initially 0)
// str_len - the number of characters in the buffer
void huffman_tree::readNode(node * &cur, bitvector &data, int &pos,
                            int str_len)
{
    assert (pos < str_len);
    bool b = data[pos++];

    cur = new node();
    switch (b)
    {
        case false:
            readNode(cur->left, data, pos, str_len);
            readNode(cur->right, data, pos, str_len);
            break;
        case true:
            assert (pos < str_len);
            cur->ch = data.char_at(pos);
            pos += 8;
            break;
        default:
            assert(false);
    }
}

// Constructs a huffman tree from a string containing the contents of
// huffman_tree::serialize. It also builds the index when it's done.
huffman_tree huffman_tree::from_tree_string(string tree_str)
{
    huffman_tree tree;

    bitvector data(tree_str);
    if (tree_str.size() > 1)
    {
        // assert (data[0] == 'b');
        int pos=0;
        readNode(tree.root, data, pos, data.size());
    }

    tree.build_index();
    return tree;
}

// Encodes a file with this huffman tree.  Returns a bitvector representing
// the encoded data. The method reads until EOF.
//
// fin - the file to read from
// include_header - whether the output string should include the
//                  serialized header
string huffman_tree::encode(FILE *fin, bool include_header)
{
    bitvector output;
    for (;;)
    {
        unsigned char c = fgetc(fin);
        if (feof(fin))
            break;
        assert(index[c] != NULL);
        output += *(index[c]);
    }

    if (include_header)
    {
        string out_str = serialize();
        out_str += output.to_string();
        return out_str;
    }
    else
    {
        return output.to_string();
    }
}

// Decodes a file using this huffman tree.  Quits if an impossible bit
// string is found.
// 
// fin - the file to read from
string huffman_tree::decode(FILE *fin)
{
    string output = "";
    bitvector input(fin);
    node *cur = root;
    for (int i=0; i<input.size(); i++)
    {
        assert(cur != NULL);
        switch (input[i])
        {
            case false:
                cur = cur->left;
                break;
            case true:
                cur = cur->right;
                break;
            default:
                assert(false);
        }
        assert(cur != NULL);
        if (cur->left == NULL && cur->right == NULL)
        {
            output += cur->ch;
            cur = root;
        }
        else
        {
            assert(cur->left != NULL && cur->right != NULL);
        }
    }
    return output;
}

// Returns a string representation of this huffman tree. This string can be
// saved and fed into huffman_tree::from_tree_string  to construct a new
// tree.
string huffman_tree::serialize()
{
    bitvector output;
    stack<node *> S;

    S.push(root);

    //DFS loop
    while (!S.empty())
    {
        node *cur = S.top();
        S.pop();

        if (cur == NULL)
            continue;
        
        if (cur->left != NULL && cur->right != NULL)
        {
            output += false;
        }
        else
        {
            output += true;
            output += cur->ch;
        }

        S.push(cur->right);
        S.push(cur->left);
    }

    string str_out = output.to_string();
    uint16_t len = str_out.size();
    str_out.insert(0, (const char *) &len, sizeof(len));
    return str_out;
}
