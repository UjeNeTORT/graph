add_library(node STATIC node.cpp)
add_library(ujenegraph STATIC graph.cpp)
target_link_libraries(ujenegraph node)
