#include <iostream>
#include "djikalg.h"

// much code copied from usage_example
int main() {
    int result = 0;
    printf("Running unit tests...\n");

    Graph myGraph("testGraph");

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
    
    std::cout << "\nTEST: Ensure that A->E shows correct optimal route...";
    const Graph::ErrState err1 = myGraph.RunDjikstrasAlgorithm("Alpha", "Echo");
    if (err1 == Graph::ErrState::success && myGraph.GetOptimalRoute() == "Alpha -> Charlie -> Delta -> Echo")
    {
        std::cout << "PASS.";
    }
    else
    {
        std::string msg = err1 == Graph::ErrState::success ? "Route: " + myGraph.GetOptimalRoute() : "ErrState != success";
        std::cout << "FAIL!\n\t" << msg;
        result |=16;
    }

    std::cout << "\nTEST: Ensure that E->A shows correct optimal route...";
    const Graph::ErrState err2 = myGraph.RunDjikstrasAlgorithm("Echo", "Alpha");
    if (err2 == Graph::ErrState::success && myGraph.GetOptimalRoute() == "Echo -> Bravo -> Alpha")
    {
        std::cout << "PASS.";
    }
    else
    {
        std::string msg = err2 == Graph::ErrState::success ? "Route: " + myGraph.GetOptimalRoute() : "ErrState != success";
        std::cout << "FAIL!\n\t" << msg;
        result |=32;

    }

    std::cout << "\n\n";
    return result;
}