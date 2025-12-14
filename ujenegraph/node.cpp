#include "node.h"

#include <iostream>

namespace UjeNeGraph {

bool operator==(const Node &Lhs, const Node &Rhs) {
  return Lhs.getIdx() == Rhs.getIdx();
}

bool operator!=(const Node &Lhs, const Node &Rhs) {
  return !(Lhs == Rhs);
}

std::weak_ordering operator<=>(const Node &Lhs, const Node &Rhs) {
  return Lhs.getIdx() <=> Rhs.getIdx();
}

std::istream &operator>>(std::istream &IS, Node &N) {
  uint32_t Idx = 0;
  IS >> Idx;
  N = Node(Idx);
  return IS;
}

std::ostream &operator<<(std::ostream &OS, const Node &N) {
  N.print(OS);
  return OS;
}

void printDotNode(std::ostream &OS,
                  const Node &N,
                  const std::string &Attr) {
  OS << '\t' << N << "[";
  if (N.getTy() != NodeType::REGULAR) OS << "style=filled, fillcolor=grey";
  if (!Attr.empty()) OS << ", ";
  OS << Attr << "];\n";
}

} // namespace UjeNeGraph
