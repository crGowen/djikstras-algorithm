#pragma once

#ifdef DJIKALG_EXPORT
#define DJIKALG_IMEX __declspec(dllexport)
#else
#define DJIKALG_IMEX __declspec(dllimport)
#endif

#include <vector>
#include <string>
#include <map>

/*
* Alot of this code is VERY old, this needs a big refactor
*/

extern "C" {
	class DJIKALG_IMEX Node;

	class DJIKALG_IMEX Connection {
		friend class Node;
		friend class Graph;
	private:
		unsigned __int64 index;
		unsigned __int32 cost;

		Connection() = delete;

		Connection(const unsigned __int64, const unsigned __int32);
	};

	class DJIKALG_IMEX Node {
		friend class Graph;
		friend class Connection;
	private:
		enum class NodeState { UNVISITED, VISITED, LOCKED };
		NodeState eNodeState;
		std::string name;
		std::vector< Connection > connections;
		unsigned __int32 associatedCost;
		unsigned __int64 priorNode;

		Node() = delete; // forbid default constructor

		Node(const std::string&);

		bool CreateConnection(const unsigned __int64&, unsigned __int32);
	};

	class DJIKALG_IMEX Graph {
	private:
		std::string name;
		std::vector<Node> nodes;
		std::string optimalRoute;

		Graph() = delete; // forbid default constructor
	public:
		enum class ErrState { success, attemptToCreateExistingNode, unkCNErr, nodeNotFound, noPossiblePath };
		unsigned __int32 optimalRouteCost;

		// Create new graph with specified name
		Graph(const std::string& n);

		// Connect the specified nodes asymmetrically. If a specified node doesn't exist, it will be created
		ErrState ConnectNodes(const std::string& node1, const std::string& node2, const unsigned __int32 forwardCost, const unsigned __int32 backwardCost);

		// Connect the specified nodes symmetrically. If a specified node doesn't exist, it will be created
		ErrState ConnectNodes(const std::string& node1, const std::string& node2, const unsigned __int32 edgeCost);

		// Create Node for this graph
		ErrState CreateNode(const std::string& name);

		// Output detailed information to the console
		ErrState CoutListNodes();

		// Perform run through of Djikstra's Algorithm to find the optimal route between 2 nodes
		ErrState RunDjikstrasAlgorithm(const std::string& start, const std::string& end);

		std::string GetOptimalRoute();
	};
}
