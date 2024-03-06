#include "dictionary.h"

#include <map>
#include <queue>
#include <unistd.h>
#include <queue>

#include "files.h"
#include "node.h"

#define KEY first
#define LEN second

typedef std::map<char,std::pair<int,char>> encoder_map;

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

void create_idx_map (std::map<void*,int> &index_map, Node* root, int cnt)
{
    index_map[root] = cnt;
    if (!root->is_leaf()) {
        Group *g = (Group*) root;
        create_idx_map(index_map, g->get_left_child(), cnt+1);
        create_idx_map(index_map, g->get_right_child(), cnt+2);
    }
}

void write_map_to_file(std::map<void*,int> &index_map, File &output, Node* root)
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
        write_map_to_file(index_map, output, node->get_left_child());
        write_map_to_file(index_map, output, node->get_right_child());
    }
}

void write_dictionary (const std::string& file, Node* root)
{
    File output (file, false);
    std::map<void*,int> index_map;
    create_idx_map(index_map, root, 0);

    write_map_to_file(index_map, output, root);
    output.flush();
}

void encode_file (File &input, File &output, encoder_map& code)
{
    output.write_int(2); // Placeholder for number of bits
    output.flush();

    // BUG: Bit count is not correct ???
    int bits = 0;
    char element;
    while ((element = input.read_char()) != EOF && element != '\000') {
        bits += code[element].second;
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
    
    for (auto p : code) {
        printf("%c : %d with length %d\n", p.first, p.second.first, p.second.second);
    }

    File input (file, true);
    File output (file + ".dc", false);
    encode_file (input, output, code);
    // TODO: Delete file objects
}
    delete_tree(root);
}
