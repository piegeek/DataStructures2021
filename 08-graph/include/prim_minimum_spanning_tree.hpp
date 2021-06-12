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
	const size_t INF = std::numeric_limits<size_t>::max();
	
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

std::optional<edges_t>
prim_minimum_spanning_tree(Graph& g, vertex_t src) {
	// constants
	const size_t INF = std::numeric_limits<size_t>::max();
	const size_t V	 = g.get_size();
	
	edges_t mst(V - 1);
	vertex_t parent[V]; // store constructed MST
	vertex_t distance[V];
	bool in[V]; // set of vertices included in MST

	for (auto i = 0; i < V; i++) {
		distance[i] = INF;
		in[V] = false;
	}
		
	// Init root
	distance[src] = 0;
	parent[src] = -1;

	for (auto count = 0; count < V - 1; count++) {
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

	for (auto i = 0; i < V - 1; i++) {
		std::get<0>(mst[i]) = parent[i];
		std::get<1>(mst[i]) = parent[i + 1];
		std::get<2>(mst[i]) = g.get_weight(parent[i], parent[i + 1]);
	}

	return mst;

	// size_t num_added = 0;

	// std::vector<bool> in_mst(V, false);
	// std::vector<size_t> distance(V, INF);

	// distance[src] = 0;
	// in_mst[src] = true;

	// while (num_added < V) {
	// 	// Extract best vertex
	// 	vertex_t v;
	// 	std::vector<vertex_t> nb;

	// 	for (auto i = 0; i < V; i++) {
	// 		if (in_mst[i]) {
	// 			std::vector<vertex_t> temp = g.get_neighbors(i);

	// 			for (auto vtx : temp) {
	// 				if (std::find(nb.begin(), nb.end(), vtx) == nb.end()) nb.push_back(vtx);
	// 			}
	// 		}
	// 	}

	// 	for (auto vtx : nb) {

	// 	}
	// }
	

	// edges_t mst;
	// std::vector<vertex_t> added;
	// std::vector<entry_t> prim_tb(V);

	// // Initialze table
	// for (auto i = 0; i < V; i++) {
	// 	std::get<0>(prim_tb[i]) = false;
	// 	std::get<1>(prim_tb[i]) = INF;
	// 	std::get<2>(prim_tb[i]) = -1;
	// }

	// // Start from vertex src
	// std::get<1>(prim_tb[src]) = 0;

	// added.push_back(src);
	// std::get<0>(prim_tb[src]) = true;

	// // Create mst
	// while (mst.size() < V) {
	// 	std::vector<vertex_t> filtered_nb;

	// 	// Get neighbors of added
	// 	for (auto vtx : added) {
	// 		std::vector<vertex_t> nb = g.get_neighbors(vtx);
	// 		for (auto n : nb) {
	// 			if (std::find(added.begin(), added.end(), n) == added.end() && std::find(filtered_nb.begin(), filtered_nb.end(), n) == filtered_nb.end()) {
	// 				filtered_nb.push_back(n);
	// 			}
	// 		}
	// 	}

	// 	// if (filtered_nb.size() == 0) {
	// 	// 	break;
	// 	// }
		
	// 	// Find vtx with minimum distance
	// 	vertex_t min = filtered_nb[0];

	// 	for (auto vtx : filtered_nb) {
	// 		if (std::get<1>(prim_tb[vtx]) < std::get<1>(prim_tb[min])) min = vtx;
	// 	}

	// 	// Update prim table
	// 	for (auto vtx : filtered_nb) {
	// 		std::get<1>(prim_tb[vtx]) = g.get_weight(std::get<2>(prim_tb[min]), vtx);
	// 		std::get<2>(prim_tb[vtx]) = min;
	// 	}
	// }

	// g.print_graph();
	// std::copy(added.begin(), added.end(), std::ostream_iterator<vertex_t>(std::cout, " "));

	// for (auto i = 0; i < added.size() - 1; i++) {
	// 	edge_t item;
	// 	std::get<0>(item) = added[i];
	// 	std::get<1>(item) = added[i + 1];
	// 	std::get<2>(item) = g.get_weight(added[i], added[i + 1]);

	// 	mst.push_back(item);
	// }

	// return mst;
}
#endif // __PRIM_MINIMUM_SPANNING_TREE_H_
