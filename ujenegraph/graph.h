#pragma once

#include <iostream>
#include <optional>
#include <ostream>
#include <memory>
#include <span>
#include <vector>
#include <unordered_map>

#include "node.h"

namespace UjeNeGraph {

class Graph final {
  // adjacency lists
  using AdjList_t = std::unordered_map<Node, std::vector<Node>>;
  AdjList_t Successors_;
  AdjList_t Predecessors_;
  std::unique_ptr<Node> Start = nullptr;
  std::unique_ptr<Node> End = nullptr;
  std::string Name_;
public:
  Graph(std::string Name = "UjeNeGraph") : Name_(Name) {}

  const AdjList_t &getAdjList() const;
  std::optional<std::span<const Node>> getChildren(const Node &N) const;
  std::optional<std::span<const Node>> getParents(const Node &N) const;

  const std::string &getName() const;

  // @returns 'true' if the node was actually inserted
  //          'false' if it already existed
  bool addNode(Node N);
  bool removeNode(Node N);
  bool addStartEnd();

  // @returns 'true' if the edge was actually inserted
  //          'false' if it already existed
  bool addEdge(const Node &From, const Node &To);

  // check if no cycles exist
  bool acyclic();
  std::vector<Node> traverseRPO();

  bool exists(const Node &N) const;
  bool empty() const;
  bool connected(const Node &N1, const Node &N2);

  void print(std::ostream &OS) const;
};

void input(std::istream &IS, Graph &G);

void printCli(std::ostream &OS, const Graph &G);
void printDot(std::ostream &OS, const Graph &G);

std::ostream &operator<<(std::ostream &OS, const Graph &G);

} // namespace UjeNeGraph
