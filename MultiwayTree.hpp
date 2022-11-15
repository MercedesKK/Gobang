/** @author  ±ÃÏ“› */
#pragma once

#include <vector>
#include "BinaryTree.hpp"

template<typename MyTree>
class MultiwayTreeIterator{
public:
    using MyBase = BinaryTreeIterator<MyTree>;
	


private:

};

template<typename _val>
class _Multiway_tree_traits{
public:
    using Val           = _val;
    using key_type      = int;
    using value_type    = std::pair<key_type, Val>;
    using key_compare   = std::equal_to<key_type>;

    class value_compare {
    public:
        value_compare(key_compare _comp) :comp(_comp) {}

        bool operator()(value_type Left, value_type Right)
        {
            return comp(Left.first, Right.first);
        }

        key_compare comp;
    };

    static key_type KeyOfVal(value_type val) { return val.first; }
    static Val GetVal(value_type val) { return val.second; }
};

template<typename MyTraits>
class MultiwayTree :private BinaryTree<MyTraits> {
public:
    using MyBase            = BinaryTree<MyTraits>;
    using Val               = typename MyTraits::Val;
    using value_type        = typename MyBase::value_type;
    using key_type          = typename MyBase::key_type;
    using  Node             = typename MyBase::Node;
    using  NodePtr          = typename MyBase::NodePtr;
    using  difference_type  = typename MyBase::difference_type;
    using  size_type        = typename MyBase::size_type;
    using  pointer          = typename MyBase::pointer;
    using  reference        = typename MyBase::reference;

public:
    MultiwayTree() = default;

public:
    using MyBase::empty;
    NodePtr getRoot() const noexcept { return _root; }

public:
    NodePtr find(Val val) const noexcept
    {
        if (empty())
            return nullptr;

        return find_aux(_root, val);
    }

private:
    NodePtr find_aux(NodePtr subroot, Val val) const noexcept
    {
        if (subroot == nullptr)
            return nullptr;

        Val subrootVal = MyTraits::GetVal(subroot->_val);
        if (subrootVal == val) {
            return subroot;
        }
        else {
            NodePtr leftResult = find_aux(subroot->_left, val);
            NodePtr rightResult = find_aux(subroot->_right, val);

            if (leftResult)
                return leftResult;
            else if (rightResult)
                return rightResult;
            else
                return nullptr;
        }
    }

public:
    void setRoot(Val val)
    {
        MyBase::insert(value_type(0, val));
    }

    void addChild(NodePtr subroot, Val val)
    {
        value_type toInsert;
        toInsert.first = MyTraits::KeyOfVal(subroot->_val) + 1;
        toInsert.second = val;
        MyBase::unchecked_insert(subroot, value_type(MyTraits::KeyOfVal(subroot->_val) + 1, val));
    }

    void remove(Val val)
    {
        NodePtr toDel = find(val);
        if (toDel == nullptr)
            return;
        else if (toDel == _root) {
            _root = _root->_parent;
            _root->_left = _root;
            _root->_right = _root;
        }
        else {
            NodePtr& deletedPos = MyBase::_GetChildRef(toDel);
            deletedPos = toDel->_left;
            if (toDel->_left)
                toDel->_left->_parent = toDel->_parent;
            toDel->_left = nullptr;
        }
        MyBase::_DestroyTree(toDel);
        return;
    }

//public:
//	void traverse(std::function<void(const NodePtr)> func)
//	{
//		if (empty())
//			return;
//
//		traverse_aux(MyBase::_root, func);
//	}
//
//private:
//	void traverse_aux(NodePtr subroot, std::function<void(const NodePtr)> func)
//	{
//		if (subroot == nullptr) {
//			return;
//		}
//		else {
//			func(subroot);
//			traverse_aux(subroot->_left);
//			traverse_aux(subroot->_right);
//		}
//	}

public:
    std::vector<Val> getChildren(NodePtr parent)
    {
        std::vector<Val> children;
        NodePtr child = parent->_right;
        while (child) {
            children.push_back(child->_val.second);
            child = child->_left;
        }

        return children;
    }

    void resetNodeVal(NodePtr node, Val val) {
        node->_val.second = val;
    }

protected:
    using MyBase::_root;
};
