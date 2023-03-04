#pragma once

#ifdef _WIN32
	#ifdef DJIKALG_EXPORT
	#define DJIKALG_IMEX __declspec(dllexport)
	#else
	#define DJIKALG_IMEX __declspec(dllimport)
	#endif
#else
	#define DJIKALG_IMEX
#endif

#include <vector>
#include <string>
#include <map>
#include <cstdint>

/*
* Alot of this code is VERY old, this needs a big refactor
*/

extern "C" {
	class DJIKALG_IMEX Node;

	class DJIKALG_IMEX Connection {
		friend class Node;
		friend class Graph;
	private:
		std::uint64_t index;
		std::uint32_t cost;

		Connection() = delete;

		Connection(const std::uint64_t, const std::uint32_t);
	};

	class DJIKALG_IMEX Node {
		friend class Graph;
		friend class Connection;
	private:
		enum class NodeState { UNVISITED, VISITED, LOCKED };
		NodeState eNodeState;
		std::string name;
		std::vector< Connection > connections;
		std::uint32_t associatedCost;
		std::uint64_t priorNode;

		Node() = delete; // forbid default constructor

		Node(const std::string&);

		bool CreateConnection(const std::uint64_t&, std::uint32_t);
	};

	class DJIKALG_IMEX Graph {
	private:
		std::string name;
		std::vector<Node> nodes;
		std::string optimalRoute;

		Graph() = delete; // forbid default constructor
	public:
		enum class ErrState { success, attemptToCreateExistingNode, unkCNErr, nodeNotFound, noPossiblePath };
		std::uint32_t optimalRouteCost;

		// Create new graph with specified name
		Graph(const std::string& n);

		// Connect the specified nodes asymmetrically. If a specified node doesn't exist, it will be created
		ErrState ConnectNodes(const std::string& node1, const std::string& node2, const std::uint32_t forwardCost, const std::uint32_t backwardCost);

		// Connect the specified nodes symmetrically. If a specified node doesn't exist, it will be created
		ErrState ConnectNodes(const std::string& node1, const std::string& node2, const std::uint32_t edgeCost);

		// Create Node for this graph
		ErrState CreateNode(const std::string& name);

		// Output detailed information to the console
		ErrState CoutListNodes();

		// Perform run through of Djikstra's Algorithm to find the optimal route between 2 nodes
		ErrState RunDjikstrasAlgorithm(const std::string& start, const std::string& end);

		std::string GetOptimalRoute();
	};
}
