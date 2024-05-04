import gigly


def name(arg1: type, arg2: type, *args: type, **kwargs: type) -> None {
    print("Hello")
}

x: int = 1
x: float = 1 # Auto Type Conversion
x: int = "2043" # Auto Type Conversion it type hint is different
x: int[] = [1, 2, 3]

if x == 1 {
    print("Hello")
} else {
    print("World")
}

for i in range(10) {
    print(i)
}

class A {
    def __init__(self, x: int) {
        self.x = x
    }

    def __auto__(input: str) -> A {
        return A(int(input))
    }

    def __str__(self) -> str {
        return str(self.x)
    }
}

a: A = "10"
print(a)

```python==3.12 export<function add, sub><class op><var hi>
import gigly

def add(a: int, b: int) -> int:
    return a + b

def sub(a: int, b: int) -> None:
    return None

def hi(a: gigly.type.Union[int, float], ...) -> None:
    print("Hello")

hi: int = 1

class op:
    def __init__(self, a: int, b: int) -> None:
        self.a = a
        self.b = b

    def __add__(self) -> int:
        return self.a + self.b

    def __sub__(self) -> int:
        return self.a - self.b
```