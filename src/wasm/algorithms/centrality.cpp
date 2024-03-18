#include "../graph.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

// For rendering on the frontend
#define MIN_SCALE 5
#define MAX_SCALE 30

#define NEGINF -9999

double scaleCentrality(double centrality, double max_centrality) {
    double scaled = MIN_SCALE + (MAX_SCALE - MIN_SCALE) * (centrality / max_centrality);
    return scaled;
}

igraph_real_t igraph_vector_max_nonan(const igraph_vector_t *v) {
    igraph_real_t max = NEGINF;
    for (long int i = 0; i < igraph_vector_size(v); ++i) {
        igraph_real_t value = VECTOR(*v)[i];
        if (!isnan(value) && value > max) {
            max = value;
        }
    }
    return max;
}

val betweenness_centrality(void) {
    igraph_vector_t betweenness;
    igraph_vector_init(&betweenness, 0);

    igraph_betweenness(&igraphGlobalGraph, &betweenness, igraph_vss_all(), true, NULL);

    double max_centrality = igraph_vector_max(&betweenness);
    val result = val::object();
    val sizeMap = val::object();
    std::string msg = "Betweenness Centrality:\n";

    for (igraph_integer_t v = 0; v < igraph_vcount(&igraphGlobalGraph); ++v) {
        double centrality = VECTOR(betweenness)[v];
        double scaled_centrality = scaleCentrality(centrality, max_centrality);

        sizeMap.set(v, scaled_centrality);

        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << centrality;
        msg += std::to_string(v) + "\t" + stream.str() + "\n";
    }
    result.set("sizeMap", sizeMap);
    result.set("message", msg);

    igraph_vector_destroy(&betweenness);
    return result;
}

val closeness_centrality(void) {
    igraph_vector_t closeness;
    igraph_vector_init(&closeness, 0);

    igraph_closeness(&igraphGlobalGraph, &closeness, NULL, NULL, igraph_vss_all(), IGRAPH_OUT, NULL, true);

    double max_centrality = igraph_vector_max_nonan(&closeness);
    val result = val::object();
    val sizeMap = val::object();
    std::string msg = "Closeness Centrality:\n";

    for (igraph_integer_t v = 0; v < igraph_vcount(&igraphGlobalGraph); ++v) {
        double centrality = VECTOR(closeness)[v];
        double scaled_centrality = scaleCentrality(isnan(centrality) ? 0 : centrality, max_centrality);

        sizeMap.set(v, scaled_centrality);

        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << centrality;
        msg += std::to_string(v) + "\t" + stream.str() + "\n";
    }
    result.set("sizeMap", sizeMap);
    result.set("message", msg);

    igraph_vector_destroy(&closeness);
    return result;
}

val degree_centrality(void) {
    igraph_vector_int_t degrees;
    igraph_vector_int_init(&degrees, 0);

    igraph_degree(&igraphGlobalGraph, &degrees, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS);

    double max_centrality = igraph_vector_int_max(&degrees);
    val result = val::object();
    val sizeMap = val::object();
    std::string msg = "Degree Centrality:\n";

    for (igraph_integer_t v = 0; v < igraph_vector_int_size(&degrees); ++v) {
        double centrality = VECTOR(degrees)[v];
        double scaled_centrality = scaleCentrality(centrality, max_centrality);
        sizeMap.set(v, scaled_centrality);

        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << centrality;
        msg += std::to_string(v) + "\t" + stream.str() + "\n";

    }

    result.set("sizeMap", sizeMap);
    result.set("message", msg);

    igraph_vector_int_destroy(&degrees);
    return result;
}

val eigenvector_centrality(void) {
    igraph_vector_t evs;
    igraph_vector_init(&evs, 0);
    igraph_real_t value;

    igraph_eigenvector_centrality(&igraphGlobalGraph, &evs, &value, IGRAPH_DIRECTED, false, NULL /*todo weights*/, NULL);

    double max_centrality = igraph_vector_max(&evs);
    val result = val::object();
    val sizeMap = val::object();
    std::string msg = "Eigenvector Centrality (scaled such that \"|max| = 1\"):\n";
    msg += "(eigenvalue = " + std::to_string(value) + ")\n";

    for (igraph_integer_t v = 0; v < igraph_vector_size(&evs); ++v) {
        double centrality = VECTOR(evs)[v];
        double scaled_centrality = scaleCentrality(centrality, max_centrality);
        sizeMap.set(v, scaled_centrality);

        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << centrality;
        msg += std::to_string(v) + "\t" + stream.str() + "\n";
    }
    result.set("sizeMap", sizeMap);
    result.set("message", msg);

    igraph_vector_destroy(&evs);
    return result;
}

val strength(void) {
    igraph_vector_t strengths;
    igraph_vector_init(&strengths, 0);

    igraph_strength(&igraphGlobalGraph, &strengths, igraph_vss_all(), IGRAPH_OUT, IGRAPH_NO_LOOPS, NULL /*TODO: weights*/);

    double max_centrality = igraph_vector_max(&strengths);
    val result = val::object();
    val sizeMap = val::object();
    std::string msg = "List of node strengths:\n";

    for (igraph_integer_t v = 0; v < igraph_vector_size(&strengths); ++v) {
        double centrality = VECTOR(strengths)[v];
        double scaled_centrality = scaleCentrality(centrality, max_centrality);
        sizeMap.set(v, scaled_centrality);

        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << centrality;
        msg += std::to_string(v) + "\t" + stream.str() + "\n";
    }

    result.set("sizeMap", sizeMap);
    result.set("message", msg);

    igraph_vector_destroy(&strengths);
    return result;
} 
