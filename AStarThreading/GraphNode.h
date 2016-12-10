#pragma once
#include <list>
#include <algorithm>

template <typename NodeType> class GraphNode {
private:
	typedef GraphNode<NodeType> Node;
	NodeType m_val;
	std::list<Node *> m_connections;
	int m_index;
	
public:
	// constructor
	GraphNode(NodeType val);
	~GraphNode();

	// accessors
	std::list<Node *> getConnections() const;
	NodeType getVal() const;
	int getIndex() const;
	void setIndex(int i);

	// public member functions
	void addConnection(Node * node);
	void removeConnection(Node * node);
	bool hasConnection(Node * node) const;
};

#pragma region Constructor

template<typename NodeType>
GraphNode<NodeType>::GraphNode(NodeType val)
	:m_val(val) {

}

template<typename NodeType>
GraphNode<NodeType>::~GraphNode() {
	delete m_val;
}

#pragma endregion

#pragma region Accessors

template<typename NodeType>
std::list<GraphNode<NodeType> *> GraphNode<NodeType>::getConnections() const {
	return m_connections;
}

template<typename NodeType>
inline NodeType GraphNode<NodeType>::getVal() const
{
	return m_val;
}

template<typename NodeType>
int GraphNode<NodeType>::getIndex() const
{
	return m_index;
}

template<typename NodeType>
void GraphNode<NodeType>::setIndex(int i)
{
	m_index = i;
}

#pragma endregion

#pragma region Public Functions

// this node will be connected to the given node
template<typename NodeType>
void GraphNode<NodeType>::addConnection(GraphNode<NodeType> * node) {
	m_connections.push_back(node);
}

// removes given node from connections if it exists, else does nothing
template<typename NodeType>
void GraphNode<NodeType>::removeConnection(GraphNode<NodeType> * node) {
	m_connections.erase(std::remove(m_connections.begin(), m_connections.end(), node), m_connections.end());
}

template<typename NodeType>
bool GraphNode<NodeType>::hasConnection(GraphNode<NodeType> * node) const {
	return std::find(m_connections.begin(), m_connections.end(), node) != m_connections.end();
}

#pragma endregion