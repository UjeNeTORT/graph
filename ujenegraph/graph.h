#pragma once

#include <iostream>
#include <ostream>
#include <vector>
#include <unordered_map>

#include "node.h"

namespace UjeNeGraph {

class Graph final {
  // adjacency lists
  using AdjList_t = std::unordered_map<Node, std::vector<Node>>;
  AdjList_t Successors_;
  std::string Name_;
public:
  Graph(std::string Name = "UjeNeGraph") : Name_(Name) {}

  const AdjList_t &getAdjList() const;
  const std::string &getName() const;
  // @returns 'true' if the node was actually inserted
  //          'false' if it already existed
  bool addNode(Node N);

  // @returns 'true' if the edge was actually inserted
  //          'false' if it already existed
  bool addEdge(const Node &From, const Node &To);

  bool exists(const Node &N) const;
  bool connected(const Node &N1, const Node &N2);

  void print(std::ostream &OS) const;
};

void input(std::istream &IS, Graph &G);

void printCli(std::ostream &OS, const Graph &G);
void printDot(std::ostream &OS, const Graph &G);

std::ostream &operator<<(std::ostream &OS, const Graph &G);

} // namespace UjeNeGraph
