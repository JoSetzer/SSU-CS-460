Interpreter Phase 4
Team Members:
*Julian Contreras
*Jonas Setzer

Contributions:
*Julian Contreras:
- if, elif, else statements
- while loops
- int and float conversions
- Partial len and split implementation for strings 

*Jonas Setzer:
- list literal implementation
- pop, append, getelement, setelement
- finalize split and len functions
- list equality and inequality

AI Use:
* No AI was used at any stage in development

Building and Running

Project must be built from the command line with:

make

This command produces an executable named interpreter.x. The interpreter must accept exactly one command-line argument: the name of the input file.

For example:

./interpreter.x tests/basic-loop.txt

Run to check for memory leaks

valgrind --leak-check=full ./interpreter.x tests/basic-loop.txt
