#include <cassert>
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
  bool StartExists = G.addStartEnd();
  bool IsAcyclic = StartExists && G.acyclic();

  if (Result.count("acyclic")) {
    std::cout << "Acyclic: " << IsAcyclic << "\n";
    return 0;
  }

  if (Result.count("rpo")) {
    if (!IsAcyclic) {
      std::cout << "Error: provided graph is not acyclic!\n";
      return 1;
    }
    auto RPO = G.traverseRPO();
    std::cout << "Reverse Post Order:\n";
    for (auto It = RPO.begin(); It != RPO.end(); ++It) std::cout << *It << ' ';
    std::cout << '\n';
  }

  if (Result.count("dom")) {
    if (!G.getStart()) {
      std::cout << "Error: start node required for dominator tree!\n";
      std::cout << "Could not insert start node by default because\n"
                   " the provided graph does not have nodes w/o parents\n";
    } else {
      assert(G.getStart() && "Start node required");
      ujene::Graph DomTree = G.getDom().value();
      std::cerr << "Dominator tree:\n" << DomTree  << "\n";
      std::ofstream OFDomGraph(DomFname);
      ujene::printDot(OFDomGraph, DomTree);
    }
  }

  if (Result.count("postdom")) {
    if (!G.getEnd()) {
      std::cout << "Error: end node required for post dominator tree!\n";
      std::cout << "Could not insert end node by default because\n"
                   " the provided graph does not have nodes w/o children\n";
    } else {
      assert(G.getEnd() && "End node required");
      ujene::Graph PDomTree = G.getPDom().value();
      std::cerr << "Post Dominator tree:\n" << PDomTree  << "\n";
      std::ofstream OFPostDomGraph(PostDomFname);
      ujene::printDot(OFPostDomGraph, PDomTree);
    }
  }

  std::cerr << G;
  if (Result.count("dot")) {
    std::ofstream OFDotGraph(DotFname);
    ujene::printDot(OFDotGraph, G);
  }

  return 0;
}
