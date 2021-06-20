#ifndef __PRIM_MINIMUM_SPANNING_TREE_H_
#define __PRIM_MINIMUM_SPANNING_TREE_H_

#include <optional>
#include <vector>
/* Feel free to add more standard library headers */
#include <algorithm>
#include <limits>
#include <iterator>

using entry_t = std::tuple<bool, size_t, vertex_t>;

/* Returns the vector of edges in the MST, or std::nullopt if MST does
 * not exist */


vertex_t min_distance(size_t V, vertex_t distance[], bool in[]) {
	vertex_t min = INF;
	size_t min_index;

	for (vertex_t v = 0; v < V; v++) {
		if (!in[v] && distance[v] < min) {
			min = distance[v];
			min_index = v;
		}

		std::cout << distance[v] << ", ";
	}
	std::cout << std::endl;

	std::cout << min_index << std::endl;
	
	return min_index;
}

std::optional<edges_t>
prim_minimum_spanning_tree(Graph& g, vertex_t src) {
	// constants
	const size_t V	 = g.get_size();
	
	edges_t mst(V - 1);
	int parent[V]; // store constructed MST
	vertex_t distance[V];
	bool in[V]; // set of vertices included in MST

	for (auto i = 0; i < V; i++) {
		distance[i] = INF;
		in[i] = false;
	}
		
	// Init root
	distance[src] = 0;
	parent[src] = -1;

	for (auto count = 0; count < V; count++) {
		// Pick the minimum key vertex from the 
        // set of vertices not yet included in MST 
		vertex_t vtx = min_distance(V, distance, in);

		in[vtx] = true;

		// Update distance and parent values of adjacent vertices
		for (vertex_t v = 0; v < V; v++) {				
			edge_weight_t weight = g.get_weight(vtx, v);

			if (!in[v] && weight > 0 && weight < distance[v]) {
				parent[v] = vtx;
				distance[v] = weight;
			}
		}
	}

	std::vector<std::tuple<int, size_t, vertex_t>> sorted(V);

	for (auto i = 0; i < V; i++) {
		std::get<0>(sorted[i]) = parent[i];
		std::get<1>(sorted[i]) = distance[i];
		std::get<2>(sorted[i]) = i;
	}

	std::sort(sorted.begin(), sorted.end());


	for (auto i = 0; i < V - 1; i++) {
		vertex_t v1 = std::get<2>(sorted[i]);
		vertex_t v2 = std::get<2>(sorted[i + 1]);

		std::get<0>(mst[i]) = v1;
		std::get<1>(mst[i]) = v2;
		std::get<2>(mst[i]) = distance[i + 1];
	}

	return mst;
}
#endif // __PRIM_MINIMUM_SPANNING_TREE_H_
