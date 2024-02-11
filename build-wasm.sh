em++ src/wasm/graph.cpp src/wasm/algorithms.cpp -o src/graph.js \
    -s WASM=1 \
    -I./src/wasm -I./src/wasm/igraph/build/include -I./src/wasm/igraph/include \
    -s EXPORT_ES6=1 \
    -s MODULARIZE=1 \
    -s ENVIRONMENT='web' \
    -s EXPORT_NAME='createModule' \
    -s LINKABLE=1 \
    -lembind --no-entry \
    -Os \
    ./src/wasm/igraph/build/src/libigraph.a
