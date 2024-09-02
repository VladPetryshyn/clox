# Bytecode
We need to move away from just walking the AST, because it's fundamentally wrong design.

## Why not walk the AST?
**there's some advantages to it.**
1. It's really simple to implement. *Runtime representation directly maps to the syntax*.
It's effortless to get from the parser to the data structures we need at runtime.
2. It's portable, it is written in javascript, and runs on any platform javascript supports.

disadvantage is it's not memory-efficient. Each piece of syntax becomes an AST node.
Simple expression like 1 + 2 turns into a lot of objects with lots of pointers between them,
something like: 
[](./media/bytecode_ast.png)

The problem is, that this tree is 'sprinkling' our data across the heap, they are loosely connected, and thus, hurting the our spatial locality(the principle that data that is accessed or referenced spatially close to each other is likely to be accessed again in the near future).

**CPU process data way faster than they can pull it from RAM.**
Chips have multiple layers of chaching, to compensate for that. if the area it needs already in the cache, it can be loaded more quickly

**How data get into the cache?**
Whenever the CPU reads a bit of data from RAM, it pulls in a whole little bundle of adjacent bytes and stuffs them in the cache.
If our program requests some data close enough to be inside that cache, our CPU runs really fast. *We really want to take advantage of this*.

BUT in order to do so, the way we represent code in memory **should be dense and ordered like it's read.**

Because those sub objects can be anywhere, every step tree walker takes, may step out of CPU cache bounds, and force CPU to stall until a new lump of data can be slurped in from RAM.

*overhead of tree nodes* with all of their pointer fields and object headers, tends to push objects away form each other and out of the cache.

## Why Native Code is fast
Native code is just a dense series of operations encoded directly in binary. *Each instruction is between one and a few bytes long*.
cpu crancks through the instructions decoding and executing each one in order. There is no tree structure like our AST.
It simply jumps from 1 point to another.


# What is a bytecode?
We have 2 chairs.
1. Tree walk interpreter (simple, portable, slow)
2. Native code (complex, platform specific, fast)

Bytecode is somewhere in the middle, it retains portability of tree walker, **it sacrifices some simplicity to get performance boost in return**

Structuraly it resembles machine code, it's dense, linear sequence of binary instructions.

# we will write a VM
That emulation layer adds overhead, which is a key reason bytecode is slower than native code. But in return, it gives us portability. Write our VM in a language like C that is already supported on all the machines we care about, and we can run our emulator on top of any hardware we like.

[./media/bytecode_comparison_to_ast.png]

### Chunks of instructions
In our bytecode each instruction has a one-byte operation code(shortened to opcode). 
This number controls what kind of instruction we're dealing with - add, subtract, look up variable, etc.

# Storing values
for small fixed-size values like integers, many instructions sets store the value directly in the code stream, right after the opcode
These called *immediate instructions* because the bits for the value are immediately after the opcode.

This doesn't work for large or variable sized constants like strings. In a native compiler these instructions being stored in the separate "constant data" region in
binary executable. Then, the instruction to load constant has an address or offset pointing to where the value is stored in that section.

Our bytecode allows instructions to have operands. It lets us parametize what the instruction does.

**You can think of bytecode as a sort of compact serialization of the AST, highly optimized for how the interpreter will deserialize it in the order it needs as it executes.**


# Challenges
## 2
Tried to figure out solution, eventually gave up, found solution online, don't quite undestood it, because of the bit shifting.

## 3
#### How they keep track of which bytes are allocated and which are free?
They use a doubly linked list to store memory blocks.
Every block stores
1. Size of the corresponding memory block
2. Whether the block is free or not.
3. Address of the previous and next node in the doubly linked list.
4. Pointer to the starting of the memory block
5. Addres where data can be stored
#### What is requried to allocate a block of memory?
Doubly linked list, when we want to allocate more memory,
we traverse through it, to find a block which has at least equal amount of space that we want to allocate,
if it has *enough space* for us, we split the block into two, and adds new block to the linked list.
if *not enough*, we extend the heap and add a new memory block to the linked list.

#### What is required to free a block of memory?
1. Check if it's memory block allocated by malloc
2. if it is find the memory address of the block
3. if it's in the end of the list, it's being detached from the linked list.
4. if it' somewhere else, it's being merged with the other block.
