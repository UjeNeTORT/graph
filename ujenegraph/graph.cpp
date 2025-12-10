#include "graph.h"

#include <iostream>
#include <sstream>

namespace {
  void printDotNode(std::ostream &OS,
                    const UjeNeGraph::Node &N,
                    const std::string &Attr = "");
  void printDotEdge(std::ostream &OS,
                    const UjeNeGraph::Node &From,
                    const UjeNeGraph::Node &To,
                    const std::string &Attr = "");
}

namespace UjeNeGraph {

const Graph::AdjList_t &Graph::getAdjList() const {
  return Successors_;
}

const std::string &Graph::getName() const {
  return Name_;
}

bool Graph::addNode(Node N) {
  if (Successors_.find(N) != Successors_.end()) return false;
  Successors_[N] = {};
  return true;
}

bool Graph::addEdge(const Node &From, const Node &To) {
  if (connected(From, To)) return false;
  addNode(To);
  Successors_[From].push_back(To);
  return true;
}

bool Graph::exists(const Node &N) const {
  return Successors_.find(N) != Successors_.end();
}

// @returns 'true' if there is a N1 -> N2 edge
//          'false' otherwise
bool Graph::connected(const Node &N1, const Node &N2) {
  if (!exists(N1) || !exists(N2)) return false;
  for (const Node &S : Successors_[N1])
    if (S == N2) return true;

  return false;
}

void Graph::print(std::ostream &OS) const {
  for (const auto &L : Successors_) {
    OS << L.first << ":";
    for (const Node &N : L.second)
      OS << " " << N;
    OS << '\n';
  }
}

void input(std::istream &IS, Graph &G) {
  Node From;
  Node To;
  std::string Line;
  while(std::getline(IS, Line)) {
    std::stringstream SS(Line);
    SS >> From;
    G.addNode(From);
    while (SS >> To) {
      if (SS.fail()) {
        SS.clear();
        break;
      }
      G.addEdge(From, To);
    }
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
    for (const Node &N : L.second)
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
                  const UjeNeGraph::Node &N,
                  const std::string &Attr) {
  OS << '\t' << N << "[" << Attr << "];\n";
}

void printDotEdge(std::ostream &OS,
                  const UjeNeGraph::Node &From,
                  const UjeNeGraph::Node &To,
                  const std::string &Attr) {
  OS << '\t' << From << " -> " << To << "[" << Attr << "];\n";
}
}
