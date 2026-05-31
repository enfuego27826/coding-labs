# Old profiling details

Tools used were gprof, perf, flamegraphs. 

`perf stats -e` details - 


```bash
109654940      cycles                                                                  (38.47%)
154657760      instructions                     #    1.41  insn per cycle              (39.57%)
13468851      branches                                                                (39.54%)
720172      branch-misses                    #    5.35% of all branches             (24.14%)
52141431      cache-references                                                        (23.99%)
19890926      cache-misses                     #   38.15% of all cache refs           (24.15%)
47739399      L1-dcache-loads                                                         (24.30%)
22418610      L1-dcache-load-misses            #   46.96% of all L1-dcache accesses   (24.31%)

0.040702232 seconds time elapsed

0.037381000 seconds user
0.004041000 seconds sys
```

Hot paths marked by perf annotate -

![alt text](<Screenshot From 2026-05-31 18-53-59.png>)

Which is the linked list traversal done by chase_dependency. Another hot path marked was refresh_history (scatter writes) but this gave major speedup. 

`gprof` details - 

```bash
Flat profile:

Each sample counts as 0.01 seconds.
  %   cumulative   self              self     total           
 time   seconds   seconds    calls  ms/call  ms/call  name    
 56.67      0.17     0.17  1154376     0.00     0.00  chase_dependency(int, int, std::vector<int, std::allocator<int> > const&, std::vector<int, std::allocator<int> > const&)
 23.33      0.24     0.07        6    11.67    13.88  refresh_history(std::vector<int, std::allocator<int> >&, std::vector<Packet, std::allocator<Packet> > const&, int)
  6.67      0.26     0.02  1320000     0.00     0.00  branchy_score(Packet const&, std::vector<int, std::allocator<int> > const&)
  3.33      0.27     0.01 20792616     0.00     0.00  std::vector<int, std::allocator<int> >::operator[](unsigned long) const
  3.33      0.28     0.01 10741918     0.00     0.00  std::vector<int, std::allocator<int> >::operator[](unsigned long)
  3.33      0.29     0.01  2640012     0.00     0.00  std::vector<Packet, std::allocator<Packet> >::size() const
```

So the same observation as earlier. I didn't find the flamegraph to be too helpful to be honest but here's an attached photo -

![alt text](<flamegraph.svg>)


# Optimizations made - 

1. I precomputed chase_dependency for each linked list node so that we pay for the expensive traversal once and then everything else is free.
2. I optimized cold_column_prose because it was literally just summing up the whole history array but in a really cache unfriendly manner, so i just ran a simple for loop and summed it all.
3. So the scatter writes in history in `refresh_history` was really bad. So I just precomputed writes for each history cell and whenever `refresh_history` was called, it just summed those to the history cells, making it cache friendly and essentially we paid the cost once so that everything later on is free.
4. I used `O3` which vectorized the loops when it could, so I didn't have to worry about writing SIMD code.
5. Removed branching and replaced it with arithmetic operations although it didn't yield much performance.

# Results 

Note that all of the results posted below are on `O3` to have a fair comparison.

Before - 

```bash
user@anurag:~/coding-labs/assignment4-profiling/lab2$ time ./main
6040578838

real	0m0.839s
user	0m0.829s
sys	    0m0.004s

```

After - 

```bash
user@anurag:~/coding-labs/assignment4-profiling/lab2$ time ./main
6040578838

real	0m0.016s
user	0m0.011s
sys	    0m0.005s
```

Perf stats before - 

```bash
109654940      cycles                                                                  (38.47%)
154657760      instructions                     #    1.41  insn per cycle              (39.57%)
13468851       branches                                                                (39.54%)
720172         branch-misses                    #    5.35% of all branches             (24.14%)
52141431       cache-references                                                        (23.99%)
19890926       cache-misses                     #   38.15% of all cache refs           (24.15%)
47739399       L1-dcache-loads                                                         (24.30%)
22418610       L1-dcache-load-misses            #   46.96% of all L1-dcache accesses   (24.31%)

0.040702232 seconds time elapsed

0.037381000 seconds user
0.004041000 seconds sys
```

After - 

```bash
user@anurag:~/coding-labs/assignment4-profiling/lab2$ perf stat -e cycles,instructions,branches,branch-misses,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./main
6040578838

 Performance counter stats for './main':

          41608631      cycles                                                                  (38.26%)
          50588234      instructions                     #    1.22  insn per cycle              (38.17%)
           8763899      branches                                                                (38.11%)
            316195      branch-misses                    #    3.61% of all branches             (24.45%)
          22777582      cache-references                                                        (24.54%)
           1546624      cache-misses                     #    6.79% of all cache refs           (24.95%)
          36446629      L1-dcache-loads                                                         (24.87%)
           1115129      L1-dcache-load-misses            #    3.06% of all L1-dcache accesses   (24.92%)

       0.015644308 seconds time elapsed

       0.014297000 seconds user
       0.002042000 seconds sys
```

As you can see there are a lot of visible improvements. Branch misses can be improved but honestly that won't yield good improvement at this scale and it just increased more cache-misses so I just removed it. Here's the perf details with branching removed - 

```bash
user@anurag:~/coding-labs/assignment4-profiling/lab2$ perf stat -e cycles,instructions,branches,branch-misses,cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses ./main
6040578838

 Performance counter stats for './main':

          45916091      cycles                                                                  (37.58%)
          75735545      instructions                     #    1.65  insn per cycle              (42.65%)
           9148617      branches                                                                (43.30%)
              8186      branch-misses                    #    0.09% of all branches             (23.31%)
          25839914      cache-references                                                        (22.67%)
           2791985      cache-misses                     #   10.80% of all cache refs           (22.72%)
          26475675      L1-dcache-loads                                                         (22.71%)
           1848871      L1-dcache-load-misses            #    6.98% of all L1-dcache accesses   (22.64%)

       0.017404134 seconds time elapsed

       0.014063000 seconds user
       0.004018000 seconds sys

```

As you can see, worse performance just branch-misses removed but at the cost of cache-misses.



