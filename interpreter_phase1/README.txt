Interpreter Phase 3
Team Members:
*Julian Contreras
*Jonas Setzer

Contributions:
*Julian Contreras:
- Implemented ValueDescriptor class and subclasses {IntegerValueDescriptor, DoubleValueDescriptor, BooleanValueDescriptor, StringValueDescriptor}
- Changed BinaryExprNode to return ValueDescriptors instead of ints
- Implemented Stringconversion
- Modified Token to account for str and //
- Worked on getToken in the Tokenizer

*Jonas Setzer:
- Added new literal recognition to Tokenizer
- Updated int values to int64_t in original integer token
- Changed UnaryExprNode to return ValueDescriptors instead of ints
- Updated range() function to accept ValueDescriptors and evaluate to ints
- Modified ArithAtom to recognize and handle new literals

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
