/** @author  ±ÃÏ“› */
#pragma once

#include <vector>
#include "BinaryTree.hpp"
#include "memory.hpp"

template<typename val>
class _Multiway_tree_traits {
public:
	using Val = val;
	using key_type = int;
	using value_type = std::pair<key_type, val>;
	using key_compare = std::equal_to<key_type>;

	class value_compare {
	public:
		value_compare(key_compare _comp) :comp(_comp) {}

		bool operator()(value_type Left, value_type Right)
		{
			return comp(Left.first, Right.first);
		}

		key_compare comp;
	};

	static const key_type KeyOfVal(const value_type& val) { return val.first; }
	static Val& GetVal(value_type& val) { return val.second; }
};

template<typename MyTree>
class MultiwayTreeIterator :public iterator_types<MyTree, std::bidirectional_iterator_tag>, public _IteratorBase {
	friend MyTree;

public:
	using iterator_types = iterator_types<MyTree, std::bidirectional_iterator_tag>;
	using iterator_category = iterator_types::iterator_category;
	using value_type = iterator_types::value_type;
	using pointer = iterator_types::pointer;
	using reference = iterator_types::reference;
	using NodePtr = iterator_types::NodePtr;
	using self = iterator_types::self;
	using difference_type = iterator_types::difference_type;
	using size_type = iterator_types::size_type;
	using Val = typename MyTree::Val;

	MultiwayTreeIterator() = delete;

	MultiwayTreeIterator(NodePtr node, const MyTree* tree)
		:_node(node)
	{
		this->_Adopt(tree);
	}

	void addChild(Val val)
	{
		static_cast<MyTree*>(this->_GetCont())->addChild(_node, val);
	}

	void remove()
	{
		static_cast<MyTree*>(this->_GetCont())->remove(_node);
	}

	self getParent()
	{
		NodePtr parent = _node->_parent;
		NodePtr child = _node;
		while (parent) {
			if (parent->_right == child)
				return self(parent, static_cast<MyTree*>(this->_GetCont()));
			else {
				child = parent;
				parent = parent->_parent;
			}
		}

		return self(nullptr, static_cast<MyTree*>(this->_GetCont()));
	}

	std::vector<self> getChildren()
	{
		std::vector<self> children;
		NodePtr child = _node->_right;
		while (child) {
			children.push_back(self(child, static_cast<MyTree*>(this->_GetCont())));
			child = child->_left;
		}

		return children;
	}

	reference operator *() const { return _Multiway_tree_traits<Val>::GetVal(_node->_val); }
	pointer operator->()   const { return &(operator*()); }
	bool operator==(const self& rhs) { return this->_node == rhs._node; }
	bool operator!=(const self& rhs) { return this->_node != rhs._node; }

private:
	NodePtr _node;
};

template<typename MyTraits>
class MultiwayTree :private BinaryTree<MyTraits>, public _ContainerBase {  //right child, left sibling
public:
	using MyBase = BinaryTree<MyTraits>;
	using Val = typename MyTraits::Val;
	using value_type = MyBase::value_type;
	using key_type = MyBase::key_type;
	using Node = MyBase::Node;
	using NodePtr = MyBase::NodePtr;
	using difference_type = MyBase::difference_type;
	using size_type = MyBase::size_type;
	using pointer = Val*;
	using reference = Val&;
	using self = MultiwayTree<MyTraits>;
	using iterator = MultiwayTreeIterator<self>;

	friend iterator;

public:
	MultiwayTree() = default;

protected:
	using MyBase::_GetHead;
	using MyBase::_HeadLink;
	using MyBase::_Transplant;
	using MyBase::_DestroyTree;

public:
	using MyBase::empty;
	iterator end()     const noexcept { return iterator(nullptr, this); }
	iterator getRoot() const noexcept
	{
		if (empty())
			return end();

		return iterator(_root, this);
	}

	void setRoot(Val val)
	{
		if (!empty())
			return;

		MyBase::insert(value_type(0, val));
	}

	void resetRoot(iterator newRoot)
	{
		NodePtr newroot = newRoot._node;
		if (find(*newRoot) == end())
			return;
		if (newroot == _root)
			return;

		NodePtr originalRoot = _root;
		_Transplant(_root, newroot);
		_DestroyTree(originalRoot);
	}

public:
	iterator find(Val val) const noexcept
	{
		if (empty())
			return iterator(nullptr, this);

		return iterator(find_aux(_root, val), this);
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
	void addChild(NodePtr subroot, Val val)
	{
		value_type toInsert;
		toInsert.first = MyTraits::KeyOfVal(subroot->_val) + 1;
		toInsert.second = val;
		MyBase::unchecked_insert(subroot, value_type(MyTraits::KeyOfVal(subroot->_val) + 1, val));
	}

	void remove(NodePtr toDel)
	{
		if (toDel == nullptr)
			return;
		else if (toDel == _root && toDel->_parent != nullptr) {  //root of tree
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