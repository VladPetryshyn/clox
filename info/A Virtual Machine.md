# An Instruction Execution Machine
You hand the vm a chunk of code - literally a Chunk - and it runs it.

# A Value Stack Manipulator
In addition to imperative side effects, Lox has expressions that produce, modify, and consume values. Thus, our compiled bytecode needs a way to shuttle values around between the different instructions that need them. For example:
```print 3 - 2;```

Because the nested tree is flattened out, we can't use recursion, and because of that we gonna use Stack.

As you step through, you see values appear and then later get eaten. The longest-lived ones are the values produced from the left-hand side of an addition.
Those stick around while we work through the right-hand operand expression.

In other words, we take all of those gaps up there and fill them in, pushing in numbers from the right:

(./media/vm_value_stack.png)

Stack based implementation is relatively fast, but very simple!

# Challenges
## 1 What bytecode instruction sequences would you generate for the following expressions:

```c
// 1 * 2 + 3
// 1
writeChunk(&chunk, OP_CONSTANT, 123);
writeChunk(&chunk, constant, 123);

// 2
constant = addConstant(&chunk, 2);
writeChunk(&chunk, OP_CONSTANT, 123);
writeChunk(&chunk, constant, 123);

writeChunk(&chunk, OP_MULTIPLY, 123);

writeConstant(&chunk, 3, 123);

writeChunk(&chunk, OP_ADD, 123);
```

```c
// 1 + 2 * 3
writeConstant(&chunk, 3, 123);
writeConstant(&chunk, 2, 123);
writeChunk(&chunk, OP_MULTIPLY, 123);

writeConstant(&chunk, 1, 123);
writeChunk(&chunk, OP_ADD, 123);
```

```c
// 3 - 2 - 1

writeConstant(&chunk, 3, 123);
writeConstant(&chunk, 2, 123);
writeChunk(&chunk, OP_SUBTRACT, 123);

writeConstant(&chunk, 1, 123);
writeChunk(&chunk, OP_SUBTRACT, 123);
```

``` c
// 1 + 2 * 3 - 4 / -5

writeConstant(&chunk, 2, 123);
writeConstant(&chunk, 3, 123);
writeChunk(&chunk, OP_MULTIPLY, 123);

writeConstant(&chunk, -4, 123);
writeConstant(&chunk, -5, 123);
writeChunk(&chunk, OP_DIVIDE, 123);

writeChunk(&chunk, OP_ADD, 123);

writeConstant(&chunk, 1, 123);
writeChunk(&chunk, OP_ADD, 123);

```
## 2 If we really wanted a minimal instruction set, we could eliminate either OP_NEGATE or OP_SUBTRACT. Show the bytecode instruction sequence you would generate for:

4 - 3 * -2
First, without using OP_NEGATE. Then, without using OP_SUBTRACT.

1. Without op-negate
```c
	// without op-negate
	writeConstant(&chunk, 4, 123);
	writeConstant(&chunk, 3, 123);
	writeChunk(&chunk, OP_SUBTRACT, 123);

	writeConstant(&chunk, -1, 123);
	writeConstant(&chunk, 2, 123);
	writeChunk(&chunk, OP_MULTIPLY, 123);

	writeChunk(&chunk, OP_MULTIPLY, 123);
```
2. Without op-subtract
```c
// without op_subtract

writeConstant(&chunk, 4, 1);
writeConstant(&chunk, 3, 1);
writeChunk(&chunk, OP_NEGATE, 1);
writeChunk(&chunk, OP_ADD, 1);

writeConstant(&chunk, 2, 1);
writeChunk(&chunk, OP_NEGATE, 1);
writeChunk(&chunk, OP_MULTIPLY, 1);
```


## 4 OPTIMIZE OP_NEGATE done
