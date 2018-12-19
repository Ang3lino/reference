
#include <iostream>
#include <vector>
#include <array>
#include <map>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

template<typename T>
struct KNode {
    vector<KNode *> children;
    int curr_pos;
    T data;

    KNode(const int &n) {
        children.resize(n);
        for (int i = 0; i < n; ++i) children[i] = nullptr;
        curr_pos = 0;
    }

    KNode(const int &n, const T &_data): data(_data) {
        children.resize(n);
        for (int i = 0; i < n; ++i) children[i] = nullptr;
        curr_pos = 0;
    }

    KNode(const vector<KNode *> &_children)  {
        children = _children;
        curr_pos = _children.size();
    }

    inline void append(const vector<KNode *> &_children) { children = _children; }
    inline void append(const KNode son) { children[curr_pos++] = son; }

};

template<typename T> 
void preorder(KNode<T> *node) {
    if (node == nullptr) {
        //printf("nil-");
    } else {
        cout << "" << node->data << "-";
        for (int i = 0; i < node->children.size(); ++i) {
            preorder(node->children[i]);
        }
    }
}

// helper function
KNode<char> *_deserialize(const string &src, int &pos) {
    if (pos >= src.size()) return nullptr;
    vector<KNode<char> *> children(4);
    const char token = src[pos++];
    if (token == 'p') {
        for (int i = 0; i < 4; ++i)
            children[i] = _deserialize(src, pos);
        auto node = new KNode<char> (children);
        node->data = token;
        return node;
    } // else it is a full or empty pixel
    return new KNode<char> (4, token);
}

KNode<char> *deserialize(const string &src) {
    int zero = 0;
    return _deserialize(src, zero);
}

/**
 * ee
 * ep
 * pe
 * pp 
 * 
 * ff
 * ef
 * fe
 * fp
 * pf
 */
KNode<char> *interpolate_quadtrees(KNode<char> *&a, KNode<char> *&b) {
    if (a != nullptr and b == nullptr) return a;
    if (a == nullptr and b != nullptr) return b;
    if (a == nullptr and b == nullptr) return nullptr;

    // both a and b are not nullptr
    if (a->data == 'f') return a;
    if (b->data == 'f') return b;
    if (a->data == 'p' and b->data == 'p') {
        vector<KNode<char> *> children(4);
        for (int i = 0; i < 4; ++i) {
            children[i] = interpolate_quadtrees(a->children[i], b->children[i]);
        }
        auto node = new KNode<char>(children);
        node->data = 'p';
        return node;
    }
    if (a->data == 'p') return a;
    if (b->data == 'p') return b;
    return a; // or b, we don't care given that a is e and b is e
}

int sum_pixels( KNode<char> *&root, int amount = 1024) {
    int sum = 0;
    if (root->data == 'p') {
        for (auto x: root->children) {
            if (x == nullptr) continue;
            sum += sum_pixels(x, amount >> 2);
        }
    } 
    if (root->data == 'f') sum += amount;
    return sum;
}

template<typename T> 
void delete_ktree(KNode<T> *&node) {
    if (node == nullptr) return;
    for (int i = 0; i < node->children.size(); ++i)
        delete_ktree(node->children[i]);
    delete node;
}

void test() {
    string s1, s2;
    array<KNode<char> *, 3> trees;

    //string s1 = "ppeeefpffeefe", s2 = "pefepeefe";
    //string s1 = "peeef", s2 = "peefe";
    //string s1 = "peeef", s2 = "peepefefe";

    s1 = "ppeeefpffeefe", s2 = "pefepeefe";
    trees[0] = deserialize(s1);
    cout << sum_pixels(trees[0]) << endl;
    trees[1] = deserialize(s2);
    cout << sum_pixels(trees[1]) << endl;
    trees[2] = interpolate_quadtrees(trees[0], trees[1]);
    cout << sum_pixels(trees[2]) << endl;

    for (int i = 0; i < 3; ++i) {
        cout << endl;
        preorder(trees[i]);
        cout << endl;
    }

    // s1 = "peeef", s2 = "peefe";
    // trees[0] = deserialize(s1);
    // trees[1] = deserialize(s2);
    // trees[2] = interpolate_quadtrees(trees[0], trees[1]);
    // cout << sum_pixels(trees[2]) << endl;

    // s1 = "peeef", s2 = "peepefefe";
    // trees[0] = deserialize(s1);
    // trees[1] = deserialize(s2);
    // trees[2] = interpolate_quadtrees(trees[0], trees[1]);
    // cout << sum_pixels(trees[2]) << endl;

}


void ask_parameters() {
    int n;
    cin >> n;
    while (n--) {
        string s1, s2;
        cin >> s1;
        cin >> s2;
        auto tree1 = deserialize(s1);
        auto tree2 = deserialize(s2);
        auto result = interpolate_quadtrees(tree1, tree2);
        cout << "There are " << sum_pixels(result) << " black pixels." << endl;
    }
}

int main(int argc, char const *argv[]) {
    //test();
    ask_parameters();
    return 0;
}

/*

3
ppeeefpffeefe
pefepeefe
peeef
peefe
peeef
peepefefe

*/