#pragma once

#include <cstdint>
#include <iostream>
#include <ostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace UjeNeGraph {

using NodeT = uint32_t;

class Graph final {
  // adjacency lists
  using AdjList_t = std::unordered_map<NodeT, std::vector<NodeT>>;
  AdjList_t Successors_;
  std::string Name_;
public:
  Graph(std::string Name = "UjeNeGraph") : Name_(Name) {}

  const AdjList_t &getAdjList() const;
  const std::string &getName() const;
  // @returns 'true' if the node was actually inserted
  //          'false' if it already existed
  bool addNode(NodeT N);

  // @returns 'true' if the edge was actually inserted
  //          'false' if it already existed
  bool addEdge(const NodeT &From, const NodeT &To);

  bool exists(const NodeT &N) const;
  bool connected(const NodeT &N1, const NodeT &N2);

  void print(std::ostream &OS) const;
};

void printCli(std::ostream &OS, const Graph &G);
void printDot(std::ostream &OS, const Graph &G);

std::ostream &operator<<(std::ostream &OS, const Graph &G);

} // namespace UjeNeGraph
