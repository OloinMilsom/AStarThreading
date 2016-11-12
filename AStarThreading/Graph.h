#pragma once
#include "GraphNode.h"
#include <iostream>

template<typename NodeType> class Graph {
private:
	// typedef for simplicity
	typedef GraphNode<NodeType> Node;

	// function to evaluate a* heuristic
	float (*m_heuristicFunc) (NodeType *, NodeType *);
	
	// array of all nodes in the graph
	Node ** m_nodes;
	
	// maximum number of nodes in the graph
	int m_maxNodes;

	// the actual number of nodes in the graph
	int m_count;
public:
	// constructor / destructor
	Graph(int size, float (*heuristicFunc) (NodeType *, NodeType *));
	~Graph();

	// accessors
	Node ** getNodes() const;

	// public member functions
	bool addNode(NodeType val, int index);
	void removeNode(int index);
	bool addArc(int from, int to);
	void removeArc(int index);
};

#pragma region Constructors / Destructor

// constructor
template<typename NodeType>
Graph<NodeType>::Graph(int size, float (*heuristicFunc) (NodeType *, NodeType *))
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
Node ** Graph<NodeType>::getNodes() const {
	return m_nodes
}

#pragma endregion

#pragma region Public Functions

// adds a node to the graph at given index,
// returns true if successful, else false
template<typename NodeType>
bool Graph<NodeType>::addNode(NodeType val, int index) {
	if (m_pNodes[index] == nullptr) {
		m_pNodes[index] = new Node(val);
		cout++;
		return true;
	}
	return false;
}

// removes node from the graph and remove all connections
template<typename NodeType>
void Graph<NodeType>::addNode(int index) {
	if (m_pNodes[index] != nullptr) {

	}
}

#pragma endregion

