# pidstat(1)

Trace minor/major page faults.
```markdown
pidstat -r -p <pid> [interval]
  minor_pagefault: happens when the page needed is already in memory but not
                   allocated to the faulting process, in that case the kernel
                   only has to create a new page-table entry pointing to the
                   shared physical page
  major_pagefault: happends when the page needed is NOT in memory, the kernel
                   has to create a new page-table entry and populate the
                   physical page
```
