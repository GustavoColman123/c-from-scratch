# Exercise 1 - File Copying with getchar()

# Problem
This program reads characters one at a time from stdin and writes
them to stdout until there is no more input. It looks simple, but
it is demonstrating something deeper: how C handles input as a
stream of bytes, and how EOF works as a signal rather than a
real character. This program represents the minimal model of
input/output in C, where data is treated as a stream of bytes
and EOF is the only termination signal.

# Core Concept
The program never opens a file. It never knows where the input
comes from. It only talks to stdin and stdout, which are streams —
abstract interfaces that can be connected to a keyboard, a file,
a pipe, or a device. The runtime and operating system together
establish the connection between the program and the underlying
data source. The program just reads bytes and writes bytes.

This is the Unix philosophy: everything is a stream of bytes.
Programs should not care about the source.

# My Approach
Exact execution flow:

1. User types something → goes to the OS input buffer
   getchar() blocks and waits until Enter is pressed

2. In interactive terminals, input is typically line-buffered,
   meaning data becomes available after a newline is entered

3. Inside the while condition:
   - c = getchar()     reads one byte, assigns it to int c
   - c != EOF          compares the result against -1
   - if true  → putchar(c) runs
   - if false → loop exits

4. putchar(c) writes to the stdout buffer
   output may be buffered depending on the environment

5. In interactive terminals, stdout is often line-buffered,
   so a newline may trigger a flush, but this is not guaranteed
   by the C standard

6. Loop repeats from step 3

7. User presses Ctrl+D (Linux) or Ctrl+Z (Windows)
   getchar() returns -1
   condition fails, loop exits cleanly

What changes with different input sources:

   keyboard → EOF arrives when user presses Ctrl+D
   file → EOF arrives when file ends, automatically
   pipe → EOF arrives when the sending process closes
                   its end of the pipe

The program code is identical in all three cases.

# Key Insights

**Why getchar() returns int instead of char**
Because it needs to return two distinct things: any valid byte
(0 to 255) AND the EOF signal (-1). A char cannot hold all three
ranges without collision. int has enough range for both.

**Why EOF is not a character**
EOF is just the integer -1, defined in stdio.h. EOF is a special
return value defined by the C standard library, not part of the
input data stream. It is not a byte that exists in the input.
It is a return value that signals "there is nothing left to read".
No file ever contains EOF as actual content.

**Why the order inside the while condition matters**
```c
while ((c = getchar()) != EOF)
```
The inner parentheses force assignment before comparison.
getchar() runs, the result goes into c, then c is compared
against EOF. If you remove the parentheses, the comparison
happens first and c gets 0 or 1 instead of the actual byte.

If you move getchar() inside the loop body:
```c
while (c != EOF) {
    c = getchar();
    putchar(c);      // executes even when c IS EOF
}
```
This is wrong. putchar() runs once with EOF before the loop
can check the condition again.

# Limitations

**Read errors**
getchar() returns EOF for both end-of-input and read errors.
The program cannot tell which one happened without calling
ferror(stdin) after the loop.

**Write errors**
putchar() can fail silently (disk full, broken pipe).
The program never checks its return value.

**feof vs ferror**
```c
if (feof(stdin))    // input ended normally
if (ferror(stdin))  // something went wrong
```
The program checks neither. It just stops.

**Binary mode on some systems**
On Linux, text and binary mode are the same. In text mode on
some systems (historically DOS/Windows), transformations may
occur that interfere with binary data, making this program
unreliable for raw binary streams.

**Infinite streams**
```bash
yes | ./program
```
yes generates "y\n" forever and never closes the pipe.
getchar() never receives EOF. The loop runs forever.
The program is not broken — it just has no way to stop
when the input has no end.

## Experiments
# basic keyboard input, Ctrl+D to stop
./program
→ expected: echoes user input until EOF is sent (Ctrl+D / Ctrl+Z)

# redirecting a text file
./program < file.txt
→ expected: exact copy of file contents to stdout

# input from another program via pipe
echo hello | ./program
→ expected: outputs "hello" followed by newline

# infinite stream, must kill with Ctrl+C
yes | ./program
→ expected: infinite output until externally terminated (Ctrl+C)

# binary file, check if raw bytes are handled correctly
./program < binaryfile.bin
→ expected: copies raw bytes, but may fail on systems with
            text mode transformations
