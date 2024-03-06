#include "node.h"

//////////////////
// GENERIC NODE //
//////////////////

Node::Node (int frequency) {
    _frequency = frequency;
}

int Node::get_frequency() const {
    return _frequency;
}

bool Node::Compare::operator() (Node *fst, Node *snd)
{
    return fst->get_frequency() > snd->get_frequency();
}

Node::~Node() {}

///////////////
// LEAF NODE //
///////////////

Leaf::Leaf(int frequency, char element) :
    Node(frequency)
{
    _element = element;
}

bool Leaf::is_leaf() const {
    return true;
}

char Leaf::get_element() const {
    return _element;
}

Leaf::~Leaf() {}

/////////////////
// MIDDLE NODE //
/////////////////

Group::Group (int frequency, Node *left, Node *right) :
    Node(frequency)
{
    _left_child = left;
    _right_child = right;
}

bool Group::is_leaf() const {
    return false;
}

Node* Group::get_left_child() const {
    return _left_child;
}

Node* Group::get_right_child() const {
    return _right_child;
}

Group::~Group() {}
