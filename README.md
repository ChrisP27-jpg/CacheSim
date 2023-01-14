# CacheSim

This is another project I did in school. The program simulates a computers cache system.

## Usage

:$ make

:$ ./cache-sim [-r lru|mru|rand] [-s seed] [-v] s-E-b-m

where s-E-b-m specified cache parameters:

    s: # of bits in address used to specify set
  
    E: # of cache lines per set
  
    b: # of bits in address used to specify offset in cache line
  
    m: total # of bits used to address primary memory
  
  must have all non-negative and 2 <= b and b + s < m
