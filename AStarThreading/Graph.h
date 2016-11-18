#pragma once
#include "GraphNode.h"
#include <iostream>
#include <vector>
#include <queue>

template<typename NodeType> class Graph {
private:
	// typedef for simplicity
	typedef GraphNode<NodeType> Node;

	// function to evaluate a* heuristic
	float (*m_heuristicFunc) (NodeType, NodeType);
	
	// array of all nodes in the graph
	std::vector<Node *> m_nodes;
	
	// maximum number of nodes in the graph
	int m_maxNodes;

	// the actual number of nodes in the graph
	int m_count;

	// aStar comparison functor


public:
	// constructor / destructor
	Graph(int size, float (*heuristicFunc) (NodeType, NodeType));
	~Graph();

	// accessors
	std::vector<Node *> getNodes() const;
	int getCount() const;
	Node * getNode(int index) const;

	// public member functions
	bool addNode(NodeType val, int index);
	void removeNode(int index);
	bool addArc(int from, int to);
	void removeArc(int from, int to);
	bool connectionExists(int from, int to) const;
	void aStar(int from, int to, std::vector<NodeType> * path);
};

#pragma region Constructors / Destructor

// constructor
template<typename NodeType>
Graph<NodeType>::Graph(int size, float (*heuristicFunc) (NodeType, NodeType))
	:m_maxNodes(size),
	 m_heuristicFunc(heuristicFunc),
	 m_count(0) {
	// create all nodes and clear it to nullptr
	m_nodes = std::vector<Node *>(m_maxNodes);
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
std::vector<GraphNode<NodeType> *> Graph<NodeType>::getNodes() const {
	return m_nodes;
}

// get the count
template<typename NodeType>
int Graph<NodeType>::getCount() const {
	return m_count;
}

template<typename NodeType>
GraphNode<NodeType> * Graph<NodeType>::getNode(int index) const
{
	if (index >= 0 && index < m_maxNodes) {
		return m_nodes[index];
	}
}

#pragma endregion

#pragma region Public Functions

// adds a node to the graph at given index,
// returns true if successful, else false
template<typename NodeType>
bool Graph<NodeType>::addNode(NodeType val, int index) {
	if (index >= 0 && index < m_maxNodes) {
		if (m_nodes[index] == nullptr) {
			m_nodes[index] = new Node(val);
			m_count++;
			return true;
		}
	}
	return false;
}

// removes node from the graph and remove all connections
template<typename NodeType>
void Graph<NodeType>::removeNode(int index) {
	if (index >= 0 && index < m_maxNodes) {
		if (m_nodes[index] != nullptr) {
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
}

// adds a connection between two in nodes if possible
// returns true if added, else false
template<typename NodeType>
bool Graph<NodeType>::addArc(int from, int to) {
	if (from >= 0 && to >= 0 && from < m_maxNodes && to < m_maxNodes) {
		if (m_nodes[from] != nullptr && m_nodes[to] != nullptr) {
			if (!m_nodes[from]->hasConnection(m_nodes[to])) {
				m_nodes[from]->addConnection(m_nodes[to]);
				return true;
			}
		}
	}
	return false;
}

// removes a connection between nodes if possible
template<typename NodeType>
void Graph<NodeType>::removeArc(int from, int to) {
	if (from >= 0 && to >= 0 && from < m_maxNodes && to < m_maxNodes) {
		if (m_nodes[from] != nullptr && m_nodes[to] != nullptr) {
			m_nodes[from]->removeConnection(m_nodes[to]);
		}
	}
}

template<typename NodeType>
bool Graph<NodeType>::connectionExists(int from, int to) const {
	if (from >= 0 && to >= 0 && from < m_maxNodes && to < m_maxNodes) {
		if (m_nodes[from] != nullptr && m_nodes[to] != nullptr) {
			return m_nodes[from]->hasConnection(m_nodes[to]);
		}
	}
	return false;
}

template<typename NodeType>
void Graph<NodeType>::aStar(int from, int to, std::vector<NodeType>* path) {
	/*function A*(start, goal)
    // The set of nodes already evaluated.
    closedSet := {}
    // The set of currently discovered nodes still to be evaluated.
    // Initially, only the start node is known.
    openSet := {start}
    // For each node, which node it can most efficiently be reached from.
    // If a node can be reached from many nodes, cameFrom will eventually contain the
    // most efficient previous step.
    cameFrom := the empty map

    // For each node, the cost of getting from the start node to that node.
    gScore := map with default value of Infinity
    // The cost of going from start to start is zero.
    gScore[start] := 0 
    // For each node, the total cost of getting from the start node to the goal
    // by passing by that node. That value is partly known, partly heuristic.
    fScore := map with default value of Infinity
    // For the first node, that value is completely heuristic.
    fScore[start] := heuristic_cost_estimate(start, goal)

    while openSet is not empty
        current := the node in openSet having the lowest fScore[] value
        if current = goal
            return reconstruct_path(cameFrom, current)

        openSet.Remove(current)
        closedSet.Add(current)
        for each neighbor of current
            if neighbor in closedSet
                continue		// Ignore the neighbor which is already evaluated.
            // The distance from start to a neighbor
            tentative_gScore := gScore[current] + dist_between(current, neighbor)
            if neighbor not in openSet	// Discover a new node
                openSet.Add(neighbor)
            else if tentative_gScore >= gScore[neighbor]
                continue		// This is not a better path.

            // This path is the best until now. Record it!
            cameFrom[neighbor] := current
            gScore[neighbor] := tentative_gScore
            fScore[neighbor] := gScore[neighbor] + heuristic_cost_estimate(neighbor, goal)

    return failure

function reconstruct_path(cameFrom, current)
    total_path := [current]
    while current in cameFrom.Keys:
        current := cameFrom[current]
        total_path.append(current)
    return total_path*/
	typedef GraphNode<NodeType> Node;
	if (m_nodes[from] != nullptr && m_nodes[to] != nullptr) {
		std::vector<Node *> closedList;
		//std::priority_queue<<Node *>, std::vector<Node *>, > openList;
	}
}

#pragma endregion

