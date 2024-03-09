#include "dictionary.h"

#include <cstdio>
#include <map>
#include <queue>
#include <unistd.h>
#include <queue>

#include "files.h"
#include "node.h"

#define KEY first
#define LEN second

typedef std::map<char,std::pair<long long,char>> encoder_map;

void delete_tree (Node* root)
{
    if (root->is_leaf()) {
        delete root;
        return;
    }
    
    Group *node = (Group*) root;
    delete_tree(node->get_left_child());
    delete_tree(node->get_right_child());

    delete node;
}

void get_frequencies(const std::string file, std::map<char,int> &dict)
{
    char element;
    File input (file, true);

    while ((element = input.read_char()) != EOF && element != '\000') {
        dict[element]++;
    }
}

void create_leaves (std::priority_queue<Node*, std::vector<Node*>, Node::Compare>& nodes, const std::map<char,int>& dict)
{
    for (std::pair<char,int> p : dict) {
        Leaf *leaf = new Leaf(p.second, p.first);
        nodes.push(leaf);
    }
}

void create_group_node (std::priority_queue<Node*, std::vector<Node*>, Node::Compare>& nodes)
{
    Node *left = nodes.top();
    nodes.pop();
    Node *right = nodes.top();
    nodes.pop();
    Group *parent = new Group(
            left->get_frequency() + right->get_frequency(),
            left,
            right
            );
    nodes.push(parent);
}

Node* create_tree (const std::map<char,int> &dict)
{
    std::priority_queue<Node*, std::vector<Node*>, Node::Compare> nodes;
    create_leaves(nodes, dict);

    while (nodes.size() > 1) {
        create_group_node(nodes);
    }

    return nodes.top();
}

void get_code (std::pair<int,char> code, encoder_map& code_map, const Node* current)
{
    if (current->is_leaf()) {
        Leaf *leaf = (Leaf*) current;
        code_map[leaf->get_element()] = code;
        return;
    }

    Group *node = (Group*) current;
    if (node->get_left_child() != NULL) {
        get_code({(code.KEY << 1) + 1, code.LEN+1}, code_map, node->get_left_child());
    }
    if (node->get_right_child() != NULL) {
        get_code({code.KEY << 1, code.LEN+1}, code_map, node->get_right_child());
    }
}

void create_idx_map (std::map<void*,int> &index_map, Node *root)
{
    static int cnt = 0;

    index_map[root] = cnt;
    cnt++;

    if (!root->is_leaf()) {
        Group *g = (Group*) root;
        create_idx_map(index_map, g->get_left_child());
        create_idx_map(index_map, g->get_right_child());
    }
}

void write_tree_to_file(std::map<void*,int> &index_map, File &output, Node* root)
{
    if (root->is_leaf()) {
        Leaf *leaf = (Leaf*) root;
        output.write_char('1');
        output.write_int(leaf->get_element());
    }
    else {
        Group *node = (Group*) root;
        output.write_char('0');
        output.write_int(index_map[node->get_left_child()]);
        output.write_int(index_map[node->get_right_child()]);
        write_tree_to_file(index_map, output, node->get_left_child());
        write_tree_to_file(index_map, output, node->get_right_child());
    }
}

void write_dictionary (const std::string& file, Node* root)
{
    File output (file, false);
    std::map<void*,int> index_map;
    create_idx_map(index_map, root);

    write_tree_to_file(index_map, output, root);
    output.flush();
    output.close_file();
}

void encode_file (File &input, File &output, encoder_map& code)
{
    output.write_int(2); // Placeholder for number of bits
    output.flush();

    int bits = 0;
    char element;
    while ((element = input.read_char()) != EOF && element != '\000') {
        bits += (int)code[element].second;
        output.write_bits(code[element]);
    }
    output.move_to_start();
    output.write_int(bits);
    output.flush();
}

// file: file from which to generate the code
// code: empty map to which the code is going to be written
void create_dict(const std::string file, encoder_map& code)
{
    std::map<char,int> dict;
    get_frequencies(file, dict);

    Node *root = create_tree(dict);
    get_code({0,0}, code, root);

    write_dictionary(file + ".dic", root);

    delete_tree(root);
}

void dict::compress(const std::string &file)
{
    encoder_map code;
    create_dict(file, code);
    
    File input (file, true);
    File output (file + ".dc", false);
    encode_file (input, output, code);
    input.close_file();
    output.close_file();
}

void read_dictionary_file (std::vector<std::pair<int,int>> &nodes, File &dic_file)
{
    char current;
    while ((current = dic_file.read_char()) != EOF && current != '\000') {
        std::pair<int,int> tmp;
        if (current == '0') { // It's a group node
            tmp.first = dic_file.read_int();
            tmp.second = dic_file.read_int();
        }
        else {
            tmp.first = -1;
            tmp.second = dic_file.read_int();
        }
        nodes.push_back(tmp);
    }
}

Node* recreate_dictionary (std::vector<std::pair<int,int>> &nodes, int idx)
{
    if (nodes[idx].first < 0) {
        Leaf *leaf = new Leaf(0, nodes[idx].second);
        return leaf;
    }

    Node* left = recreate_dictionary(nodes, nodes[idx].first);
    Node* right = recreate_dictionary(nodes, nodes[idx].second);
    Group *group = new Group(0, left, right);
    return group;
}

Node* read_dictionary (const std::string &file)
{
    File dic_file (file, true);

    std::vector<std::pair<int,int>> nodes;
    read_dictionary_file(nodes, dic_file);

    Node* root = recreate_dictionary (nodes, 0);
    return root;
}

void decode_file(File &input, File &output, Node* root)
{
    Node* current = root;
    int bits = input.read_int();
    char bit;
    while (bits--) {
        bit = input.read_bit();
        if (!current->is_leaf()) {
            Group *node = (Group*) current;
            if (bit == 1)
                current = node->get_left_child();
            else
                current = node->get_right_child();
        }

        if (current->is_leaf()) {
            Leaf* leaf = (Leaf*) current;
            output.write_char(leaf->get_element());
            current = root;
        }
    }
    output.flush();
}

void dict::decompress(const std::string &file)
{
    File input (file, true);
    File output (file.substr(0, file.size()-3), false);
    Node *root = read_dictionary(file.substr(0, file.size()-3) + ".dic");
    decode_file(input, output, root);
    delete_tree(root);
    input.close_file();
    output.close_file();
}
