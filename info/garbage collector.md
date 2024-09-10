# Mark and sweep collector
once the program was out of memory, it would go back and find unused storage it could reclaim.



    Marking: We start with the roots and traverse or trace through all of the objects those roots refer to. This is a classic graph traversal of all of the reachable objects. Each time we visit an object, we mark it in some way. (Implementations differ in how they record the mark.)

    Sweeping: Once the mark phase completes, every reachable object in the heap has been marked. That means any unmarked object is unreachable and ripe for reclamation. We go through all the unmarked objects and free each one.

[./media/mark-sweep.png]

The time spent actually freeing memory is the same, but the GC spends cycles figuring out which memory to free. That is time not spent running the user’s code and doing useful work. In our implementation, that’s the entirety of the mark phase. The goal of a sophisticated garbage collector is to minimize that overhead.

There are two key metrics we can use to understand that cost better:

    Throughput is the total fraction of time spent running user code versus doing garbage collection work. Say you run a clox program for ten seconds and it spends a second of that inside collectGarbage(). That means the throughput is 90%—it spent 90% of the time running the program and 10% on GC overhead.

The idea is that the collector frequency automatically adjusts based on the live size of the heap. We track the total number of bytes of managed memory that the VM has allocated. When it goes above some threshold, we trigger a GC. After that, we note how many bytes of memory remain—how many were not freed. Then we adjust the threshold to some value larger than that.

Self-adjusting heap
The idea is that the collector frequency automatically adjusts based on the live size of the heap. We track the total number of bytes of managed memory that the VM has allocated. When it goes above some threshold, we trigger a GC.
