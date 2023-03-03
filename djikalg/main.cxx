#include "djikstras-alg.h"
#include <iostream>

//CONNECTION
Connection::Connection(const std::uint64_t i, const std::uint32_t c) : index{ i }, cost{ c } {}

//NODE
Node::Node(const std::string& n) : name{ n }, eNodeState{ NodeState::UNVISITED }, associatedCost{ 0 }, priorNode{ 0 } {}

bool Node::CreateConnection(const std::uint64_t& i, std::uint32_t c) {
	this->connections.push_back(Connection(i, c));

	return true;
}

//GRAPH
Graph::Graph(const std::string& n) : name{ n }, optimalRoute{ "" }, optimalRouteCost{ 0 } {}

Graph::ErrState Graph::CreateNode(const std::string& newNode) {
	std::vector<Node>::iterator it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&](Node const& n) { return n.name == newNode; }
	);

	if (it != this->nodes.end()) return ErrState::attemptToCreateExistingNode;

	this->nodes.push_back(Node(newNode));

	return ErrState::success;
}

Graph::ErrState Graph::CoutListNodes() {
	std::cout << "Graph: " << this->name << ". Contains " << this->nodes.size() << " nodes.\n\n";
	for (Node n : this->nodes) {
		std::cout << "Node " << n.name << ":\n";
		for (Connection c : n.connections) {
			std::cout << "\tConnected to " << this->nodes[c.index].name << " with cost " << c.cost << ".\n";
		}
		std::cout << "\n";
	}

	return ErrState::success;
}

Graph::ErrState Graph::ConnectNodes(const std::string& node1, const std::string& node2, const std::uint32_t forwardCost, const std::uint32_t backwardCost) {
	std::uint64_t index1, index2;

	// Get indexes
	std::vector<Node>::iterator it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&](Node const& n) { return n.name == node1; }
	);

	index1 = it - this->nodes.begin();
	if (it == this->nodes.end()) {
		Graph::CreateNode(node1);
	}

	it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&](Node const& n) { return n.name == node2; }
	);

	index2 = it - this->nodes.begin();
	if (it == this->nodes.end()) {
		Graph::CreateNode(node2);
	}

	// Pass to index function
	if (this->nodes[index1].CreateConnection(index2, forwardCost) && this->nodes[index2].CreateConnection(index1, backwardCost))
		return ErrState::success;
	else
		return ErrState::unkCNErr;
}

Graph::ErrState Graph::ConnectNodes(const std::string& node1, const std::string& node2, const std::uint32_t edgeCost) {
	return Graph::ConnectNodes(node1, node2, edgeCost, edgeCost);
}

std::string Graph::GetOptimalRoute() {
	return this->optimalRoute;
}

Graph::ErrState Graph::RunDjikstrasAlgorithm(const std::string& start, const std::string& end) {
	std::uint64_t indexS, indexE;

	// Get indexes
	std::vector<Node>::iterator it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&](Node const& n) { return n.name == start; }
	);

	indexS = it - this->nodes.begin();
	if (it == this->nodes.end()) {
		return ErrState::nodeNotFound;
	}

	it = std::find_if(this->nodes.begin(), this->nodes.end(),
		[&](Node const& n) { return n.name == end; }
	);

	indexE = it - this->nodes.begin();
	if (it == this->nodes.end()) {
		return ErrState::nodeNotFound;
	}

	for (Node& n : this->nodes) n.eNodeState = Node::NodeState::UNVISITED;

	std::uint64_t currentNode = indexS;
	this->nodes[currentNode].associatedCost = 0;
	this->nodes[currentNode].eNodeState = Node::NodeState::LOCKED;

	unsigned int lcIndex;

	while (currentNode != indexE) {
		// traverse all connections to current node and update where necessary nearby nodes
		for (Connection c : this->nodes[currentNode].connections) {
			if (this->nodes[c.index].eNodeState == Node::NodeState::UNVISITED
				||
				(this->nodes[currentNode].associatedCost + c.cost) < this->nodes[c.index].associatedCost) {
				this->nodes[c.index].associatedCost = this->nodes[currentNode].associatedCost + c.cost;
				this->nodes[c.index].priorNode = currentNode;
				this->nodes[c.index].eNodeState = Node::NodeState::VISITED;
			}
		}

		// find the next node to be set to currentNode
		bool lowestCostSet = false;
		unsigned int lowestCost = 0;
		lcIndex = 0;
		for (int i = 0; i < this->nodes.size(); i++) {
			if (
				!(this->nodes[i].eNodeState == Node::NodeState::LOCKED)
				&& this->nodes[i].associatedCost > 0
				&& (!lowestCostSet || this->nodes[i].associatedCost < lowestCost)
			)
			{
				lowestCost = this->nodes[i].associatedCost;
				lcIndex = i;
				lowestCostSet = true;
			}
		}
		currentNode = lcIndex;
		if (!lowestCostSet) return ErrState::noPossiblePath;
		this->nodes[currentNode].eNodeState = Node::NodeState::LOCKED;
	}

	this->optimalRouteCost = this->nodes[currentNode].associatedCost;

	this->optimalRoute = this->nodes[currentNode].name;
	currentNode = this->nodes[currentNode].priorNode;
	while (currentNode != indexS) {
		this->optimalRoute = this->nodes[currentNode].name + " -> " + this->optimalRoute;
		currentNode = this->nodes[currentNode].priorNode;
	}
	this->optimalRoute = this->nodes[currentNode].name + " -> " + this->optimalRoute;

	return ErrState::success;
}

