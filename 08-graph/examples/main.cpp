#include <string>
#include <cstddef>
#include <iostream>
#include <vector>
#include <tuple>

#include "graph.hpp"
#include "topological_sort.hpp"
#include "dijkstra_shortest_paths.hpp"
#include "prim_minimum_spanning_tree.hpp"

int main(int argc, char *argv[]) {
    edges_t edges = {{0, 1, 1.0f}, {1, 2, 2.0f}};
    Graph graph(3, edges, GraphType::DIRECTED);

    graph.print_graph();

    std::optional<edges_t> sorted = prim_minimum_spanning_tree(graph, 0);

    for (auto s : sorted.value()) std::cout << std::get<2>(s) << std::endl;

    return 0;
}
