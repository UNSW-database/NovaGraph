#include "../graph.h"
#include <iostream>

val vertices_are_adjacent(igraph_integer_t src, igraph_integer_t tar) {
    igraph_bool_t res;
    bool hasWeights = VECTOR(globalWeights) != NULL;

    igraph_are_connected(&globalGraph, src, tar, &res);

    val result = val::object();
    val colorMap = val::object();
    val data = val::object();

    data.set("source", igraph_get_name(src));
    data.set("target", igraph_get_name(tar));
    colorMap.set(src, 1);
    colorMap.set(tar, 1);

    if (res) {
        std::string linkId = std::to_string(src) + '-' + std::to_string(tar);
        colorMap.set(linkId, 1);
        if (hasWeights) {
            igraph_integer_t eid;
            igraph_get_eid(&globalGraph, &eid, src, tar, true, 0);
            double weight = VECTOR(globalWeights)[eid];
            data.set("weight", weight);
        }
    }

    data.set("adjacent", res);
    result.set("colorMap", colorMap);
    result.set("mode", MODE_COLOR_SHADE_DEFAULT);
    result.set("data", data);
    return result;
}

val jaccard_similarity(val js_vs_list) {
    IGraphVectorInt vs_list;
    IGraphMatrix m;
    igraph_vs_t vs;

    val result = val::object();
    val colorMap = val::object();
    val data = val::object();
    val nodes = val::array();
    for (size_t i = 0; i < js_vs_list["length"].as<size_t>(); i++) {
        igraph_integer_t nodeId = js_vs_list[i].as<igraph_integer_t>();
        nodes.set(i, igraph_get_name(nodeId));
        colorMap.set(nodeId, 1);
        vs_list.push_back(nodeId);
    }

    igraph_vs_vector(&vs, vs_list.vec());
    igraph_similarity_jaccard(&globalGraph, m.mat(), vs, IGRAPH_OUT, false);
    igraph_matrix_printf(m.mat(), "%.2f");

    val rows = val::array();
    double max_similarity = -1.0;
    val max_pair = val::object();
    for (long int i = 0; i < m.nrows(); i++) {
        val row = val::array();
        for (long int j = 0; j < m.ncols(); j++) {
            std::stringstream stream;
            stream << std::fixed << std::setprecision(2) << m.get(i, j);
            double similarity = atof(stream.str().c_str());
            row.set(j, similarity);

            if (similarity > max_similarity && i != j) {
                max_similarity = similarity;
                int nodeId1 = vs_list.at(i);
                int nodeId2 = vs_list.at(j);
                max_pair.set("node1", igraph_get_name(nodeId1));
                max_pair.set("node2", igraph_get_name(nodeId2));
                max_pair.set("similarity", similarity);
            }
        }
        rows.set(i, row);
    }

    result.set("colorMap", colorMap);
    result.set("mode", MODE_COLOR_SHADE_DEFAULT);
    data.set("similarityMatrix", rows);
    data.set("maxSimilarity", max_pair);
    data.set("nodes", nodes);
    result.set("data", data);
    return result;    
}

val topological_sort(void) {
    igraph_bool_t isDAG;
    igraph_is_dag(&globalGraph, &isDAG);
    if (!isDAG) throw std::runtime_error("This graph is not a Directed Acyclic Graph (DAG) and cannot be topologically sorted.");

    IGraphVectorInt order;
    igraph_topological_sorting(&globalGraph, order.vec(), IGRAPH_OUT);

    val result = val::object();
    val colorMap = val::object();
    val data = val::object();
    val nodeOrder = val::array();

    // Node colors will get lighter colours (lower freq values) which will be scaled
    std::unordered_map<int, int> fm;
    int current_fm_value = order.size();
    for (igraph_integer_t v = 0; v < order.size(); v++) {
        val n = val::object();
        igraph_integer_t nodeId = order.at(v);
        n.set("id", nodeId);
        n.set("node", igraph_get_name(nodeId));
        nodeOrder.set(v, n);
        fm[nodeId] = current_fm_value--;
    }
    frequenciesToColorMap(fm, colorMap);

    result.set("colorMap", colorMap);
    result.set("mode", MODE_COLOR_SHADE_DEFAULT);
    data.set("order", nodeOrder);
    result.set("data", data);
    return result;
}

val diameter(void) {
    igraph_real_t diameter;
    igraph_integer_t src, tar;
    IGraphVectorInt vPath, ePath;
    bool hasWeights = VECTOR(globalWeights) != NULL;
    igraph_diameter_dijkstra(&globalGraph, igraph_weights(), &diameter, &src, &tar, vPath.vec(), ePath.vec(), true, true);

    val result = val::object();
    val colorMap = val::object();
    val data = val::object();

    data.set("source", igraph_get_name(src));
    data.set("target", igraph_get_name(tar));
    data.set("weighted", hasWeights);
    data.set("diameter", diameter);

    val path = val::array();
    for (int i = 0; i < vPath.size(); ++i) {
        int node = vPath.at(i);
        std::string nodeId = std::to_string(node);
        colorMap.set(nodeId, 0.5);

        if (i > 0) {
            std::string linkId = std::to_string(vPath.at(i-1)) + '-' + nodeId;
            colorMap.set(linkId, 1);

            val link = val::object();
            link.set("from", igraph_get_name(vPath.at(i-1)));
            link.set("to", igraph_get_name(node));

            int weight_index = ePath.at(i-1);
            if (hasWeights) {
                link.set("weight", VECTOR(globalWeights)[weight_index]);
            };
            path.set(i-1, link);
        }
    }
    colorMap.set(src, 1);
    colorMap.set(tar, 1);

    result.set("colorMap", colorMap);
    result.set("mode", MODE_COLOR_SHADE_DEFAULT);
    data.set("path", path);
    result.set("data", data);
    return result;
}