#ifndef __GRAPH_H_
#define __GRAPH_H_

#include <vector>
#include <unordered_map>
#include <list>
#include <tuple>

/* Feel free to add more standard library headers */
#include <limits>
const size_t INF = std::numeric_limits<size_t>::max();

// A vertex is typed as `vertex_t`. An edge is typed as `edge_t`,
// which is associated with a source vertex, a destination vertex, and
// its weight. A list of edges is typed as `edges_t`, which is a
// vector of `edge_t`.

using vertex_t = std::size_t;
using edge_weight_t = double;
using edge_t = std::tuple<vertex_t,
                          vertex_t,
                          edge_weight_t>;
using edges_t = std::vector<edge_t>;

enum class GraphType {
UNDIRECTED,
DIRECTED
};

/*
 * We do not enforce a specific design constraints other than the
 * consturtor. You may introduce your own changes as you
 * want---implementing your own member functions or variables. You
 * will still need to ensure that your constructor works properly,
 * such that we can run the test for grading. The provided test will
 * check if the constructor is correctly designed.
 */
class Graph {
    public:
        Graph() = delete;
        Graph(const Graph&) = delete;
        Graph(Graph&&) = delete;

        /* We assume that if num_vertices is n, a graph contains n vertices
           from 0 to n-1. */
        Graph(size_t num_vertices,
              const edges_t& edges,
              GraphType Type) {
            // TODO: Implement the constructor

            size_ = num_vertices;
            type_ = Type;
            // Allocate memory for adjacency matrix
            matrix_ = new edge_weight_t*[size_];
            for (auto i = 0; i < size_; i++) matrix_[i] = new edge_weight_t[size_];

            // Initialize to INF
            for (auto i = 0; i < size_; i++) {
                for (auto j = 0; j < size_; j++) {
                    matrix_[i][j] = INF;
                }
            }

            for (auto& edge : edges) {
                vertex_t from   = std::get<0>(edge);
                vertex_t to     = std::get<1>(edge);
                vertex_t weight = std::get<2>(edge);
                if (type_ == GraphType::DIRECTED) {
                    matrix_[from][to] = weight;
                }
                if (type_ == GraphType::UNDIRECTED) {
                    matrix_[from][to] = weight;
                    matrix_[to][from] = weight;
                }
            }

        }
        // TODO(optional): Define helper functions, e.g., out_deges(v)

        void print_graph() {
            for (auto i = 0; i < size_; i++) {
                for (auto j = 0; j < size_ - 1; j++) {
                    std::cout << matrix_[i][j] << ", ";
                }
                std::cout << matrix_[i][size_ - 1] << std::endl;
            }
        }

        size_t get_size() {
            return size_;
        }

        size_t get_indegree(size_t idx) {
            size_t indegree = 0;

            for (auto i = 0; i < size_; i++) {
                if (matrix_[i][idx] != INF) indegree++;
            }

            return indegree;
        }

        std::vector<vertex_t> get_neighbors(vertex_t vertex) {
            std::vector<vertex_t> neighbors;

            if (type_ == GraphType::DIRECTED) {
                for (auto i = 0; i < get_size(); i++) {
                    if (matrix_[vertex][i] != INF) neighbors.push_back(i);
                }
            }
            if (type_ == GraphType::UNDIRECTED) {
                for (auto i = 0; i < get_size(); i++) {
                    if (matrix_[vertex][i] != INF) neighbors.push_back(i);
                }
            }

            return neighbors;
        }

        edge_weight_t get_weight(vertex_t vtx1, vertex_t vtx2) {
            return matrix_[vtx1][vtx2];
        }

    private:
        // TODO: Roll out your own data structures
        size_t size_;
        GraphType type_;
        edge_weight_t** matrix_;
 
};

#endif // __GRAPH_H_
