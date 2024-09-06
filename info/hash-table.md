# An array of buckets
A contiguous array of buckets that you index directly into. Is the basis of every hashmap, only later you add the collision detection, hash function, dynamic resizing.

With only 26 possible variables (27 if you consider underscore a “letter”, I guess), the answer is easy. Declare a fixed-size array with 26 elements. We’ll follow tradition and call each element a bucket. Each represents a variable with a starting at index zero. If there’s a value in the array at some letter’s index, then that key is present with that value. Otherwise, the bucket is empty and that key/value pair isn’t in the data structure.

Performance is even better. Given a variable name—its character—you can subtract the ASCII value of a and use the result to index directly into the array. Then you can either look up the existing value or store a new value directly into that slot. It doesn’t get much faster than that.


## Load factor and wrapped keys
The bigger the array, the fewer the indexes that get mapped to the same bucket and the fewer the collisions that are likely to occur.

We can track this collision likelihood by measuring **load factor**.

**Load Factor**: n. of entries / n of buckets. So a hash table with five entries and an array of 16 elements has a load factor of 0.3125. The higher the load factor, the greater the chance of collisions.

One way we mitigate collisions is by resizing the array. Just like the dynamic arrays we implemented earlier, we reallocate and grow the hash table’s array as it fills up. Unlike a regular dynamic array, though, we won’t wait until the array is full. *Instead, we pick a desired load factor and grow the array when it goes over that.*

# Collision Resolution
Even with a low collision factor, *we can't guarantee, that there will be no collisions.*

Collision resolution strategies

## 1. Separate chaining.
[./media/chaining_hash.png]
Instead of each bucket containing a single entry, we let it contain a collection of them. 
In the classic implementation each bucket points to a linked list of entries.
To find an entry you find its bucket, and then walk the list until you find an entry with the matching key.

In the worst case, it degrades to just a linked list. O(n).
It's easy to avoid by controlling the load factor and how entries get scattered across buckets(hashing). 
In typical separate-chained hash tables, it’s rare for a bucket to have more than one or two entries.

But it’s not a great fit for modern CPUs. It has a lot of overhead from pointers and tends to scatter little linked list nodes around in memory which isn’t great for cache usage.



## 2. Open addressing
With this technique all entries live directly in the bucket array, with one entry per bucket. 
*If two entries collide, we find a different empty bucket to use instead.*
When inserting an entry, its bucket may be full, sending us to look at another bucket. That bucket itself may be occupied and so on. This process of finding an available bucket is called *probing*, and the order that you examine buckets is a *probe sequence.*

The good thing about linear probing is that it’s cache friendly. Since you walk the array directly in memory order, it keeps the CPU’s cache lines full and happy. The bad thing is that it’s prone to clustering. If you have a lot of entries with numerically similar key values, *you can end up with a lot of colliding, overflowing buckets right next to each other.*


# Hash functions
Finally, we get to the “hash” part of “hash table”. A hash function takes some larger blob of data and “hashes” it to produce a fixed-size integer hash code whose value depends on all of the bits of the original data. A good hash function has three main goals:

1. It must be deterministic. The same input must always hash to the same number. If the same variable ends up in different buckets at different points in time, it’s gonna get really hard to find it.
2. It must be uniform. Given a typical set of inputs, it should produce a wide and evenly distributed range of output numbers, with as few clumps or patterns as possible. We want it to scatter values across the whole numeric range to minimize collisions and clustering.
3. It must be fast. Every operation on the hash table requires us to hash the key first. If hashing is slow, it can potentially cancel out the speed of the underlying array storage.

There is a veritable pile of hash functions out there. Some are old and optimized for architectures no one uses anymore. Some are designed to be fast, others cryptographically secure. Some take advantage of vector instructions and cache sizes for specific chips, others aim to maximize portability.

So for clox, I picked a simple, well-worn hash function called *FNV-1a* that’s served me fine over the years. Consider trying out different ones in your code and see if they make a difference.


# Deleting from the hash table
We can simply delete items from the hash map, by using mirror insertion. We use `findEntry()` to look up the entry's bucket. Then clear out the bucket. Done!
In case when there are no collisions, it works fine. But if it has a collision, the bucket can be part of one or more probe sequences.
[./media/hash_table_delete_1.png]

if we for example delete 'biscuit', by simply clearing the Entry, we brake our probe sequence, leaving the trailing entries unreachable.

if we later try to find jam, we will never find it
[./media/hash_table_delete_2.png]

we can solve this issue by using 'tombstones', instead of deleting the entry, we replace it with a 'tombstone', during the linear probing, when we hit the 'tombstone', we don't treat it like an empty slot.
[./media/hash_table_delete_3.png]
