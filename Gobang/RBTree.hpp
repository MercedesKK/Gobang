/** @author  ±ÃÏ“› */
#pragma once

#include <functional>

namespace {
	enum _Color
	{
		_Red,
		_Black
	};
}

//template<typename MyTree>
//class RBTreeIterator{
//public:
//	using iterator_category = std::bidirectional_iterator_tag;
//	using value_type        = typename MyTree::value_type;
//	using pointer           = typename MyTree::pointer;
//	using reference         = typename MyTree::reference;
//	using NodePtr           = typename MyTree::NodePtr;
//	using self              = typename MyTree::iterator;
//	using difference_type   = ptrdiff_t;
//	using size_type         = size_t;
//
//	RBTreeIterator() = delete;
//
//	RBTreeIterator(NodePtr node) :_node(node) {}
//
//protected:
//	void increment()
//	{
//		if (_node->_right) {  //has right child
//			_node = _node->_right;
//			_node = MyTree::_LeftmostPtr(_node);
//
//			return;
//		}
//		else {
//			if (_node == _node->_parent->_right && _node->_parent->_parent == nullptr) {  //iterator on the root with no right child
//				_node = _node->_parent;
//				return;
//			}
//
//			while (_node == _node->_parent->_right) {
//				_node = _node->_parent;
//			}
//			_node = _node->_parent;
//
//			return;
//		}
//	}
//
//	void decrement()
//	{
//		if (_node->_left) {  //has left child
//			_node = _node->_left;
//			_node = MyTree::_RightmostPtr(_node);
//
//			return;
//		}
//		else {
//			while (_node == _node->_parent->_left)
//				_node = _node->_parent;
//			_node = _node->_parent;
//
//			return;
//		}
//	}
//
//public:
//	reference operator *() const { return _node->_val; }
//	pointer operator->()   const { return &(operator*()); }
//	self& operator++()
//	{
//		increment();
//		return *this;
//	}
//	self operator++(int)
//	{
//		self temp = *this;
//		operator++();
//		return temp;
//	}
//	self& operator--()
//	{
//		if (_node->_parent == nullptr) {  //iterator on the head
//			_node = _node->_right;
//			return _node;
//		}
//
//		decrement();
//		return *this;
//	}
//	self operator--(int)
//	{
//		self temp = *this;
//		operator--();
//		return temp;
//	}
//	bool operator==(const self& rhs) { return this->_node == rhs._node; }
//	bool operator!=(const self& rhs) { return this->_node != rhs._node; }
//
//protected:
//	NodePtr _node;
//};

template<typename Entry>
struct RBTreeNode{
	using NodePtr = RBTreeNode<Entry>*;
	using NodeRef = RBTreeNode<Entry>&;
	using value_type = Entry;

	RBTreeNode()
		:_parent(nullptr), _left(nullptr), _right(nullptr),_color(_Red) {}

	RBTreeNode(NodePtr parent, NodePtr left, NodePtr right)
		:_parent(parent), _left(left), _right(right), _color(_Red) {}

	RBTreeNode(value_type val)
		:_parent(nullptr), _left(nullptr), _right(nullptr), _val(val), _color(_Red) {}

	RBTreeNode(_Color color)
		:_parent(nullptr), _left(nullptr), _right(nullptr), _color(color) {}

	_Color _color;
	NodePtr _parent;
	NodePtr _left;
	NodePtr _right;
	value_type _val;
};

template<typename val,
	typename key = val,
	typename val_comp = std::less<val>,
	typename key_comp = val_comp >
	class _RBTree_example_traits {
	public:
		using value_type = val;
		using key_type = key;
		using value_compare = val_comp;
		using key_compare = key_comp;

		static key_type KeyOfVal(value_type _val) { return _val; }
};

template<typename MyTraits>
class RBTree{
public:
	using value_type      = typename MyTraits::value_type;
	using key_type        = typename MyTraits::key_type;
	using value_compare   = typename MyTraits::value_compare;
	using key_compare     = typename MyTraits::key_compare;
	using difference_type = ptrdiff_t;
	using size_type       = size_t;

public:
	using Node    = RBTreeNode<value_type>;
	using NodePtr = RBTreeNode<value_type>*;
	using self    = RBTree<MyTraits>;

public:
	using pointer   = value_type*;
	using reference = value_type&;
	//using iterator  = RBTreeIterator<self>;

	//friend class iterator;

public:
	RBTree()
		:_NIL(new Node(_Black))
	{
		_NIL->_parent = _NIL;
		_NIL->_left = _NIL;
		_NIL->_right = _NIL;
		_root = _NIL;
	}

	RBTree(const RBTree& rhs)
		:_NIL(new Node(_Black))
	{
		_NIL->_parent = _NIL;
		_NIL->_left = _NIL;
		_NIL->_right = _NIL;
		_root = _NIL;
		_Copy(rhs);
	}

	RBTree(RBTree&& rhs)
		:_root(rhs._root),
		_NIL(rhs._NIL) {}

	~RBTree()
	{
		_Destroy();
	}

protected:
	void _Copy(const RBTree& rhs)
	{
		copy_aux(rhs.root(), rhs.NIL());
	}

	void copy_aux(const NodePtr subroot,const NodePtr nil)
	{
		if (subroot != nil) {
			this->insert(subroot->_val);
			copy_aux(subroot->_left, nil);
			copy_aux(subroot->_right, nil);
		}
	}

	void _DestroyNode(NodePtr node)
	{
		delete node;
	}

	void _DestroyTree(NodePtr subroot)
	{
		if (subroot != _NIL) {
			_DestroyTree(subroot->_left);
			_DestroyTree(subroot->_right);
			_DestroyNode(subroot);
		}
	}

	void _Destroy()
	{
		if (_root!=_NIL)
			_DestroyTree(_root);

		_DestroyNode(_NIL);
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
	//iterator begin() const noexcept { return _LeftmostPtr(_root); }
	//iterator end()   const noexcept { return _NIL; }
	//iterator getRoot() const noexcept { return _head->_parent; }
	bool empty()        const noexcept { return _root == _NIL; }
	NodePtr firstPtr()  const noexcept { return _LeftmostPtr(_root); }
	NodePtr lastPtr()   const noexcept { return _RightmostPtr(_root); }
	NodePtr root()      const noexcept { return _root; }
	const NodePtr NIL() const noexcept { return _NIL; }
	void clear() noexcept
	{
		_DestroyTree(_root);
		_root = _NIL;
	}
	int size() const noexcept
	{
		if (empty())
			return 0;

		return size_aux(_root);
	}

private:
	bool _RedCondition(NodePtr subroot)  //check the red condition of the highest two levels of subtree
	{
		if (subroot->_color == _Red)
			if (subroot->_left->_color == _Red || subroot->_right->_color == _Red)
				return false;

		return true;
	}

	void _RotateRight(NodePtr subroot)  //root isn't specially handled
	{
		NodePtr subrootParent = subroot->_parent;
		NodePtr subrootRight = subroot->_right;

		_Transplant(subroot->_parent, subroot);

		subroot->_right = subrootParent;
		subrootParent->_parent = subroot;

		subrootParent->_left = subrootRight;
		if (subrootRight != _NIL)
			subrootRight->_parent = subrootParent;
	}

	void _RotateLeft(NodePtr subroot)  //root isn't specially handled
	{
		NodePtr subrootParent = subroot->_parent;
		NodePtr subrootLeft = subroot->_left;

		_Transplant(subroot->_parent, subroot);

		subroot->_left = subrootParent;
		subrootParent->_parent = subroot;

		subrootParent->_right = subrootLeft;
		if (subrootLeft != _NIL)
			subrootLeft->_parent = subrootParent;
	}

	void RB_RotateRight(NodePtr subroot)
	{
		_Color temp = subroot->_color;
		subroot->_color = subroot->_parent->_color;
		subroot->_parent->_color = temp;

		_RotateRight(subroot);
	}

	void RB_RotateLeft(NodePtr subroot)
	{
		_Color temp = subroot->_color;
		subroot->_color = subroot->_parent->_color;
		subroot->_parent->_color = temp;

		_RotateLeft(subroot);
	}

	int size_aux(NodePtr subroot) const noexcept
	{
		if (subroot == _NIL)
			return 0;

		return 1 + size_aux(subroot->_left) + size_aux(subroot->_right);
	}

	NodePtr find_aux(NodePtr subroot,value_type val)
	{
		if (subroot == _NIL)
			return _NIL;
		if (val == subroot->_val)
			return subroot;

		if (value_comp()(val, subroot->_val))
			return find_aux(subroot->_left, val);
		else
			return find_aux(subroot->_right, val);
	}

public:
	void insert(value_type val)
	{
		if (_root == _NIL) {  //empty tree
			_root = new Node(val);
			_root->_color = _Black;
			_root->_parent = _NIL;
			_root->_left = _NIL;
			_root->_right = _NIL;

			return;
		}

		auto newNode = unchecked_insert(_root, val);
		if (!_RedCondition(newNode->_parent))
			_InsertAdjustion(newNode->_parent->_parent);
	}

protected:
	NodePtr unchecked_insert(NodePtr subroot, value_type val)
	{
		//recursive insertion
		if (value_comp()(val, subroot->_val)) {
			if (subroot->_left != _NIL) {
				return unchecked_insert(subroot->_left, val);
			}
			else {
				NodePtr newNode = new Node(val);
				newNode->_parent = subroot;
				subroot->_left = newNode;
				newNode->_left = _NIL;
				newNode->_right = _NIL;

				return newNode;
			}
		}
		else {
			if (subroot->_right != _NIL) {
				return unchecked_insert(subroot->_right, val);
			}
			else {
				NodePtr newNode = new Node(val);
				newNode->_parent = subroot;
				subroot->_right = newNode;
				newNode->_left = _NIL;
				newNode->_right = _NIL;

				return newNode;
			}
		}
	}

	void _InsertAdjustion(NodePtr subroot)  //perform upward adjustment
	{
		if (subroot->_left->_color == _Red && subroot->_right->_color == _Red) {
			_ColorFlip(subroot);
			if (subroot->_parent == _NIL) {
				subroot->_color = _Black;
				return;
			}
			if (!_RedCondition(subroot->_parent))
				return _InsertAdjustion(subroot->_parent->_parent);
		}
		else if (subroot->_left->_color == _Red) {
			NodePtr parent = subroot->_left;
			if (parent->_left->_color == _Red) {
				RB_RotateRight(parent);
				return;
			}
			else if (parent->_right->_color == _Red) {
				NodePtr child = parent->_right;
				RB_RotateLeft(child);
				RB_RotateRight(child);
				return;
			}
		}
		else if (subroot->_right->_color == _Red) {
			NodePtr parent = subroot->_right;
			if (parent->_right->_color == _Red) {
				RB_RotateLeft(parent);
				return;
			}
			else if (parent->_left->_color == _Red) {
				NodePtr child = parent->_left;
				RB_RotateRight(child);
				RB_RotateLeft(child);
				return;
			}
		}
	}

	void _ColorFlip(NodePtr subroot)
	{
		subroot->_color = _Red;
		subroot->_left->_color = _Black;
		subroot->_right->_color = _Black;
	}

public:
	void remove(NodePtr node)
	{
		NodePtr removed = node;
		NodePtr replaced = removed;
		NodePtr checked = _NIL;
		_Color replacedColor = replaced->_color;

		if (removed->_left == _NIL) {
			checked = removed->_right;
			_Transplant(removed, removed->_right);
		}
		else if (removed->_right == _NIL) {
			checked = removed->_left;
			_Transplant(removed, removed->_left);
		}
		else {
			replaced = _RightmostPtr(removed->_left);
			replacedColor = replaced->_color;
			checked = replaced->_left;
			if (replaced->_parent == removed) {
				checked->_parent = replaced;
			}
			else {
				_Transplant(replaced, replaced->_left);
				replaced->_left = removed->_left;
				replaced->_left->_parent = replaced;
			}
			_Transplant(removed, replaced);
			replaced->_right = removed->_right;
			replaced->_right->_parent = replaced;
			replaced->_color = removed->_color;
		}

		delete removed;
		if (replacedColor == _Black)
			_RemoveAdjustion(checked);
	}

	void remove(value_type val)
	{
		remove(find_aux(_root, val));
	}

private:
	void _RemoveAdjustion(NodePtr checked)
	{
		while (checked != _root && checked->_color == _Black) {  //fix the black condition for a non-root doubly black node
			if (checked == checked->_parent->_left) {
				NodePtr sibling = checked->_parent->_right;
				//red siblng
				if (sibling->_color == _Red) {
					RB_RotateLeft(sibling);
					sibling = checked->_parent->_right;
				}

				//black sibling
				if (sibling->_left->_color == _Black && sibling->_right->_color == _Black) {  //black children
					sibling->_color = _Red;
					checked = checked->_parent;
					continue;
				}
				else if (sibling->_left->_color == _Red && sibling->_right->_color == _Black) {  //red left child
					RB_RotateRight(sibling->_left);
					sibling = checked->_parent->_right;
				}
				sibling->_right->_color = _Black;  //red right child
				RB_RotateLeft(sibling);
				checked = _root;
			}
			else {
				NodePtr sibling = checked->_parent->_left;
				//red sibling
				if (sibling->_color == _Red) {
					RB_RotateRight(sibling);
					sibling = checked->_parent->_left;
				}

				//black sibling
				if (sibling->_left->_color == _Black && sibling->_right->_color == _Black) {  //black children
					sibling->_color = _Red;
					checked = checked->_parent;
					continue;
				}
				else if (sibling->_left->_color == _Black && sibling->_right->_color == _Red) {  //red right child
					RB_RotateLeft(sibling->_right);
					sibling = checked->_parent->_left;
				}
				sibling->_left->_color = _Black;  //red left child
				RB_RotateRight(sibling);
				checked = _root;
			}
		}

		checked->_color = _Black;  //make sure restore the root and red condition
	}

public:
	/** @brief use preorder traversal to handle raw pointer */
	void traverse(std::function<void(const NodePtr)> func)
	{
		if (empty())  //empty tree
			return;

		traverse_aux(_root, func);
	}

private:
	void traverse_aux(NodePtr subroot, std::function<void(const NodePtr)> func)
	{
		if (subroot == _NIL) {
			return;
		}
		else {
			func(subroot);
			traverse_aux(subroot->_left, func);
			traverse_aux(subroot->_right, func);
		}
	}

protected:
	NodePtr _LeftmostPtr(NodePtr subroot) const noexcept
	{
		while (subroot->_left != _NIL)
			subroot = subroot->_left;

		return subroot;
	}

	NodePtr _RightmostPtr(NodePtr subroot) const noexcept
	{
		while (subroot->_right != _NIL)
			subroot = subroot->_right;

		return subroot;
	}

	/*_TreeChild _ChildCondition(NodePtr child)
	{
		NodePtr parent = child->_parent;
		if (child == parent->_left)
			return _Left;
		else if (child == parent->_right)
			return _Right;

		return _Unused;
	}*/

	void _Transplant(NodePtr original,NodePtr newchild)
	{
		NodePtr parent = original->_parent;

		original->_parent = _NIL;  //the situation that newchild is root isnt considered
		if (newchild->_parent != _NIL) {
			if (newchild == newchild->_parent->_left)
				newchild->_parent->_left = _NIL;
			else if (newchild == newchild->_parent->_right)
				newchild->_parent->_right = _NIL;
		}

		if (parent == _NIL)  //original is root
			_root = newchild;
		else if (original == parent->_left)
			parent->_left = newchild;
		else
			parent->_right = newchild;
		newchild->_parent = parent;
	}

protected:
	NodePtr _root;
	const NodePtr _NIL;
};