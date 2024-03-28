#ifndef GRAPH_H
#define GRAPH_H

#include "igraph_wrappers.h"
#include <emscripten/val.h>
#include <emscripten/bind.h>
#include <vector>
#include <string>
#include <unordered_map>

//#define N 11 // number of nodes
extern igraph_t igraphGlobalGraph;      // igraph structure
extern igraph_vector_t globalWeights;   // edge weights

#define MODE_COLOR_IMPORTANT        1   // Dark for important, light for less important
#define MODE_COLOR_SHADE_DEFAULT    2   // Multiple purple shades
#define MODE_COLOR_SHADE_ERROR      3   // Multiple purple shades with error for missing entries
#define MODE_SIZE_SCALAR            4   // No color change but size is scaled
#define MODE_RAINBOW                5   // Multiple colors in various groups

using namespace emscripten;

val initGraph(void);
void cleanupGraph(void);

std::string igraph_check_attribute(const igraph_t *graph);
igraph_error_t igraph_init_copy(igraph_t *to, const igraph_t *from);

val igraph_vector_int_to_val(igraph_vector_int_t* vec);
val igraph_vector_int_list_to_val(igraph_vector_int_list_t* v);
int getFreq(const val& map, std::string key);
void frequenciesToColorMap(std::unordered_map<int, int> fm, val &colorMap);


val vertices_are_connected(igraph_integer_t src, igraph_integer_t tar);
val dijkstra_source_to_target(igraph_integer_t src, igraph_integer_t tar);
val dijkstra_source_to_all(igraph_integer_t src);
val yen_source_to_target(igraph_integer_t src, igraph_integer_t tar, igraph_integer_t k);
val bf_source_to_target(igraph_integer_t src, igraph_integer_t tar);
val bf_source_to_all(igraph_integer_t src);
val bfs(igraph_integer_t src);
val dfs(igraph_integer_t src);
val randomWalk(igraph_integer_t start, int steps);
val min_spanning_tree(void);

val betweenness_centrality(void);
val closeness_centrality(void);
val degree_centrality(void);
val eigenvector_centrality(void);
val harmonic_centrality(void);
val strength(void);
val pagerank(igraph_real_t damping);

val louvain(igraph_real_t resolution);
val leiden(igraph_real_t resolution);
val fast_greedy(void);


#endif