# E221

16 X 32 cache.

4 bits for cache line, 5 bits for line offset

Cache address:

1. line number = (p>>5) & ((1 >> 5) - 1)
2. word offset = p & ((1 >> 6) - 1)

## 64 words

### Best case

loop 1:

- 0..31: 1 miss, 31 hit
- 32..64: 1 miss, 31 hit

loop 2..4:

- 0 miss

total: 2 memory read, 2 lines, 62/64 hit ration

### Worst case

loop 1:

- 3 memory read, 3 lines 61/64 hit ration

## 512

### Best case

total: 16 memory readd 496/512

### Wors case

total: 17 reads per loop, 68 reads for 4 loops, 444/512
