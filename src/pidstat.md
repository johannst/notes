# pidstat(1)

```markdown
pidstat [opt] [interval] [cont]
  -U [user]     show username instead UID, optionally only show for user
  -r            memory statistics
  -d            I/O statistics
```

# Page fault and memory utilization
```markdown
pidstat -r -p <pid> [interval] [count]
```

```markdown
minor_pagefault: Happens when the page needed is already in memory but not
                 allocated to the faulting process, in that case the kernel
                 only has to create a new page-table entry pointing to the
                 shared physical page (not required to load a memory page from
                 disk).

major_pagefault: Happens when the page needed is NOT in memory, the kernel
                 has to create a new page-table entry and populate the
                 physical page (required to load a memory page from disk).
```

# I/O statistics
```markdown
pidstat -d -p <pid> [interval] [count]
```
