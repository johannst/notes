# [OProfile](https://oprofile.sourceforge.io/)

```markdown
operf -g -p <pid>
  -g ...... caputre call-graph information

opreport [opt] FILE
            show time spent per binary image
  -l ...... show time spent per symbol
  -c ...... show callgraph information (see below)
  -a ...... add column with time spent accumulated over child nodes

ophelp      show supported hw/sw events
```
