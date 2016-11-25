#pragma once
#include <queue>
template<
	class T,
	class Container = std::vector<T>,
	class Compare = std::less<typename Container::value_type>
> class my_priority_queue : public std::priority_queue<T, Container, Compare>
{
public:
	typedef typename
		std::priority_queue<
		T,
		Container,
		Compare>::container_type::const_iterator const_iterator;

	const_iterator begin() const {
		return c.begin();
	}

	const_iterator end() const {
		return c.end();
	}
};