# cs537_p3a

Authors: Collin Lull and Sui Jiet Tay
Project title: Parallel Sort

Algorithm: Merge Sort scaled across nproc - 12.
Thought process: 
1) Started with reading input and writing an output file
2) Replicate working merge sort scaled to >1GB
3) Incorporate multi-threading scaled across nproc.
4) Test it against with initial working implementation to make sure it ran faster
5) Old implementation: 9 min (single threaded) -> New implementation: 1min 15 sec (12 threads for each merge division)