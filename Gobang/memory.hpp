
#pragma once

#include <functional>
#include <stdexcept>

#include "RBTree.hpp"

template<typename MyContainer, typename Category>
struct iterator_types {
public:
	using iterator_category = Category;
	using value_type = typename MyContainer::value_type;
	using pointer = typename MyContainer::pointer;
	using reference = typename MyContainer::reference;
	using NodePtr = typename MyContainer::NodePtr;
	using self = typename MyContainer::iterator;
	using difference_type = ptrdiff_t;
	using size_type = size_t;
};

/** forward declaration */
class _ContainerBase;
class _IteratorBase;

struct ContainerProxy {
	ContainerProxy() noexcept = default;
	ContainerProxy(_ContainerBase* cont) noexcept
		:_MyCont(cont) {}

	_ContainerBase* _MyCont = nullptr;
	RBTree<_RBTree_example_traits<_IteratorBase*> > _MyIter;
};

class _ContainerBase {
public:
	_ContainerBase() noexcept
		:_MyProxy(new ContainerProxy(this)) {}

	_ContainerBase(const _ContainerBase&) = delete;
	_ContainerBase& operator=(const _ContainerBase&) = delete;

	~_ContainerBase()
	{
		_OrphanAll();

		delete _MyProxy;
	}

	inline void _OrphanAll();

	inline void _SwapAll(_ContainerBase& right);

public:
	ContainerProxy* _MyProxy;
};

class _IteratorBase {
public:
	_IteratorBase() noexcept = default;

	_IteratorBase(const _IteratorBase& _Right) noexcept
	{
		*this = _Right;
	}

	_IteratorBase& operator=(const _IteratorBase& _Right) noexcept
	{
		if (_MyProxy != _Right._MyProxy) {
			if (_Right._MyProxy) {
				_Adopt(_Right._MyProxy->_MyCont);
			}
		}

		return *this;
	}

	~_IteratorBase()
	{
		_OrphanMe();
	}

	inline _ContainerBase* _GetCont() const { return _MyProxy ? _MyProxy->_MyCont : nullptr; }

	inline void _Adopt(const _ContainerBase* parent)
	{
		if (parent) {
			ContainerProxy* parentProxy = parent->_MyProxy;
			if (_MyProxy != parentProxy) {
				_OrphanMe();

				parentProxy->_MyIter.insert(this);
				_MyProxy = parentProxy;
			}
		}
	}

	inline void _OrphanMe()
	{
		if (_MyProxy) {
			_MyProxy->_MyIter.remove(this);

			_MyProxy = nullptr;
		}
	}

public:
	ContainerProxy* _MyProxy = nullptr;
};

inline void _ContainerBase::_OrphanAll()
{
	if (_MyProxy) {
		_MyProxy->_MyIter.traverse(
			std::function<void(RBTreeNode<_IteratorBase*>*)>(
				[](RBTreeNode<_IteratorBase*>* node) {
					node->_val->_MyProxy = nullptr;
				}
				)
		);

		_MyProxy->_MyIter.clear();
	}
}