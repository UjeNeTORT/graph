#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>

namespace UjeNeGraph {

class Node final {
  uint32_t Idx_;
  uint32_t LogicIdx_;
public:
  Node(uint32_t Idx = 0, uint32_t LogicIdx = INT32_MAX)
    : Idx_(Idx), LogicIdx_(LogicIdx) {}
  uint32_t getIdx() const { return Idx_; }
  uint32_t getLogicIdx() const { return LogicIdx_; }
  bool isSame(const Node &Other) const { return Idx_ == Other.Idx_; }
  void print(std::ostream &OS) const {
    OS << Idx_;
    if (LogicIdx_ != INT32_MAX)
      OS << "(" << LogicIdx_ << ")";
  }
};

bool operator==(const Node &Lhs, const Node &Rhs);
bool operator!=(const Node &Lhs, const Node &Rhs);
std::weak_ordering operator<=>(const Node &Lhs, const Node &Rhs);
std::istream &operator>>(std::istream &IS, Node &N);
std::ostream &operator<<(std::ostream &OS, const Node &N);

} // namespace UjeNeGraph

template<>
struct std::hash<UjeNeGraph::Node> {
  std::size_t operator() (const UjeNeGraph::Node &N) const noexcept {
    return std::hash<decltype(N.getIdx())>{}(N.getIdx());
  }
};
