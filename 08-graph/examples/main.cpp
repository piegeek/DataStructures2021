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
    edges_t dijkstra_edges1 = {{0, 1, 3.0f},
                               {0, 2, 1.0f},
                               {1, 2, 7.0f},
                               {1, 3, 5.0f},
                               {1, 4, 1.0f},
                               {2, 3, 2.0f},
                               {3, 4, 7.0f}};

    Graph g1(5, dijkstra_edges1, GraphType::UNDIRECTED);
    std::unordered_map<vertex_t,
                       std::optional<std::tuple<vertex_t,
                                                edge_weight_t>>> dijkstra_result
        = dijkstra_shortest_path(g1, 2);

    for(size_t i = 0 ; i < 5 ; ++i) {
        if(i == 2)
            continue;
        std::cout << std::get<0>(dijkstra_result[i].value()) << std::endl;
        std::cout << std::get<1>(dijkstra_result[i].value()) << std::endl;

        // REQUIRE(
        //     fabs(std::get<1>(dijkstra_result[i].value()) - dist_answer[i])
        //     < std::numeric_limits<edge_weight_t>::epsilon());
    }
    return 0;
}
