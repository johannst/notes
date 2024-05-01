# armv8

keywords: aarch64, arm64, A64, aarch32, A32, T32, abi

The `armv8` architecture introduces support for 64-bit and defines two
*execution states* `aarch64` and `aarch32`.

Implementations are **not** required to implement all execution states for all
*exception levels (EL)*. For example the [coretex-a32][coretex-a32] only
implements `aarch32`, while  the [coretex-a34][coretex-a34] only implements
`aarch64`.

The execution states support different instruction sets.
- `aarch64` only supports the new [`A64`][a64] instruction set, where all
  instructions have the fixed size of of 32 bits.
- `aarch32` supports the [`A32`][a32] and [`T32`][t32] instruction sets. These
  are updated versions of the `armv7` instruction sets, kept backwards
  compatible allowing `armv7` programs to run on `armv8`.
  > In `armv7` the instruction sets `A32` an `T32` were called `arm` and
  > `thumb` respectively.

A *program* always runs in either the `aarch64` or the `aarch32` execution
state, but never in a *mixture* of both. Transitions between execution states
only occur when raising or lowering the exception level.
- `aarch64 -> aarch32` can only occur when switching from *higher EL* to *lower
  EL*.
- `aarch32 -> aarch64` can only occur when switching from *lower EL* to *higher
  EL*.

The following figure depicts which execution state Transitions are allowed.
```
      (user) EL0     ^       |
        (os) EL1     |    32->64
(hypervisor) EL2  64->32     |
    (secure) EL3     |       v
```
This means for example, an *os* running in `aarch32` can only support `aarch32`
user applications, while an *os* running in `aarch64` can support
`aarch32 / aarch64` user applications.


[coretex-a32]: https://en.wikichip.org/wiki/arm_holdings/microarchitectures/cortex-a32
[coretex-a34]: https://en.wikichip.org/wiki/arm_holdings/microarchitectures/cortex-a34

[a32]: https://developer.arm.com/documentation/ddi0597/latest
[t32]: https://developer.arm.com/documentation/ddi0597/latest
[a64]: https://developer.arm.com/documentation/ddi0602/latest
