#pragma once
#include <list>

template <typename NodeType> class GraphNode {
private:
	typedef GraphNode<NodeType> Node;
	NodeType m_val;
	List<Node> m_connections;
public:
	GraphNode(NodeType val) {
		m_val = val;
	}
};