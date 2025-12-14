#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>

namespace UjeNeGraph {

enum class NodeType : uint32_t {
  ERROR   = 0,
  REGULAR = 1, // part of original graph
  START   = 2, // connects connectivity components into a single graph
  END     = 3, // connects connectivity components into a single graph
};

class Node final {
  uint32_t Idx_;
  uint32_t LogicIdx_;
  NodeType Ty_;
public:
  Node(uint32_t Idx = 0, NodeType Ty = NodeType::REGULAR, uint32_t LogicIdx = INT32_MAX)
    : Idx_(Idx)
    , LogicIdx_(LogicIdx)
    , Ty_(Ty) {}
  NodeType getTy() const { return Ty_; }
  uint32_t getIdx() const { return Idx_; }
  uint32_t getLogicIdx() const { return LogicIdx_; }
  bool isSame(const Node &Other) const { return Idx_ == Other.Idx_; }
  void print(std::ostream &OS) const {
    if (Ty_ == NodeType::START) OS << "START"; // "(" << Idx_ << ")";
    else if (Ty_ == NodeType::END) OS << "END"; // "(" << Idx_ << ")";
    else OS << Idx_;
  }
};

bool operator==(const Node &Lhs, const Node &Rhs);
bool operator!=(const Node &Lhs, const Node &Rhs);
std::weak_ordering operator<=>(const Node &Lhs, const Node &Rhs);
std::istream &operator>>(std::istream &IS, Node &N);
std::ostream &operator<<(std::ostream &OS, const Node &N);

void printDotNode(std::ostream &OS,
                  const UjeNeGraph::Node &N,
                  const std::string &Attr = "");
} // namespace UjeNeGraph

template<>
struct std::hash<UjeNeGraph::Node> {
  std::size_t operator() (const UjeNeGraph::Node &N) const noexcept {
    return std::hash<decltype(N.getIdx())>{}(N.getIdx());
  }
};
