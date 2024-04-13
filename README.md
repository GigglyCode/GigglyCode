# GigglyCode
A programing language, created by SohamTilekar for learning C++.

# Goal
The goal of this programming language is to allow inter-operability between languages by having support inlining Python and C code (other programming languages might be supported as well).

# Syntax
Syntax is very similar to Python's, except for using curly braces instead of indentation.
This example should give a taste on what it should look like:
```
def fizzbuzz(start, stop, step) {
  for i in range(start, stop, step) {
    if i % 15 == 0 {
      print('fizzbuzz');
    } elif i % 5 == 0 {
      print('buzz');
    } elif i % 3 == 0 {
      print('fizz');
    } else {
      print(i);
    }
  }
}
```
To use the very novel feature of GigglyCode called "inlining", start with `#inline` directive and end with `#end`. For example:
```
#inline python 3.12 exports add

def add(a, b):
  return a + b

#end

print(add(1, 2)) // 3
```

`<WIP>`
