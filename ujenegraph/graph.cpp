#include "graph.h"
#include "node.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <span>
#include <sstream>

namespace {
  void printDotEdge(std::ostream &OS,
                    const UjeNeGraph::Node &From,
                    const UjeNeGraph::Node &To,
                    const std::string &Attr = "");
}

namespace UjeNeGraph {

bool Graph::addStartEnd() {
  std::vector<Node> NNoParents;
  std::vector<Node> NNoChildren;

  // remove former start, end nodes and error nodes
  for (auto [N, Ch] : Successors_)
    if (N.getTy() != NodeType::REGULAR) removeNode(N);

  // find nodes w/o children and parents, ignoring Start and End
  for (auto [N, Ch] : Successors_)
    if (Ch.empty() || Ch[0] == N) NNoChildren.push_back(N);
  for (auto [N, Pr] : Predecessors_)
    if (Pr.empty() || Pr[0] == N) NNoParents.push_back(N);

  Node StartNew(0, NodeType::START);
  Node EndNew(INT32_MAX, NodeType::END);
  addNode(StartNew);
  addNode(EndNew);
  for (auto NNop  : NNoParents)  addEdge(StartNew, NNop);
  for (auto NNoch : NNoChildren) addEdge(NNoch, EndNew);

  Start = std::make_unique<Node>(StartNew);
  End   = std::make_unique<Node>(EndNew);

  return true;
}

const Graph::AdjList_t &Graph::getAdjList() const {
  return Successors_;
}

std::optional<std::span<const Node>> Graph::getChildren(const Node &N) const {
  if (!exists(N)) return std::nullopt;
  assert(Successors_.find(N) != Successors_.end() && "exists() returned true for non existent map object");
  return std::span<const Node>(Successors_.at(N));
}

std::optional<std::span<const Node>> Graph::getParents(const Node &N) const {
  if (!exists(N)) return std::nullopt;
  assert(Predecessors_.find(N) != Predecessors_.end() && "exists() returned true for non existent map object");
  return std::span<const Node>(Predecessors_.at(N));
}

const std::string &Graph::getName() const {
  return Name_;
}

bool Graph::addNode(Node N) {
  if (exists(N)) return false;
  Successors_[N] = {};
  Predecessors_[N] = {};
  return true;
}

bool Graph::removeNode(Node N) {
  if(!exists(N)) return false;

  for (auto S : Successors_[N]) {
    Predecessors_[S].erase(
      std::find(
        Predecessors_[S].begin(), Predecessors_[S].end(), N
      )
    );
  }

  for (auto P : Predecessors_[N]) {
    Successors_[P].erase(
      std::find(
        Successors_[P].begin(), Successors_[P].end(), N
      )
    );
  }

  Successors_.erase(N);
  Predecessors_.erase(N);
  return true;
}

bool Graph::addEdge(const Node &From, const Node &To) {
  if (connected(From, To)) return false;
  addNode(To);
  Successors_[From].push_back(To);
  Predecessors_[To].push_back(From);
  return true;
}
bool Graph::acyclic() const {
  if (empty()) return true;
}

bool Graph::exists(const Node &N) const {
  return Successors_.find(N) != Successors_.end();
}

bool Graph::empty() const { return Successors_.empty(); }

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
    printDotNode(OS, L.first, L.first.getTy() == NodeType::REGULAR
                                 ? ""
                                 : "style=\"filled\", fillcolor=\"grey\""); // todo fix styles

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

void printDotEdge(std::ostream &OS,
                  const UjeNeGraph::Node &From,
                  const UjeNeGraph::Node &To,
                  const std::string &Attr) {
  OS << '\t' << From << " -> " << To << "[" << Attr << "];\n";
}
}
