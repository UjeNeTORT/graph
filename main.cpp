#include <cstdint>
#include <iostream>

#include "CXXGraph/CXXGraph.hpp"
#include "graph.h"

namespace cxxg = CXXGraph;
namespace ujene = UjeNeGraph;

int main() {
  cxxg::Graph<uint32_t> g;
  ujene::Graph G;
  G.addNode(3);
  G.addNode(5);
  G.addNode(7);
  G.addNode(9);
  G.addNode(9);

  G.addEdge(3, 5);
  G.addEdge(3, 7);
  G.addEdge(3, 2);
  G.addEdge(3, 2);
  G.addEdge(5, 7);
  G.addEdge(7, 9);

  std::cout << G;
  ujene::printDot(std::cout, G);

  return 0;
}
