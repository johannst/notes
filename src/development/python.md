# python

## Decorator [[run][run-decorator]]
Some decorator examples with type annotation.

```python
from typing import Callable

def log(f: Callable[[int], None]) -> Callable[[int], None]:
    def inner(x: int):
        print(f"log::inner f={f.__name__} x={x}")
        f(x)
    return inner

@log
def some_fn(x: int):
    print(f"some_fn x={x}")


def log_tag(tag: str) -> Callable[[Callable[[int], None]], Callable[[int], None]]:
    def decorator(f: Callable[[int], None]) -> Callable[[int], None]:
        def inner(x: int):
            print(f"log_tag::inner f={f.__name__} tag={tag} x={x}")
            f(x)
        return inner
    return decorator

@log_tag("some_tag")
def some_fn2(x: int):
    print(f"some_fn2 x={x}")
```

## Walrus operator [[run][run-walrus]]
Walrus operator `:=` added since **python 3.8**.
```python
from typing import Optional

# Example 1: if let statements

def foo(ret: Optional[int]) -> Optional[int]:
    return ret

if r := foo(None):
    print(f"foo(None) -> {r}")

if r := foo(1337):
    print(f"foo(1337) -> {r}")

# Example 2: while let statements

toks = iter(['a', 'b', 'c'])
while tok := next(toks, None):
    print(f"{tok}")

# Example 3: list comprehension

print([tok for t in ["  a", "  ", " b "] if (tok := t.strip())])
```

## [Unittest][doc-unittest] [[run][run-unittest]]
Run unittests directly from the command line as \
`python3 -m unittest -v test`

Optionally pass `-k <patter>` to only run subset of tests.
```python
# file: test.py

import unittest

class MyTest(unittest.TestCase):
    def setUp(self):
        pass
    def tearDown(self):
        pass
    # Tests need to start with the prefix 'test'.
    def test_foo(self):
        self.assertEqual(1 + 2, 3)
    def test_bar(self):
        with self.assertRaises(IndexError):
            list()[0]
```

## [Doctest][doc-doctest] [[run][run-doctest]]
Run doctests directly from the command line as \
`python -m doctest -v test.py`

```python
# file: test.py

def sum(a: int, b: int) -> int:
    """Sum a and b.

    >>> sum(1, 2)
    3

    >>> sum(10, 20)
    30
    """
    return a + b
```

## [timeit][doc-timeit]
Micro benchmarking.
```bash
python -m timeit '[x.strip() for x in ["a ", " b"]]'
```

[run-decorator]: https://www.online-python.com/IDdiE0gpYU
[run-walrus]: https://www.online-python.com/9T12PvmKVy
[doc-unittest]: https://docs.python.org/3/library/unittest.html
[run-unittest]: https://www.online-python.com/2fit4UcbzI
[doc-doctest]: https://docs.python.org/3/library/doctest.html
[run-doctest]: https://www.online-python.com/LZst51UNIH
[doc-timeit]: https://docs.python.org/3/library/timeit.html
