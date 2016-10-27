#pragma once

template <typename T> class Node {
private:
	T m_val;
public:
	Node(T val) {
		m_val = val;
	}
};