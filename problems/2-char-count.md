## Problems & confusions

**1. I kept writing c = getchar() by habit**

My first instinct was to store the result before comparing.
Took me a moment to accept that the comparison inside while() is enough.
Not sure why it felt wrong to "use" a value without saving it first.

**2. I wasn't sure if \n counted**

I assumed EOF was the only special character and that \n might be
handled differently by the terminal or getchar().
Tested it. It counts. \n is just another character to getchar().

**3. ++nc vs nc++ — I thought it mattered here**

I remembered that pre and post increment behave differently and
got stuck trying to figure out which one was "correct" for this case.
Eventually realized the result of the expression is never used,
so both do exactly the same thing in this context.
The distinction only matters when you use the value in the same line.
