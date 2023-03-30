#include "djikalg.h"
#include <iostream>

int main() {

    Graph myGraph("myGraph");

    myGraph.CreateNode("Alpha");
    myGraph.CreateNode("Bravo");
    myGraph.CreateNode("Charlie");
    myGraph.CreateNode("Delta");
    myGraph.CreateNode("Echo");

    myGraph.ConnectNodes("Alpha", "Bravo", 3);
    myGraph.ConnectNodes("Alpha", "Charlie", 4);
    myGraph.ConnectNodes("Bravo", "Charlie", 5);

    // B->E costs 10, E->B costs 3... like a cliff
    myGraph.ConnectNodes("Bravo", "Echo", 10, 3);

    myGraph.ConnectNodes("Charlie", "Delta", 4);
    myGraph.ConnectNodes("Charlie", "Echo", 6);
    myGraph.ConnectNodes("Delta", "Echo", 1);

    // print list of nodes
    myGraph.CoutListNodes();

    const Graph::ErrState err1 = myGraph.RunDjikstrasAlgorithm("Alpha", "Echo");
    std::cout << "\n" << myGraph.GetOptimalRoute() << " with cost of " << myGraph.optimalRouteCost;

    const Graph::ErrState err2 = myGraph.RunDjikstrasAlgorithm("Echo", "Alpha");
    std::cout << "\n" << myGraph.GetOptimalRoute() << " with cost of " << myGraph.optimalRouteCost;

    return 0;
}