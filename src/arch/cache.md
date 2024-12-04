# cache

Caches are organized by the following components
- `sets`
- `ways`
- `entries`

Each `set` consists of one or more `ways` and  a `way` is a single slot which
can hold an `entry`.
```
S-set / W-way cache

         +----------------- .. -----------+
SET 0    | WAY 0 | WAY 1 |      | WAY W-1 |
         +----------------- .. -----------+
SET 1    | WAY 0 | WAY 1 |      | WAY W-1 |
         +----------------- .. -----------+
..       |                                |
         +----------------- .. -----------+
SET S-1  | WAY 0 | WAY 1 |      | WAY W-1 |
         +----------------- .. -----------+
```

In general a cache is described by the number of `sets S` and the number of
`ways W`. Depending on the values for `S` and `W` caches can be further
classified.
- `W=1` is a `direct-mapped` cache, which means that each entry can be placed
  at exactly **ONE** location in the cache. It is also called a _one-way set
  associative_ cache.
- `S>1 & W>1` is a `W-way set associative` cache, which consists of S sets where
  each set consists of W ways. Each entry maps to a **UNIQUE** set, but to
  **ANY** way in that set.
- `S=1` is a `fully-associative` cache, which means that each entry can be
  placed at **ANY** location in the cache.

To determine which set an entry falls into, a `hash function` is applied on the
`key` which is associated with the entry. The set is then given by applying the
modulo operation to the hash value `hash % num_sets`.

The following figure illustrates the different cache classes and gives an
example which entries the given hash value `5` can map to.
```
direct-mapped      2-way set associative      fully-associative

HASH=5 (IDX=5%4)   HASH=5 (IDX=5%4)           HASH=5 (only one IDX)
|                  |                          |
|    S=4, W=1      |   S=4, W=2               |   S=1, W=4
|    +--------+    |   +--------+--------+    |   +--------+--------+--------+--------+
|   0|        |    |  0|        |        |    `->0| xxxxxx | xxxxxx | xxxxxx | xxxxxx |
|    +--------+    |   +--------+--------+        +--------+--------+--------+--------+
`- >1| xxxxxx |    `->1| xxxxxx | xxxxxx |
     +--------+        +--------+--------+
    2|        |       2|        |        |
     +--------+        +--------+--------+
    3|        |       3|        |        |
     +--------+        +--------+--------+
```

## CPU (hardware) caches
The number of sets in a hardware cache is usually a power of two. The `address`
acts as the key and some bits in the address are used to select the set in the
cache. The hash function in this case is simple, as it just extracts the bits
from the address which are used to select the set.

The `address` is usually split up into the `{ TAG, IDX, OFF }` bits which are
used to lookup an entry in the cache.

The `IDX` bits are used to index into the corresponding set, where the `TAG`
bits are then compared against the stored `TAG` bits in each way. If any way
holds an entry with the matching `TAG` bits, the lookup is a `HIT`, else a
`MISS`.

In case the entry is in the cache, the `OFF` bits are used to index into the
cache line. Hence, the number of offset bits available define the cache line
size.

The following gives an example for _64-bit addresses_ and a _direct-mapped_ cache.
```
        63                      0
        +-----------------------+
ADDR:   |  TAG  |  IDX  |  OFF  |
        +-----------------------+
            |       |       `------------------,
            |       |                          |
            |       |    CACHE                 |
            |       |    +----------------+    |
            |       |    | TAG | CACHE_LN |    |
            |       |    +----------------+    |
            |       |    | TAG | CACHE_LN |    |
            |       |    +----------------+    |
            |       |    | ..             |    |
            |       |    +----------------+    |
            |       `--> | TAG | CACHE_LN |    |
            |            +----------------+    |
            |               |     |            |
            |               v     v            |
            `-------------> =     + <----------`
                            |     |
                            v     v
                           HIT?  DATA


OFF bits: ln2 (cache_line_sz)
IDX bits: ln2 (num_sets)
TAG bits: 64 - IDX bits - OFF bits
```

The total size of a cache can be computed by `cache_line_sz * num_sets * num_ways`.
```
Example
  SETS: 64        => 6 IDX bits
  WAYS: 8
  LINE: 64 bytes  => 6 OFF bits

  SIZE: 64 sets * 8 ways * 64 bytes => 32k bytes
```

## Hardware caches with virtual memory
In the context of _virtual memory_, caches can be placed at different location
in the memory path, either _before_ or _after_ the `virtual address (VA)` to
`physical address (PA)` translation. Each placement has different properties
discussed in the following.

If the cache is placed _before_ the `VA -> PA` translation, it is called
`virtually indexed virtually tagged (VIVT)` cache, as it is indexed by a virtual
address and data in the cache is tagged with the virtual address as well.

The benefit of VIVT caches is that lookups are very fast as there is no need to
wait for the result of the address translation. However, VIVT caches may suffer
from the following problems.
- `synonyms`: different VAs map to the same PA. This can happen in a single
  address space (same page table), if for example a process maps the same file
  at different VAs (also commonly referred to as _aliasing_ or _cache-line
  sharing_). This can also happen in different address spaces (different page
  tables), if for example pages are shared between two processes.
  ```
  PT1
  +-------+
  |       |        PHYSMEM          PT2
  +-------+        +-------+        +-------+
  |  VA1  |---,    |       |        |       |
  +-------+   |    +-------+        +-------+
  |       |   +--->|  PA1  |<-------|  VA3  |
  +-------+   |    +-------+        +-------+
  |  VA2  |---`    |       |        |       |
  +-------+        +-------+        +-------+
  |       |
  +-------+

  Assume VA1 != VA2 != VA3

  CACHE
   TAG     DATA
  +-------+-------------+        Problems:
  |  VA1  | Copy of PA1 |        * multiple copies of the same data.
  |  VA3  | Copy of PA1 |        * write through one VA and read through a
  |       |             |          different VA results in reading stale data.
  |  VA2  | Copy of PA1 |
  +-------+-------------+
  ```
- `homonyms`: same VA corresponds to different PAs. This is the standard case
  between two different address spaces (eg in a multi-tasking os), for example
  if the same VA is used in two different processes, but it maps to a different
  PA for each process.
  ```
  PT1              PHYSMEM          PT2
  +-------+        +-------+        +-------+
  |  VA1  |------->|  PA1  |    ,---|  VA2  |
  +-------+        +-------+    |   +-------+
  |       |        |       |    |   |       |
  |       |        +-------+    |   |       |
  |       |        |  PA2  |<---`   |       |
  +-------+        +-------+        +-------+

  Assume VA1 == VA2

  CACHE
   TAG     DATA
  +-------+-------------+        Problems:
  |  VA1  | Copy of PA1 |        * same VA from different address spaces map to
  |       |             |          different PA
  |       |             |        * read thorugh VA2 returns data from PA1
  +-------+-------------+          rather than from PA2
  ```

While `synonyms` may lead to accessing _stale_ data, if there is no hardware to
guarantee coherency between aliased entries, `homonyms` may lead to accessing
the _wrong_ data.

On one hand there are multiple counter measures to avoid `homonyms`, for example
physical tagging, tags could contain an address space identifier (ASID), or the
cache could be flushed on context switches (changing the page table).
Approaches like physical tagging and ASIDs work, as the same VA always maps to
the same index in the cache, which would then result in a cache miss in case of
the homonym.

Preventing `synonyms` on the other hand is harder, as neither physical tagging
nor ASIDs help in this case. Flushing the cache during context switches only
helps with the case where different address spaces alias shared pages, but it
won't help if the same PA is aliased by different VAs in a single address space.
There are to alternative approaches, one is to have hardware support to detect
synonyms and the other one is to have the operating system only allow shared
mappings with VAs that have the same index bits for the cache. However, the
latter only works for direct-mapped caches, as there is only a single location
where those VAs could map to in the cache.

If the cache is placed _after_ the `VA -> PA` translation, it is called
`physically indexed physically tagged (PIPT)` cache, as it is indexed by a
physical address and data in the cache is tagged with the physical address as
well.

Compared to VIVT caches, PIPT caches do not suffer from `synonyms` or
`homonyms`. However, their major drawback is that the lookup depends on the
result of the address translation, and hence the translation and the cache
lookup happen sequentially which greatly decreases access latency.

Between VIVT and PIPT caches there is also a hybrid approach called `virtually
indexed physically tagged (VIPT)` cache, where the cache lookup is done with a
virtual address and the data is tagged with the physical address.

The benefit of this approach is that the cache lookup and the address
translation can be done in parallel, and due to the physical tagging, `homonyms`
are not possible.

For VIPT caches, `synonyms` may still happen depending on how the cache is
constructed.
- if the `index` bits for the cache lookup, exceed the `page offset` in the
  virtual address, then `synonyms` are still possible.
- if all the `index` bits for the cache lookup fall into the `page offset` of
  the virtual address, then the bits used for the cache lookup won't change
  during the `VA -> PA` translation, and hence the cache effectively operates as
  a PIPT cache. The only downside is that the number of sets in the cache is
  limited by the page size.

### VIPT as PIPT example
The following example shows that for a system with `4k` pages and cache lines of
`64 bytes` a VIPT cache can have at most `64 sets` to still act as PIPT cache.
```
      63      12              0
      +-----------------------+
VA:   |       |     PG_OFF    |
      +-----------------------+
CACHE BITS:   | C_IDX | C_OFF |
              +---------------+

PAGE SIZE  : 4k
PAGE OFFSET: ln (PAGE SIZE) = 12 bits

CACHE LINE  : 64 bytes
CACHE OFFSET: ln (CACHE LINE) = 6 bits

CACHE INDEX: PG_OFF - C_OFF = 6 bits
CACHE SETS : 2^CACHE INDEX = 64 sets
```
The total cache size can be increased by adding additional ways, however that
also has a practical upper limit, as adding more ways reduces the latency.

## Cache info in Linux
```sh
# Info about different caches (size, ways, sets, type, ..).
lscpu -C
# NAME ONE-SIZE ALL-SIZE WAYS TYPE        LEVEL SETS PHY-LINE COHERENCY-SIZE
# L1d       32K     128K    8 Data            1   64        1             64
# L1i       32K     128K    8 Instruction     1   64        1             64
# L2       256K       1M    4 Unified         2 1024        1             64
# L3         6M       6M   12 Unified         3 8192        1             64

# Info about how caches are shared between cores / hw-threads. Identified by
# the same cache ids on the same level.
lscpu -e
# CPU  CORE  L1d:L1i:L2:L3  ONLINE
#   0     0  0:0:0:0           yes
#   1     1  1:1:1:0           yes
#   4     0  0:0:0:0           yes
#   5     1  1:1:1:0           yes
#
# => CPU 0,4 share L1d, L1i, L2 caches (here two hw-threads of a core).
```
