# UjeNeGraph
---

A library with basic graph algorithm implementations:
- graph cycles check
- topologic sort
- Dominator tree build
- Postdominator tree build

## Install and build

1. Clone the repository
```bash
git clone https://github.com/UjeNeTORT/graph.git --recursive
```

2. Build
```bash
cd graph
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

3. Run on examples
```bash
build/bin/graph # see help message
build/bin/graph --acyclic < examples/1.graph
build/bin/graph --rpo < examples/2.graph
build/bin/graph --dom < examples/3.graph
build/bin/graph --postdom < examples/4.graph
```
> [!NOTE]
> Some algorithms may not run due to ambiguity of provided graphs:
> `Start` and `End` nodes are only attached to nodes without parents or children,
> so if graph fails to find such nodes, it will not insert `Start` and `End`.
>
> Some algorithms depend on presence of nodes marked as `Start` or `End`.
>
> To resolve this, manually modify your graphs i.e. attach nodes without parents or
> children to explicitly mark them as entry or exit points.

4. Generate graphs images (graphviz required)
```bash
# generates image out of everything with .dot extension in folder
chmod +x gen_image.sh
./gen_image.sh .
```

