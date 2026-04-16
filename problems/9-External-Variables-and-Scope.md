# Problems — Day 09 (External Variables and Scope)

## Problem 1 — Assuming all global initialization goes to the same memory segment

**What I wrote**
int max = 0;           // at file scope
char longest[MAXLINE] = "";  // at file scope

What I thought

Both variables are initialized at compile time and stored in the same way.

What actually happens

Global variables with zero initialization (int max = 0) are typically placed in .bss. Explicitly initialized globals (char longest[] = "") go to .data. In some older toolchains, uninitialized or zero-initialized globals may be emitted as common symbols, but modern compilers (with -fno-common by default) place zero-initialized data in .bss.

The distinction matters for embedded systems (memory footprint, load time) and for understanding how C maps to hardware.

Fix

Know the rule:

· .bss — zero-initialized globals (no space in binary)
· .data — explicitly initialized globals (space in binary)

int max = 0 is zero-initialization → .bss in modern toolchains.

# Problem 2 — Calling a read of a global "accidental correctness"

What I wrote

char line[MAXLINE];  // global, not explicitly initialized

void print_line(void) {
    printf("%s\n", line);  // called before get_line
}

What I thought

The program works by accident because line happens to contain an empty string.

What actually happens

The program has a valid initial state defined by the language. line is in .bss, so all bytes are zero. line[0] = '\0' is guaranteed. The string is empty by definition, not by accident. The problem is not that the value is undefined — it is that the empty string does not represent meaningful program data.

Fix

The correct statement: "The program has a valid initial state, but that state does not represent meaningful program data. Reading a global before writing to it produces a valid empty string, not garbage, but the semantics are wrong."

# Problem 3 — Moving the condition into a function without restricting visibility

What I wrote

void update_longest_if_longer(int current_len) {
    if (current_len > max) {
        max = current_len;
        copy();
    }
}

What I thought

This solves the problem because now only update_longest_if_longer calls copy.

What actually happens

copy is still visible. Any function can call copy() directly and corrupt longest by copying a shorter line or uninitialized data. Moving the condition does nothing if copy remains public.

Fix

Make copy static:

static void copy(void) {
    int i = 0;
    while ((longest[i] = line[i]) != '\0') {
        i++;
    }
}

copy should not modify line. Its responsibility is to copy, not to alter the source. Invalidating line would introduce hidden state.

Now copy is invisible outside the translation unit. The only way to modify longest is through update_longest_if_longer. The protocol becomes enforceable, not just documented.

# Problem 4 — Blaming the wrong variable for test interference

What I wrote
void test_get_line(void) {
    // simulate input "hello\n"
    assert(get_line() == 5);
    assert(strcmp(line, "hello") == 0);
}

void test_empty_line(void) {
    // simulate input "\n"
    assert(get_line() == 0);
}

What I thought

line retains its previous content and causes test interference.

What actually happens

get_line always writes a new string and appends '\0'. line is safely overwritten each call. The real problem is max and longest — they persist across tests. If test 1 finds a line of length 5, max = 5 and longest = "hello". Test 2 runs with max still 5 and longest still "hello", even though it only read an empty line. The tests are not isolated.

Fix

The correct statement: "line is safely overwritten each call, but max and longest persist across tests. This breaks test isolation unless globals are explicitly reset."

# Problem 5 — Explaining extern without mentioning the linker

What I wrote

extern only declares that the variable exists somewhere else.

What actually happens

This is correct but incomplete. extern tells the compiler "this symbol is defined in another translation unit". The compiler generates an unresolved reference. The linker resolves it at link time, matching the declaration to the single definition across all object files.

Fix

Add: "The linker resolves the symbol across translation units. If no definition exists, linking fails with 'undefined reference'. If multiple definitions exist, the linker may emit a multiple definition error, or in older toolchains, may merge them in undefined ways."

# Problem 6 — Framing the multiple-writer problem as a threading issue

What I wrote

If the program is extended to threads later, line contains interleaved or overwritten data.

What actually happens

The problem is not threads. The problem is multiple writers without coordination. Threads make the problem observable as a race condition, but the design flaw exists even in single-threaded code. Two functions writing to line sequentially still produce corrupted data if called in the wrong order. Threads only add concurrency.

Fix

The correct statement: "The design allows multiple writers without coordination. In single-threaded code, the corruption depends on call order. In multi-threaded code, it becomes a nondeterministic race condition. The root cause is not threading — it is the absence of ownership."

Hidden invariant

line[] must not be modified between read_next_line() and commit_to_longest()
and must be null-terminated at all times.

This is the most important implicit contract in the program. No function signature reveals it. No type enforces it. The entire correctness depends on every programmer knowing and respecting it. This is what "invisible invariants" means.

Classification of invariants

These invariants are:

· temporal — ordering of function calls (read_next_line before commit_to_longest)
· structural — line must be null-terminated at all times
· ownership-related — only specific functions may write to each global

C cannot express any of these in its type system.

Summary of corrections

Problem Correction
1 .bss placement with modern toolchains, no "common" ambiguity
2 Valid initial state ≠ meaningful state
3 static visibility; removed invalid line[0] = '\0'
4 Test isolation requires explicit reset (not "impossible")
5 Linker behavior with multiple definitions
6 Multiple writers, not threads
