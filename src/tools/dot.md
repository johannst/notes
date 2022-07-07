# dot(1)

[Online playground](https://edotor.net/)

## Example `dot` file to copy & paste from.

Can be rendered to `svg` with the following command.
```bash
dot -T svg -o g.svg g.dot
```

Example `dot` file.
```dot
// file: g.dot
digraph {
    // Render ranks from left to right.
    rankdir=LR
    // Make background transparent.
    bgcolor=transparent

    // Global node attributes.
    node [shape=box]
    // Global edge attributes.
    edge [style=dotted,color=red]

    // Add nodes & edge.
    stage1 -> stage2
    // Add multiple edges at once.
    stage2 -> { stage3_1, stage3_2 }
    // Add edge with custom attributes.
    stage3_2 -> stage4 [label="some text"]

    // Set custom attributes for specific node.
    stage4 [color=green,fillcolor=lightgray,style="filled,dashed",label="s4"]

    // Create a subgraph. This can be used to group nodes/edges or as scope for
    // global node/edge attributes.
    // If the name starts with 'cluster' a border is drawn.
    subgraph cluster_1 {
        stage5_1
        stage5_2
    }

    // Add some edges to subgraph nodes.
    stage3_1 -> { stage5_1, stage5_2 }
}
```

Rendered `svg` file.
![g.svg](https://johannst.github.io/notes/assets/g.svg)

## References
- [DOT language](https://graphviz.org/doc/info/lang.html)
- [Attributes](https://graphviz.org/doc/info/attrs.html)
- [Node shapes](https://graphviz.org/doc/info/shapes.html)
- [Colors](https://graphviz.org/doc/info/colors.html)
- [User manual](https://graphviz.org/pdf/dotguide.pdf)
