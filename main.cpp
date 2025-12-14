#include <fstream>
#include <iostream>

#include "graph.h"
#include "cxxopts.hpp"

namespace ujene = UjeNeGraph;

int main(int argc, char *argv[]) {
  std::string DotFname;
  std::string DomFname;
  std::string PostDomFname;
  cxxopts::Options Options("graph", "Algorithms on graphs");

  Options.add_options()
  ("h,help", "print help message")
  ("acyclic", "print 1 if graph is acyclic, 0 otherwise")
  ("dot", "output graph in dot",
    cxxopts::value<std::string>(DotFname)->default_value("graph.dot"))
  ("rpo", "print rpo sorted graph")
  ("dom", "print dominator tree",
    cxxopts::value<std::string>(DomFname)->default_value("dom.dot"))
  ("postdom", "print post dominator tree",
    cxxopts::value<std::string>(PostDomFname)->default_value("postdom.dot"))
  ;

  auto Result = Options.parse(argc, argv);
  if (Result.count("h") || Result.count("help") || Result.arguments().empty()) {
    std::cout << Options.help() << "\n";
    return 0;
  }

  ujene::Graph G;
  std::cout << "Enter the graph: (to stop press Ctrl-D)\n";
  ujene::input(std::cin, G);
  bool StartEndExist = G.addStartEnd();
  bool IsAcyclic = StartEndExist && G.acyclic();

  if (Result.count("acyclic")) {
    std::cout << "Acyclic: " << IsAcyclic << "\n";
    return 0;
  }
  if (!IsAcyclic) {
    std::cout << "Error: provided graph is not acyclic!\n";
    return 1;
  }

  if (Result.count("rpo")) {
    auto RPO = G.traverseRPO();
    std::cout << "Reverse Post Order:\n";
    for (auto It = RPO.begin(); It != RPO.end(); ++It) std::cout << *It << ' ';
    std::cout << '\n';
  }

  if (Result.count("dom")) {
    auto Dom = getDom(G);
    std::cerr << "Dominator tree:\n" << Dom << "\n";
  }

  std::cerr << G;
  if (Result.count("dot")) {
    std::ofstream OFDotGraph(DotFname);
    ujene::printDot(OFDotGraph, G);
  }

  return 0;
}
