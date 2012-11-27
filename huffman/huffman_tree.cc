#include <queue>
#include <stack>
#include <vector>
#include <cassert>
#include <cstdio>
#include <cstring>

#include "huffman_tree.h"
#include "huffman_node.h"
#include "util/bitvector.h"

using namespace std;

huffman_tree::huffman_tree()
{
    bzero(index, 256*sizeof(*index));
    root = NULL;
}

huffman_tree::~huffman_tree()
{
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

    if (root == NULL)
        return;

    // DFS on the tree, and build an array index mapping chars to bit strings
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

            node->right->bits += true;
            node->left->bits += false;

            S.push(node->right);
            S.push(node->left);
        }
        else
        {
            assert(node->left == NULL && node->right == NULL);
            index[(unsigned char)node->ch] = &(node->bits);
        }
    }
}

// Comparator used in the priority queue implementation of
// huffman_tree::from_input_file
struct HuffLess
{
    bool operator()(huffman_node *p1, huffman_node *p2) const 
    {
        return (p1->freq > p2->freq);
    }
};

huffman_tree huffman_tree::from_input_file(FILE *fin)
{
    huffman_tree tree;

    int freq[256];
    freq_list(fin, freq);

    priority_queue<huffman_node*, vector<huffman_node *>, HuffLess> Q;
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

void huffman_tree::readNode(huffman_node * &node, const char *data, int &pos,
                            int str_len)
{
    assert (pos < str_len);
    char c = data[pos++];

    node = new huffman_node(0, c);
    switch (c)
    {
        case '0':
            readNode(node->left, data, pos, str_len);
            readNode(node->right, data, pos, str_len);
            break;
        case '1':
            assert (pos < str_len);
            node->ch = data[pos++];
            break;
        default:
            assert(false);
    }
}

huffman_tree huffman_tree::from_tree_string(string tree_str)
{
    huffman_tree tree;

    if (tree_str.size() > 0)
    {
        // assert (data[0] == 'b');
        int pos=0;
        readNode(tree.root, tree_str.data(), pos, tree_str.size());
    }

    tree.build_index();
    return tree;
}

bitvector huffman_tree::encode(FILE *fin)
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
    return output;
}

string huffman_tree::decode(FILE *fin)
{
    string output = "";
    bitvector input(fin);
    huffman_node *cur = root;
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

string huffman_tree::to_string()
{
    string output = "";
    stack<huffman_node *> S;

    S.push(root);

    //DFS loop
    while (!S.empty())
    {
        huffman_node *node = S.top();
        S.pop();

        if (node == NULL)
            continue;
        
        if (node->left != NULL && node->right != NULL)
        {
            output += '0';
        }
        else
        {
            output += '1';
            output += node->ch;
        }

        S.push(node->right);
        S.push(node->left);
    }

    return output;
}
