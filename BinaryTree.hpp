/** @author  ±ÃÏ“› */
#pragma once

#include <exception>
#include <initializer_list>
#include <stdexcept>
#include <assert.h>
#include <functional>

#include"memory.hpp"

#define TEST_ACCESS public;

template<typename MyTree>
class BinaryTreeIterator :public iterator_types<MyTree, std::bidirectional_iterator_tag> {  //use inorder traversal
    friend MyTree;
public:
    using iterator_types    = iterator_types<MyTree, std::bidirectional_iterator_tag>;
    using iterator_category = typename iterator_types::iterator_category;
    using value_type        = typename iterator_types::value_type;
    using pointer           = typename iterator_types::pointer;
    using reference         = typename iterator_types::reference;
    using NodePtr           = typename iterator_types::NodePtr;
    using self              = typename iterator_types::self;
    using difference_type   = typename iterator_types::difference_type;
    using size_type         = typename iterator_types::size_type;

    BinaryTreeIterator() = delete;

    explicit BinaryTreeIterator(NodePtr node)
        :_node(node) {}

protected:
    void increment()
    {
        if (_node->_right) {  //has right child
            _node = _node->_right;
            _node = MyTree::_LeftmostPtr(_node);

            return;
        }
        else {
            if (_node == _node->_parent->_right && _node->_parent->_parent == nullptr) {  //iterator on the root with no right child
                _node = _node->_parent;
                return;
            }

            while (_node == _node->_parent->_right) {
                _node = _node->_parent;
            }
            _node = _node->_parent;

            return;
        }
    }

    void decrement()
    {
        if (_node->_left) {  //has left child
            _node = _node->_left;
            _node = MyTree::_RightmostPtr(_node);

            return;
        }
        else {
            while (_node == _node->_parent->_left)
                _node = _node->_parent;
            _node = _node->_parent;

            return;
        }
    }

public:
    reference operator *() const { return _node->_val; }
    pointer operator->()   const { return &(operator*()); }
    self& operator++()
    {
        increment();
        return *this;
    }
    self operator++(int)
    {
        self temp = *this;
        operator++();
        return temp;
    }
    self& operator--()
    {
        if (_node->_parent == nullptr) {  //iterator on the head
            _node = _node->_right;
            return _node;
        }

        decrement();
        return *this;
    }
    self operator--(int)
    {
        self temp = *this;
        operator--();
        return temp;
    }
    bool operator==(const self& rhs) { return this->_node == rhs._node; }
    bool operator!=(const self& rhs) { return this->_node != rhs._node; }

protected:
    NodePtr _node;
};

template<typename Entry>
struct BinaryTreeNode {
    using NodePtr    = BinaryTreeNode<Entry>*;
    using NodeRef    = BinaryTreeNode<Entry>&;
    using value_type = Entry;

    BinaryTreeNode()
        :BinaryTreeNode(nullptr, nullptr, nullptr) {}

    BinaryTreeNode(NodePtr parent) = delete;

    BinaryTreeNode(NodePtr left, NodePtr right) = delete;

    BinaryTreeNode(NodePtr parent, NodePtr left, NodePtr right)
        :_parent(parent), _left(left), _right(right) {}

    BinaryTreeNode(value_type val)
        :_parent(nullptr), _left(nullptr), _right(nullptr), _val(val) {}

    NodePtr _parent;
    NodePtr _left;
    NodePtr _right;

    value_type _val;
};

template<typename val,
    typename key = val,
    typename val_comp = std::less<val>,
    typename key_comp = val_comp >
class _Tree_example_traits {
public:
    using value_type    = val;
    using key_type      = key;
    using value_compare = val_comp;
    using key_compare   = key_comp;

    static key_type KeyOfVal(value_type _val) { return _val; }
};

enum _TreeChild {
    _Right,
    _Left,
    _Head,
    _Root,
    _Unused
};

template<typename MyTraits>
class BinaryTree {
public:
    using value_type      = typename MyTraits::value_type;
    using key_type        = typename MyTraits::key_type;
    using value_compare   = typename MyTraits::value_compare;
    using key_compare     = typename MyTraits::key_compare;
    using difference_type = ptrdiff_t;
    using size_type       = size_t;

public:
    using Node            = BinaryTreeNode<value_type>;
    using NodePtr         = BinaryTreeNode<value_type>*;
    using self            = BinaryTree<MyTraits>;

public:
    using pointer         = value_type*;
    using reference       = value_type&;
    using iterator        = BinaryTreeIterator<self>;

    friend class BinaryTreeIterator<self>;

public:
    BinaryTree()
        :_root(new Node())
    {
        _root->_left = _root;
        _root->_right = _root;
    }

    ~BinaryTree()
    {
        _Destroy();
    }

protected:
    void _DestroyNode(NodePtr node)
    {
        delete node;
    }

    void _DestroyTree(NodePtr subroot)
    {
        if (subroot) {
            _DestroyTree(subroot->_left);
            _DestroyTree(subroot->_right);
            _DestroyNode(subroot);
        }
    }

private:
    void _Destroy()
    {
        if (_root->_parent) {
            _DestroyNode(_root->_parent);
            _DestroyTree(_root);
        }
        else
            _DestroyNode(_root);
    }

protected:
    void _RotateRight(NodePtr subroot)  //root isn't specially handled
    {
        _GetChildRef(subroot->_parent) = subroot;
        subroot->_parent = subroot->_parent->_parent;

        NodePtr subrootParent = subroot->_parent;
        NodePtr subrootRight = subroot->_right;

        subroot->_right = subrootParent;
        subrootParent->_parent = subroot;

        subrootParent->_left = subrootRight;
        subrootRight->_parent = subrootParent;

    }

    void _RotateLeft(NodePtr subroot)  //root isn't specially handled
    {
        _GetChildRef(subroot->_parent) = subroot;
        subroot->_parent = subroot->_parent->_parent;

        NodePtr subrootParent = subroot->_parent;
        NodePtr subrootLeft = subroot->_left;

        subroot->_left = subrootParent;
        subrootParent->_parent = subroot;

        subrootParent->_right = subrootLeft;
        subrootLeft->_parent = subrootParent;
    }

private:
    key_compare key_comp() const noexcept
    {
        return key_compare();
    }

    value_compare value_comp() const noexcept
    {
        return value_compare(key_comp());
    }

public:
    iterator begin() const noexcept { return iterator(_GetHead()->_left); }
    iterator end()   const noexcept { return iterator(_GetHead()); }
    //iterator getRoot() const noexcept { return iterator(_root); }
    bool empty()     const noexcept { return _root == _GetHead(); }

    iterator find(const key_type& key) const noexcept
    {
        if (empty())  //empty tree
            return end();

        return unchecked_find(_root, key);
    }

    virtual iterator insert(const value_type& val)
    {
        if (empty()) {  //empty tree
            NodePtr root = new Node(val);
            root->_parent = _root;
            _root->_left = root;
            _root->_right = root;
            _root = root;

            return iterator(_root);
        }

        return iterator(unchecked_insert(_root, val));
    }

    virtual iterator remove(const key_type& keyval)
    {
        iterator keyPos = find(keyval);
        if (keyPos == end())  //not found
            return keyPos;

        return iterator(unchecked_remove(keyPos._node));
    }

protected:
    iterator unchecked_find(NodePtr subroot, const key_type& key) const noexcept
    {
        //get key
        key_type subrootKey = MyTraits::KeyOfVal(subroot->_val);

        //found
        if (key == subrootKey)
            return iterator(subroot);

        //recursive search
        if (key_comp()(key, subrootKey)) {
            if (subroot->_left)
                return unchecked_find(subroot->_left, key);
            else
                return end();
        }
        else {
            if (subroot->_right)
                return unchecked_find(subroot->_right, key);
            else
                return end();
        }
    }

    NodePtr unchecked_insert(NodePtr subroot, const value_type& val)
    {
        //recursive insertion
        if (value_comp()(val, subroot->_val)) {
            if (subroot->_left) {
                return unchecked_insert(subroot->_left, val);
            }
            else {
                NodePtr newNode = new Node(val);
                newNode->_parent = subroot;
                subroot->_left = newNode;

                if (subroot == _GetHead()->_left)
                    _GetHead()->_left = newNode;

                return newNode;
            }
        }
        else {
            if (subroot->_right) {
                return unchecked_insert(subroot->_right, val);
            }
            else {
                NodePtr newNode = new Node(val);
                newNode->_parent = subroot;
                subroot->_right = newNode;

                if (subroot == _GetHead()->_right)
                    _GetHead()->_right = newNode;

                return newNode;
            }
        }
    }

    NodePtr unchecked_remove(NodePtr subroot)
    {
        //get reference to subroot(to change the status of parent if it has one)
        const NodePtr parent = subroot->_parent;
        NodePtr& newsubroot = _GetChildRef(subroot);

        //removal
        if (newsubroot->_left == nullptr) {  //left node empty
            newsubroot = newsubroot->_right;
            if (newsubroot)
                newsubroot->_parent = parent;
        }
        else if (newsubroot->_right == nullptr) {  //right node empty
            newsubroot = newsubroot->_left;
            if (newsubroot)
                newsubroot->_parent = parent;
        }
        else {
            newsubroot = new Node(_RightmostPtr(subroot->_left)->_val);  //duplicate the new root value

            //links
            newsubroot->_left = subroot->_left;
            newsubroot->_right = subroot->_right;
            subroot->_right->_parent = newsubroot;
            newsubroot->_parent = parent;
            if (subroot->_left == _RightmostPtr(subroot->_left))
                subroot->_left->_parent = newsubroot;

            //remove the node duplicated
            unchecked_remove(_RightmostPtr(subroot->_left));
        }

        //modify the header's child
        if (subroot == _GetHead()->_left || subroot == _GetHead()->_right)
            _HeadLink();

        delete subroot;

        NodePtr returnval = newsubroot;
        return returnval;
    }

public:
    void preorderTraverse(std::function<void(const iterator)> func)
    {
        if (empty())  //empty tree
            return;

        pretraverse_aux(_root, func);
    }

    void inorderTraverse(std::function<void(const iterator)> func)
    {
        if (empty())  //empty tree
            return;

        intraverse_aux(_root, func);
    }

    void postorderTraverse(std::function<void(const iterator)> func)
    {
        if (_root == _GetHead())  //empty tree
            return;

        posttraverse_aux(_root, func);
    }

private:
    void pretraverse_aux(NodePtr subroot, std::function<void(const iterator)> func)
    {
        if (subroot == nullptr) {
            return;
        }
        else {
            func(iterator(subroot));
            pretraverse_aux(subroot->_left, func);
            pretraverse_aux(subroot->_right, func);
        }
    }

    void intraverse_aux(NodePtr subroot, std::function<void(const iterator)> func)
    {
        if (subroot == nullptr) {
            return;
        }
        else {
            intraverse_aux(subroot->_left, func);
            func(iterator(subroot));
            intraverse_aux(subroot->_right, func);
        }
    }

    void posttraverse_aux(NodePtr subroot, std::function<void(const iterator)> func)
    {
        if (subroot == nullptr) {
            return;
        }
        else {
            posttraverse_aux(subroot->_left, func);
            posttraverse_aux(subroot->_right, func);
            func(iterator(subroot));
        }
    }

protected:
    static NodePtr _LeftmostPtr(NodePtr subroot)
    {
        while (subroot->_left)
            subroot = subroot->_left;

        return subroot;
    }

    static NodePtr _RightmostPtr(NodePtr subroot)
    {
        while (subroot->_right)
            subroot = subroot->_right;

        return subroot;
    }

    NodePtr _GetHead() const noexcept
    {
        if (_root->_parent)
            return _root->_parent;
        else
            return _root;
    }

    void _HeadLink()
    {
        _GetHead()->_left = _LeftmostPtr(_root);
        _GetHead()->_right = _RightmostPtr(_root);
    }

    virtual _TreeChild _ChildCondition(NodePtr child)
    {
        NodePtr parent = child->_parent;
        if (child == parent->_left)
            return _Left;
        else if (child == parent->_right)
            return _Right;
        else
            return _Unused;
    }

    virtual NodePtr& _GetChildRef(NodePtr subroot)
    {
        _TreeChild childCondition = _ChildCondition(subroot);
        NodePtr parent = subroot->_parent;
        if (childCondition == _Left)
            return parent->_left;
        else if (childCondition == _Right)
            return parent->_right;
        else
            return _root;
    }

protected:
    NodePtr _root;  //point to the head if the tree is empty
};
