We’ll call this common representation “Obj”. Each Lox value whose state lives on the heap is an Obj. We can thus use a single new ValueType case to refer to all heap-allocated types.

Because ObjString is an Obj, it also needs the state all Objs share. It accomplishes that by having its first field be an Obj. C specifies that struct fields are arranged in memory in the order that they are declared. Also, when you nest structs, the inner struct’s fields are expanded right in place. So the memory for Obj and for ObjString looks like this:
[./media/struct_inheritance.png]
Note how the first bytes of ObjString exactly line up with Obj. This is not a coincidence—C mandates it. This is designed to enable a clever pattern: You can take a pointer to a struct and safely convert it to a pointer to its first field and back.

Given an ObjString*, you can safely cast it to Obj* and then access the type field from it. Every ObjString “is” an Obj in the OOP sense of “is”. When we later add other object types, each struct will have an Obj as its first field. Any code that wants to work with all objects can treat them as base Obj* and ignore any other fields that may happen to follow.

Today, we should at least do the bare minimum: avoid leaking memory by making sure the VM can still find every allocated object even if the Lox program itself no longer references them. There are many sophisticated techniques that advanced memory managers use to allocate and track memory for objects. We’re going to take the simplest practical approach.

We’ll create a linked list that stores every Obj. The VM can traverse that list to find every single object that has been allocated on the heap, whether or not the user’s program or the VM’s stack still has a reference to it.

**I’ve seen language implementations die because it was too hard to get the GC in later. If your language needs GC, get it working as soon as you can. It’s a crosscutting concern that touches the entire codebase.**
