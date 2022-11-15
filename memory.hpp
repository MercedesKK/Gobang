
#pragma once

#include <functional>
#include <stdexcept>

template<typename MyContainer,typename Category>
struct iterator_types {
public:
	using iterator_category = Category;
	using value_type        = typename MyContainer::value_type;
	using pointer           = typename MyContainer::pointer;
	using reference         = typename MyContainer::reference;
	using NodePtr           = typename MyContainer::NodePtr;
	using self              = typename MyContainer::iterator;
	using difference_type   = ptrdiff_t;
	using size_type         = size_t;
};