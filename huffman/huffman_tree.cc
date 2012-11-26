#include <queue>
#include <stack>
#include <vector>
#include <cassert>
#include <cstdio>
#include <cstring>

#include "huffman_tree.h"
#include "huffman_node.h"

using namespace std;

// Performs a DFS on the tree, deleting its nodes.
huffman_tree::~huffman_tree()
{
    // delete tree
    stack<huffman_node *> S;
    S.push(root);
    while (!S.empty())
    {
        huffman_node *node = S.top();
        S.pop();

        if (node->left != NULL && node->right != NULL)
        {
            S.push(node->right);
            S.push(node->left);
        }
        else
        {
            // For a huffman tree, either both nodes are NULL or both
            // nodes are not NULL, there can't be one NULL and another not
            // NULL.
            assert(node->left == NULL && node->right == NULL);
        }

        delete node;
    }
}

// Goes over the file and counts the frequency of each byte.
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

void huffman_tree::build_index()
{
    bzero(index, 256*sizeof(*index));

    // DFS on the tree, and build an array index mapping chars to bit
    // strings
    stack<huffman_node *> S;
    S.push(root);
    while (!S.empty())
    {
        huffman_node *node = S.top();
        S.pop();

        if (node->left != NULL && node->right != NULL)
        {
            node->right->bits += node->bits;
            node->left->bits += node->bits;

            node->right->bits += '1';
            node->left->bits += '0';

            S.push(node->right);
            S.push(node->left);
        }
        else
        {
            assert(node->left == NULL && node->right == NULL);
            index[node->ch] = &(node->bits);
        }
    }
}

// Comparator for two 
struct HuffLess
{
    bool operator()(huffman_node *p1, huffman_node *p2) const 
    {
        return (*p1 < *p2);
    }
};

// Constructs a huffman tree from any given input file.
// fin - the file to read from.
huffman_tree huffman_tree::from_input_file(FILE *fin)
{
    huffman_tree tree;

    int freq[256];
    freq_list(fin, freq);

    priority_queue<huffman_node *, vector<huffman_node *>,HuffLess> Q;
    for (int i=0; i<256; i++)
    {
        if (freq[i] == 0)
            continue;
        huffman_node *node = new huffman_node(freq[i], i);
        Q.push(node);
    }

    while (Q.size() > 1)
    {
        huffman_node *lnode = Q.top(); Q.pop();
        huffman_node *rnode = Q.top(); Q.pop();
        huffman_node *node = new huffman_node(lnode->freq + rnode->freq);
        node->left = lnode;
        node->right = rnode;
        Q.push(node);
    }

    huffman_node *node = Q.top();
    tree.root = node;
    tree.build_index();

    return tree;
}

void readNode(huffman_node * &node, FILE *fin)
{
    int c = fgetc(fin);
    assert (c == 'b');

    c = fgetc(fin);
    assert (c != EOF);
    node = new huffman_node(0, c);


    c = fgetc(fin);
    assert(c == '[');

    c = fgetc(fin);
    if (c == 'b')
    {
        ungetc(c, fin);
        readNode(node->left, fin);
        readNode(node->right, fin);
        c = fgetc(fin);
        assert(c == ']');
    }
    else
    {
        assert(c == ']');
    }
}

// Constructs a huffman tree from a file containing the contents of
// huffman_tree::to_string.
// fin - the file to read from
huffman_tree huffman_tree::from_tree_file(FILE *fin)
{
    huffman_tree tree;

    int c = fgetc(fin);
    if (c != EOF)
    {
        assert (c == 'b');
        ungetc(c, fin);
        readNode(tree.root, fin);
    }

    return tree;
}

// Encodes a file with this huffman tree. Returns a string of ASCII 1's
// and 0's representing the encoded file. The method reads until EOF.
// fin - the file to read from
string huffman_tree::encode(FILE *fin)
{
    string output = "";
    for (;;)
    {
        unsigned char c = fgetc(fin);
        if (feof(fin))
            break;
        assert(index[c] != NULL);
        output += *(index[c]);
    }
    return output;
}

// Decodes a file using this huffman tree. Quits if an impossible bit
// string is found.
// fin - the file to read from
string huffman_tree::decode(FILE *fin)
{
    string output = "";
    huffman_node *cur = root;
    for (;;)
    {
        char c = fgetc(fin);
        if (feof(fin))
            break;
        switch (c)
        {
            case '0':
                cur = cur->left;
                break;
            case '1':
                cur = cur->right;
                break;
            default:
                assert(false);
        }
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

// Returns a string representation of this huffman tree. This string can
// be saved and fed into huffman_tree::from_tree_file to construct a new
// tree.
// The EBNF of this string is
// ==========================
// Tree ::= N | E
// Node ::= B[C]
// Children ::= NN | E
// Byte ::= b(a single byte)
// Empty ::= (the empty string)
string huffman_tree::to_string()
{
    string output = "";
    stack<huffman_node *> S;

    S.push(root);

    // Used to mark when to print a closing bracket in the DFS stack.
    huffman_node sentinal(-1);

    //DFS loop
    while (!S.empty())
    {
        huffman_node *node = S.top();
        S.pop();

        if (node == NULL)
            continue;

        if (node == &sentinal)
        {
            output += ']';
            continue;
        }

        output += 'b';
        output += node->ch;
        output += '[';

        S.push(&sentinal);
        S.push(node->right);
        S.push(node->left);
    }

    return output;
}
