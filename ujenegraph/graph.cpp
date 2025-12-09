#include "graph.h"

#include <iostream>

namespace {
  void printDotNode(std::ostream &OS,
                    const UjeNeGraph::NodeT &N,
                    const std::string &Attr = "");
  void printDotEdge(std::ostream &OS,
                    const UjeNeGraph::NodeT &From,
                    const UjeNeGraph::NodeT &To,
                    const std::string &Attr = "");
}

namespace UjeNeGraph {

const Graph::AdjList_t &Graph::getAdjList() const {
  return Successors_;
}

const std::string &Graph::getName() const {
  return Name_;
}

bool Graph::addNode(NodeT N) {
  if (Successors_.find(N) != Successors_.end()) return false;
  Successors_[N] = {};
  return true;
}

bool Graph::addEdge(const NodeT &From, const NodeT &To) {
  if (connected(From, To)) return false;
  addNode(To);
  Successors_[From].push_back(To);
  return true;
}

bool Graph::exists(const NodeT &N) const {
  return Successors_.find(N) != Successors_.end();
}

// @returns 'true' if there is a N1 -> N2 edge
//          'false' otherwise
bool Graph::connected(const NodeT &N1, const NodeT &N2) {
  if (!exists(N1) || !exists(N2)) return false;
  for (const NodeT &S : Successors_[N1])
    if (S == N2) return true;

  return false;
}

void Graph::print(std::ostream &OS) const {
  for (const auto &L : Successors_) {
    OS << L.first << ":";
    for (const NodeT &N : L.second)
      OS << " " << N ;
    OS << '\n';
  }
}

void printCli(std::ostream &OS, const Graph &G) {
  G.print(OS);
}

void printDot(std::ostream &OS, const Graph &G) {
  const auto &AdjList = G.getAdjList();
  OS << "digraph " << G.getName() << " {\n";
  for (const auto &L : AdjList)
    printDotNode(OS, L.first);

  for (const auto &L : AdjList)
    for (const NodeT &N : L.second)
      printDotEdge(OS, L.first, N);

  OS << "}\n";
}

std::ostream &operator<<(std::ostream &OS, const Graph &G) {
  printCli(OS, G);
  return OS;
}

} // namespace UjeNeGraph

namespace {
void printDotNode(std::ostream &OS,
                  const UjeNeGraph::NodeT &N,
                  const std::string &Attr) {
  OS << '\t' << N << "[" << Attr << "];\n";
}

void printDotEdge(std::ostream &OS,
                  const UjeNeGraph::NodeT &From,
                  const UjeNeGraph::NodeT &To,
                  const std::string &Attr) {
  OS << '\t' << From << " -> " << To << "[" << Attr << "];\n";
}
}
