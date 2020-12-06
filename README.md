# Introduction

`ska::sort` is an in-place radix sort, implemented as a C++14 header-only
library using templates.

# Usage

For a vector of a basic type, just pass two iterators:

```cpp
ska::sort(durations.begin(), durations.end());
```

For a more complicated situation, provide a lambda that returns a key to sort
on:

```cpp
ska::sort(enemies.begin(), enemies.end(), [](const Enemey & enemy)
{
    return std::make_tuple(!is_in_combat(enemy), distance_to_player(enemy));
});
```

Check references for more examples.

# License

Boost Software License - Version 1.0

# References

* [I Wrote a Faster Sorting Algorithm][1]
* C++Now 2017: [Sorting in less than O(n log n): Generalizing and optimizing
radix sort][2]

[1]: https://probablydance.com/2016/12/27/i-wrote-a-faster-sorting-algorithm/
[2]: https://www.youtube.com/watch?v=zqs87a_7zxw
