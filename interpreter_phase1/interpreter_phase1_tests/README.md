# Phase 1 Sample Tests

Every `.txt` file in `valid/` is a complete program that should be accepted
and evaluated. Every file ends with the required newline before `EOF`.

The values below are the values that `print` statements should print, in the order they appear. However, each `print rel-expr` statement should print a single line. In the table below, they have been printed one after another to make the table shorter.

| Input file | Main purpose | Expected printed values |
|---|---|---|
| `valid/sum-accumulator.txt` | Loop counter and accumulator | `55` |
| `valid/sum-nested-loops.txt` | Nested loops; repeated addition of one | `55` |
| `valid/fibonacci.txt` | Multiple assignments per iteration; computes `F(10)` with `F(0) = 0` and `F(1) = 1` | `55` |
| `valid/factorial.txt` | Multiplication in a loop | `720` |
| `valid/greatest-common-divisor.txt` | `!=`, modulo, and a data-dependent loop | `21` |
| `valid/prime-test.txt` | Modulo, comparison values, and a divisor search; tests whether `29` is prime | `1` |
| `valid/comparisons-and-precedence.txt` | All relational operators and both relational/arithmetic precedence levels | `1 1 1 1 1 1 0 0 1 0 1` |
| `valid/zero-iteration-loop.txt` | Condition checked before the body | `27` |
| `valid/arithmetic-regression.txt` | Unary signs, parentheses, multiplication, division, and modulo | `7 -20 2 5 7 12` |

The files in `invalid/` should be rejected rather than evaluated:

| Input file | Reason it is invalid |
|---|---|
| `invalid/chained-comparison.txt` | Relational operators are optional, not repeatable. |
| `invalid/repeated-sign.txt` | The grammar permits at most one unary sign. |
| `invalid/empty-loop-body.txt` | A loop body must contain at least one statement. |
| `invalid/statement-after-opening-brace.txt` | `NEWLINE` is required immediately after `{`. |
| `invalid/lone-exclamation-mark.txt` | `!` is not an operator; only `!=` is valid. |

For example, from the project directory:

```console
./interpreter.x tests/valid/sum-accumulator.txt
./interpreter.x tests/invalid/chained-comparison.txt
```

These are representative samples, not an exhaustive test suite. Additional
tests should check alternate values, boundary cases, malformed loop headers,
missing delimiters, and the required `NEWLINE` before `EOF`.
