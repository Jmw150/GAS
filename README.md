# GAS

GAS is a small C++ prototype for experimenting with finitely presented groups,
group words, and simple rewriting procedures.

Right now the codebase is intentionally compact and easy to inspect. It models
group words, generators, relations, and group presentations, and it includes a
few algebraic constructions such as commutators, quotients, derived series
steps, and lower central series steps.

This project is best read as an instructional or exploratory algebra system
rather than a finished computer algebra package.

## What It Currently Supports

- `Word` values with concatenation, substring operations, replacement, and
  printing
- `Generator` and `Relation` types for building presentations
- `GroupPresentation` values of the form `<a,b | abAB>`
- Quotients by adjoining new relators
- Word commutators
- Presentation commutators
- Derived subgroup presentation steps
- Lower central series steps
- A small collection of rewrite-system utilities inspired by
  Knuth-Bendix-style completion:
  `delete_trivial_equalities`, `reduce`, `compose`, `simplify`, `orient`,
  `collapse`, and `find_overlaps`

## Current Notation

Generators are currently assumed to be single characters.

- Lowercase letters represent generators such as `a`, `b`, `c`
- Uppercase letters represent formal inverses such as `A = a^-1`
- A presentation like `<a,b | abAB>` is read as
  `<a,b | abAB = 1>`

Whitespace and commas are stripped from `Word` input, so:

```text
"a b, A"
```

is stored internally as:

```text
abA
```

## Important Limitations

This README should be honest about the current mathematical behavior:

- `Generator` names are restricted to one character
- `Word::inverse()` and the free `inverse(const Word&)` function flip case, but
  they do not reverse the order of the word
- `GroupPresentation::reduce_inverses()` removes adjacent inverse pairs in a
  simple pass; it is not a complete normal-form engine
- The rewriting code is useful for experiments, but it is not yet a complete
  or robust Knuth-Bendix implementation
- The code currently uses direct `.cpp` inclusion instead of a more polished
  header/source library layout

Those choices keep the code easy to read, but they also mean the project should
be treated as a teaching and prototyping tool first.

## Building

The project uses a simple `Makefile`.

Build the sample executable:

```bash
make main
```

Run the full test suite:

```bash
make test
```

Clean generated binaries:

```bash
make clean
```

## Tests

The repository currently includes three doctest-based test binaries:

- `tests/test.cpp` for the main algebra and rewrite functionality
- `tests/test_word.cpp` for additional word and presentation checks
- `tests/test_factorial.cpp` as a minimal doctest example

Running `make test` builds and runs all of them.

## Example

The following fragment matches the current style of the code:

```cpp
#include "group_presentation.cpp"

int main() {
    GroupPresentation G(
        set{Generator("a"), Generator("b")},
        set{Relation("abAB")}
    );

    GroupPresentation D = derived(G);
    std::cout << G << "\n";
    std::cout << D << "\n";
}
```

Typical output looks like:

```text
<a,b | abAB>
<a,b | AAaa,ABab,BAba,BBbb>
```

## Project Direction

The code already contains placeholders or partial groundwork for bigger ideas.
Natural next steps include:

- stronger validation of presentations and relators
- a clearer library structure with headers
- fuller Knuth-Bendix completion support
- Tietze transformations
- group actions
- representation-theoretic calculations

## Development Notes

If you are reading the source to learn from it, the most important files are:

- [src/word.cpp](src/word.cpp)
- [src/group_presentation.cpp](src/group_presentation.cpp)
- [tests/test.cpp](tests/test.cpp)

The project is small enough that reading those files in order gives a good
overview of the entire system.
