# Problem 6 — Digit Histogram

## Mistake 1 — Ignoring scaling

**Issue:** Printing raw frequencies produced excessively long bars.

**Fix:** Introduced proportional scaling using a maximum width.

## Mistake 2 — Division by zero

**Issue:** If no digits were present, `max` was zero, leading to
a potential division by zero during scaling.

**Fix:** Applied scaling only when `max > 0`.

## Mistake 3 — Misalignment of output

**Issue:** Without consistent formatting, the histogram was difficult
to read.

**Fix:** Added labels and aligned output using formatted printing.
