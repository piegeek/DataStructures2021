#ifndef __TOPOLOGICAL_SORT_H_
#define __TOPOLOGICAL_SORT_H_

#include <vector>
#include "graph.hpp"
/* Feel free to add more standard library headers */
#include <queue>
#include <tuple>

/* Return _a_ valid topologically sorted sequence of vertex descriptors */
std::vector<vertex_t> topological_sort(Graph& g) {
	std::vector<vertex_t> indegree(g.get_size(), 0);

	for (auto i = 0; i < g.get_size(); i++) indegree[i] = g.get_indegree(i);

	std::queue<vertex_t> q;
	for (auto i = 0; i < g.get_size(); i++) {
		if (indegree[i] == 0) q.push(i);
	}

	std::vector<vertex_t> sorted;

	while (q.size() > 0) {
		vertex_t popped = q.front();
		q.pop();
		
		std::vector<vertex_t> neighbors = g.get_neighbors(popped);
		if (neighbors.size() > 0) {
			for (auto vtx : neighbors) {
				indegree[vtx]--;
				if (indegree[vtx] == 0) q.push(vtx);
			}

		}

		sorted.push_back(popped);
	}

	return sorted;
}

#endif // __TOPOLOGICAL_SORT_H_
