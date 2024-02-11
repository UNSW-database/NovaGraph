#include "graph.h"
#include <iostream>

// Check connection
bool vertices_are_connected(igraph_integer_t src, igraph_integer_t tar) {
    igraph_bool_t res;
    igraph_are_connected(&globalGraph, src, tar, &res);
    std::cout << "Connected status: " << res << std::endl;
    return res;
}


// DIJKSTRA

VecInt dijkstra_source_to_target(igraph_integer_t src, igraph_integer_t tar) {
    VecInt vs;
    igraph_vector_int_t vertices;

    // TODO: change final NULL to weights
    igraph_get_shortest_path_dijkstra(&globalGraph, &vertices, NULL, src, tar, NULL, IGRAPH_OUT);
    
    for (int i = 0; i < igraph_vector_int_size(&vertices); ++i) {
        vs.push_back(VECTOR(vertices)[i]);
    }

    igraph_vector_int_destroy(&vertices);

    return vs;
}

std::vector<VecInt> dijkstra_source_to_all(igraph_integer_t src) {
    std::vector<VecInt> res;
    igraph_vector_int_list_t paths;
    igraph_vector_int_list_init(&paths, 0);

    igraph_vs_t targets = igraph_vss_all(); // list of all vertices

    igraph_get_shortest_paths_dijkstra(&globalGraph, &paths, NULL, src, targets, /* TODO*/ NULL, IGRAPH_OUT, NULL, NULL);

    long numPaths = igraph_vector_int_list_size(&paths);

    for (long i = 0; i < numPaths; ++i) {
        igraph_vector_int_t path = VECTOR(paths)[i];
        std::vector<int> pathVector(VECTOR(path), VECTOR(path) + igraph_vector_int_size(&path));
        res.push_back(pathVector);
    }

    igraph_vector_int_list_destroy(&paths);

    return res;
}


// A*

/*
igraph_error_t my_heuristic(igraph_real_t *result, igraph_integer_t from, igraph_integer_t to, void *extra) {
    // Your heuristic calculation goes here
    // This function should estimate the distance from 'from' to 'to'
    // It should provide a lower value for better candidate exploration

    // For simplicity, let's use a constant heuristic value for demonstration
    *result = 1.0;
    igraph_distance

    return IGRAPH_SUCCESS;
}

VecInt astar_source_to_target(igraph_integer_t src, igraph_integer_t tar) {

    igraph_vector_int_t vertices;
    //igraph_vector_int_t edges;

    // TODO: change final NULL to weights
    igraph_get_shortest_path_astar(&globalGraph, &vertices, NULL, src, tar, NULL, IGRAPH_OUT, my_heuristic, NULL);

    VecInt vs;
    for (int i = 0; i < igraph_vector_int_size(&vertices); ++i) {
        vs.push_back(VECTOR(vertices)[i]);
    }

    igraph_vector_int_destroy(&vertices);

    return vs;
}
*/

// Yen
std::vector<VecInt> yen_source_to_target(igraph_integer_t src, igraph_integer_t tar, igraph_integer_t k) {
    std::vector<VecInt> res;
    igraph_vector_int_list_t paths;
    igraph_vector_int_list_init(&paths, 0);

    igraph_get_k_shortest_paths(&globalGraph, NULL, &paths, NULL, k, src, tar, IGRAPH_OUT);

    long numPaths = igraph_vector_int_list_size(&paths);

    for (long i = 0; i < numPaths; ++i) {
        igraph_vector_int_t path = VECTOR(paths)[i];
        std::vector<int> pathVector(VECTOR(path), VECTOR(path) + igraph_vector_int_size(&path));
        res.push_back(pathVector);
    }

    igraph_vector_int_list_destroy(&paths);

    return res;
}

    
// BELLMAN-FORD

VecInt bf_source_to_target(igraph_integer_t src, igraph_integer_t tar) {
    VecInt vs;
    igraph_vector_int_t vertices;

    // TODO: change final NULL to weights
    igraph_get_shortest_path_bellman_ford(&globalGraph, &vertices, NULL, src, tar, NULL, IGRAPH_OUT);

    for (int i = 0; i < igraph_vector_int_size(&vertices); ++i) {
        vs.push_back(VECTOR(vertices)[i]);
    }

    igraph_vector_int_destroy(&vertices);

    return vs;
}

std::vector<VecInt> bf_source_to_all(igraph_integer_t src) {
    std::vector<VecInt> res;
    igraph_vector_int_list_t paths;
    igraph_vector_int_list_init(&paths, 0);

    igraph_vs_t targets = igraph_vss_all(); // list of all vertices

    igraph_get_shortest_paths_bellman_ford(&globalGraph, &paths, NULL, src, targets, /* TODO*/ NULL, IGRAPH_OUT, NULL, NULL);

    long numPaths = igraph_vector_int_list_size(&paths);

    for (long i = 0; i < numPaths; ++i) {
        igraph_vector_int_t path = VECTOR(paths)[i];
        std::vector<int> pathVector(VECTOR(path), VECTOR(path) + igraph_vector_int_size(&path));
        res.push_back(pathVector);
    }
    
    igraph_vector_int_list_destroy(&paths);

    return res;
}