#pragma once
#include "GraphNode.h"
#include <iostream>

template<typename NodeType> class Graph {
private:
	// typedef for simplicity
	typedef GraphNode<NodeType> Node;

	// function to evaluate a* heuristic
	float (*m_heuristicFunc) (NodeType, NodeType);
	
	// array of all nodes in the graph
	Node ** m_nodes;
	
	// maximum number of nodes in the graph
	int m_maxNodes;

	// the actual number of nodes in the graph
	int m_count;
public:
	// constructor / destructor
	Graph(int size, float (*heuristicFunc) (NodeType, NodeType));
	~Graph();

	// accessors
	Node ** getNodes() const;
	int getCount() const;

	// public member functions
	bool addNode(NodeType val, int index);
	void removeNode(int index);
	bool addArc(int from, int to);
	void removeArc(int from, int to);
};

#pragma region Constructors / Destructor

// constructor
template<typename NodeType>
Graph<NodeType>::Graph(int size, float (*heuristicFunc) (NodeType, NodeType))
	:m_maxNodes(size),
	 m_heuristicFunc(heuristicFunc),
	 m_count(0) {
	// create all nodes and clear it to nullptr
	m_nodes = new Node * [m_maxNodes];
	for (int i = 0; i < m_maxNodes; i++) {
		m_nodes[i] = nullptr;
	}
}

// destructor
template<typename NodeType>
Graph<NodeType>::~Graph() {
	for (int i = 0; i < m_maxNodes; i++) {
		if (m_nodes[i] != nullptr) {
			delete m_nodes[i];
		}
	}
	delete m_nodes;
}

#pragma endregion

#pragma region Accessors

// get the node underlying node array
template<typename NodeType>
GraphNode<NodeType> ** Graph<NodeType>::getNodes() const {
	return m_nodes;
}

// get the count
template<typename NodeType>
int Graph<NodeType>::getCount() const {
	return m_count;
}

#pragma endregion

#pragma region Public Functions

// adds a node to the graph at given index,
// returns true if successful, else false
template<typename NodeType>
bool Graph<NodeType>::addNode(NodeType val, int index) {
	if (m_nodes[index] == nullptr) {
		m_nodes[index] = new Node(val);
		m_count++;
		return true;
	}
	return false;
}

// removes node from the graph and remove all connections
template<typename NodeType>
void Graph<NodeType>::removeNode(int index) {
	if (m_pNodes[index] != nullptr) {
		// remove all connections to node at given index
		for (int i = 0; i < m_maxNodes; i++) {
			if (m_nodes[i] != nullptr) {
				m_nodes[i]->removeConnection(m_nodes[index])
			}
		}
		// delete node from list
		delete m_nodes[index];
		m_nodes[index] = nullptr;
		count--;
	}
}

// adds a connection between two in nodes if possible
// returns true if added, else false
template<typename NodeType>
bool Graph<NodeType>::addArc(int from, int to)
{
	if (m_nodes[from] != nullptr && m_nodes[to] != nullptr) {
		if (!m_nodes[from]->hasConnection(m_nodes[to])) {
			m_nodes[from]->addConnection(m_nodes[to]);
			return true;
		}
	}
	return false;
}

// removes a connection between nodes if possible
template<typename NodeType>
void Graph<NodeType>::removeArc(int from, int to)
{
	if (m_nodes[from] != nullptr && m_nodes[to] != nullptr) {
		m_nodes[from]->removeConnection(m_nodes[to]);
	}
}

#pragma endregion

