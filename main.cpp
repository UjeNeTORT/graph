#include <fstream>
#include <iostream>

#include "graph.h"
#include "cxxopts.hpp"

namespace ujene = UjeNeGraph;

int main(int argc, char *argv[]) {
  std::string RPOFname;
  std::string DomFname;
  std::string PostDomFname;
  cxxopts::Options Options("graph", "Algorithms on graphs");

  Options.add_options()
  ("h,help", "print help message")
  ("acyclic", "print 1 if graph is acyclic, 0 otherwise")
  ("rpo", "print rpo sorted graph",
    cxxopts::value<std::string>(RPOFname)->default_value("rpo.dot"))
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

  if (Result.count("acyclic")) {
    std::cout << "Acyclic: " << (StartEndExist && G.acyclic()) << "\n";
  }

  std::cout << G;
  std::ofstream OFs_dom_dot(DomFname);
  ujene::printDot(OFs_dom_dot, G);

  return 0;
}
