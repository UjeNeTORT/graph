#include "graph.h"
#include "node.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <memory>
#include <optional>
#include <span>
#include <sstream>
#include <stack>
#include <unordered_map>
#include <vector>

namespace {
  void printDotEdge(std::ostream &OS,
                    const UjeNeGraph::Node &From,
                    const UjeNeGraph::Node &To,
                    const std::string &Attr = "");
}

namespace UjeNeGraph {

bool Graph::addStartEnd() {
  bool is_empty = empty();

  std::vector<Node> NNoParents;
  std::vector<Node> NNoChildren;

  // remove former start, end nodes and error nodes
  for (auto [N, Ch] : Successors_)
    if (N.getTy() != NodeType::REGULAR) removeNode(N);

  // find nodes w/o children and parents, ignoring Start and End
  for (auto [N, Ch] : Successors_)
    if (Ch.empty()) NNoChildren.push_back(N);
  for (auto [N, Pr] : Predecessors_)
    if (Pr.empty()) NNoParents.push_back(N);

  // failed to choose Start/End node, which means cycles were detected
  if (!empty() && (NNoParents.empty() || NNoChildren.empty())) return false;

  Node StartNew(0, NodeType::START);
  Node EndNew(INT32_MAX, NodeType::END);
  addNode(StartNew);
  addNode(EndNew);
  for (auto NNop  : NNoParents)  addEdge(StartNew, NNop);
  for (auto NNoch : NNoChildren) addEdge(NNoch, EndNew);

  if (is_empty) addEdge(StartNew, EndNew);

  Start = std::make_unique<Node>(Successors_.find(StartNew)->first);
  End   = std::make_unique<Node>(Successors_.find(EndNew)->first);

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

bool Graph::acyclic() {
  // assert(Start != nullptr && "Graph must have a node marked as START");
  // assert(End != nullptr && "Graph must have a node marked as END");
  if (Start == nullptr || End == nullptr) return false;

  // only start and end nodes
  if (Successors_.size() == 2) return true;

  std::vector<const Node *> Nodes;
  for (const auto &[N, Ch] : Successors_)
    Nodes.push_back(&N);

  // stack where white nodes wait to get checked
  std::vector<Node> Visiting;

  enum COLOR { WHITE = 0, GREY = 1, BLACK = 2};
  std::unordered_map<Node, COLOR> Colors;
  for (auto It = Nodes.begin(); It != Nodes.end(); ++It)
    Colors[**It] = WHITE;

  Visiting.push_back(*Start.get());
  while (!Visiting.empty()) {
    Node Curr = Visiting.back();

    assert(Colors.find(Curr) != Colors.end());
    if (Colors[Curr] != GREY) {
      Colors[Curr] = GREY;

      assert(getChildren(Curr) != std::nullopt);

      for (const Node &Ch : Successors_[Curr]) {
        if      (Colors[Ch] == WHITE) Visiting.push_back(Ch);
        else if (Colors[Ch] == GREY)  return false; // found cycle
      }
    } else if (Colors[Curr] == GREY) {
      Visiting.pop_back();
      Colors[Curr] = BLACK;
    }
  }
  return true;
}

std::vector<Node> Graph::traverseRPO() {
  assert(acyclic() && "Graph must be acyclic");

  std::vector<const Node *> Nodes;
  for (const auto &[N, Ch] : Successors_)
    Nodes.push_back(&N);


  std::vector<Node> NNoParents;
  for (auto [N, Pr] : Predecessors_)
    if (Pr.empty()) NNoParents.push_back(N);

  std::vector<Node> NodesSorted;
  std::vector<std::pair<Node, Node>> RE; // removed edges

  while (!NNoParents.empty()) {
    Node N = NNoParents.back();
    NNoParents.pop_back();
    NodesSorted.push_back(N);
    for (Node &M : Successors_[N]) {
      // if edge E: N -> M removed - ignore it
      std::pair<Node, Node> E(N, M);
      if (std::find(RE.begin(), RE.end(), E) != RE.end()) continue;
      RE.push_back(E);

      // if M has no incoming edges, besides ignored, add to nodes without parents
      bool MHasParents = false;
      for (auto It = Predecessors_[M].begin(); It != Predecessors_[M].end(); ++It) {
        std::pair<Node, Node> E(*It, M);
        if (std::find(RE.begin(), RE.end(), E) == RE.end()) {
          MHasParents = true;
          break;
        }
      }
      if (!MHasParents) NNoParents.push_back(M);
    }
  }

  return NodesSorted;
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
