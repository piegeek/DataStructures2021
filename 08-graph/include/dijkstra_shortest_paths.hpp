#ifndef __DIJKSTRA_SHORTEST_PATHS_H_
#define __DIJKSTRA_SHORTEST_PATHS_H_

#include <unordered_map>
#include <optional>
#include <tuple>
/* Feel free to add more standard library headers */

#include "graph.hpp"


vertex_t dj_min_distance(size_t V, vertex_t distance[], bool in[]) {
	vertex_t min = INF;
	size_t min_index;

	for (vertex_t v = 0; v < V; v++) {
		if (!in[v] && distance[v] < min) {
			min = distance[v];
			min_index = v;
		}
	}
	
	return min_index;
}

/* Given a vertex `v`, `dijkstra_shortest_path` is:
 * - `nullopt` if `v` is not reacheble from `src`.
 * - {`u`, `d`} where `u` is the predecessor of `v` and `d` is the distance
 *   from `src`
 */
std::unordered_map<vertex_t,
    std::optional<std::tuple<vertex_t, edge_weight_t>>>
dijkstra_shortest_path(Graph& g, vertex_t src) {
    const size_t V = g.get_size();

    std::unordered_map<vertex_t, std::optional<std::tuple<vertex_t, edge_weight_t>>> map;
    bool visited[V];
    vertex_t distance[V];
    vertex_t parent[V];

    for (int i = 0; i < V; i++) {
        visited[i] = false;
        distance[i] = INF;
    }

    visited[src] = true;
    distance[src] = 0;
    parent[src] = -1;

    for (auto i = 0; i < V; i++) {
        vertex_t min = dj_min_distance(V, distance, visited);

        visited[min] = true;

        // Relaxation
        for (auto j = 0; j < V; j++) {
            if (g.get_weight(min, j) > 0 && visited[j] == false && distance[j] > distance[min] + g.get_weight(min, j)) {
                distance[j] = distance[min] + g.get_weight(min, j);
                parent[j] = min;
            }
        }
    }

    for (auto k = 0; k < V; k++) {
        if (distance[k] == INF) {
            map[k] = std::nullopt;
        }
        else {
            std::tuple<vertex_t, edge_weight_t> entry;
            std::get<0>(entry) = k == src ? src : parent[k];
            std::get<1>(entry) = g.get_weight(k, parent[k]);
            map[k] = entry;
        }
    }

    return map;
}

#endif // __DIJKSTRA_SHORTEST_PATHS_H_
