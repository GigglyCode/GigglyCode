# GigglyCode
A programing language, created by SohamTilekar for learning C++. The language is currently in the lexer stage of development.

# Goal
The goal of this programming language is to allow inter-operability between languages by having support inlining Python and C code (other programming languages might be supported as well).

# Syntax
Syntax is very similar to Python's, except for using curly braces instead of indentation.
This example should give a taste on what it should look like:
```python
def fizzbuzz(start, stop, step) {
  for i in range(start, stop, step) {
    if i % 15 == 0 {
      print('fizzbuzz');
    } else if i % 5 == 0 {
      print('buzz');
    } else if i % 3 == 0 {
      print('fizz');
    } else {
      print(i);
    }
  }
}
```
