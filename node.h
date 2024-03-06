#pragma once

class Node {
    private: 
        int _frequency;

    public:
        Node(int frequency);
        ~Node();
        int get_frequency() const;
        virtual bool is_leaf() const = 0;

        class Compare {
            public:
                bool operator() (Node*, Node*);
        };
};

class Leaf : public Node {
    private:
        char _element;

    public:
        Leaf(int frequency, char element);
        ~Leaf();
        bool is_leaf() const;
        char get_element() const;
};

class Group : public Node {
    private:
        Node *_left_child;
        Node *_right_child;

    public:
        Group(int frequency, Node *left, Node *right);
        ~Group();
        bool is_leaf() const;
        Node* get_left_child() const;
        Node* get_right_child() const;
};
