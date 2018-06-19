# Fractions
A simple header-only library for C++ to do fraction math.

NOTE: Fractions are not reduced/minimized on arithmetic-operations, but are reduced on comparisons.

Usage:

Copy [fractions.h](fractions.h) into your project.

```cpp
auto f1 = Fraction<int32_t, int32_t>{-3, 9};
printf("f1: %s\n", f1.to_string().c_str());
f1.reduce();
printf("f1-reduced: %s\n", f1.to_string().c_str());

auto f2 = frac32::from_float(0.5248f);
printf("f2: %s\n", f2.to_string().c_str());
```

Output:
```
f1: -3/9
f1-reduced: -1/3
f2: 328/625
f3: 6/21
```

(See [tests/test.cpp](tests/test.cpp) for more)

## License
MIT, see [license file](LICENSE).