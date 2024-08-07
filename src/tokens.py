import enum

class TokenType(enum.Enum):
    EOF = "EOF"
    ILLEGAL = "ILLEGAL"
    
    # Comparisons
    EQ = "EQ"
    NOT_EQ = "NOT_EQ"
    LT = "LT"
    GT = "GT"
    LTE = "LTE"
    GTE = "GTE"

    Identifier = "Identifier"
    Int = "Int"
    Float = "Float"
    String = "String"
    RawString = "RawString"
    
    # assignment operators
    PlusEqual = "PlusEqual"                   # Addition assignment +=
    DashEqual = "DashEqual"                   # Subtraction assignment -=
    AsteriskEqual = "AsteriskEqual"           # Multiplication assignment *=
    PercentEqual = "PercentEqual"             # Modulus assignment %=
    CaretEqual = "CaretEqual"                 # Exponentiation assignment ^=
    ForwardSlashEqual = "ForwardSlashEqual"   # Division assignment /=
    BackwardSlashEqual = "BackwardSlashEqual" # Division assignment \=
    Equals = "Equals"                         # Equals sign =
    Is = "Is"                                 # Is sign is

    # Increment and Decrement Operators
    Increment = "Increment"  # Increment ++
    Decrement = "Decrement"  # Decrement --

    # Bitwise Operators
    BitwiseAnd = "BitwiseAnd"   # Bitwise AND &&
    BitwiseOr = "BitwiseOr"     # Bitwise OR ||
    BitwiseXor = "BitwiseXor"   # Bitwise XOR ^
    BitwiseNot = "BitwiseNot"   # Bitwise NOT ~
    LeftShift = "LeftShift"     # Left shift <<
    RightShift = "RightShift"   # Right shift >>

    # Arithmetic Operators
    Dot = "Dot"                 # Dot .
    Ellipsis = "Ellipsis"       # Ellipsis ...
    Plus = "Plus"               # Addition +
    Dash = "Dash"               # Subtraction -
    Asterisk = "Asterisk"       # Multiplication *
    Percent = "Percent"         # Modulus %
    AsteriskAsterisk = "AsteriskAsterisk"   # Exponentiation **
    ForwardSlash = "ForwardSlash"           # Division /
    BackwardSlash = "BackwardSlash"         # Division \

    # Symbols
    LeftParen = "LeftParen"       # Left Parenthesis (
    RightParen = "RightParen"     # Right Parenthesis )
    LeftBrace = "LeftBrace"       # Left Brace {
    RightBrace = "RightBrace"     # Right Brace }
    LeftBracket = "LeftBracket"   # Left Bracket [
    RightBracket = "RightBracket" # Right Bracket ]
    Colon = "Colon"               # Colon :
    Semicolon = "Semicolon"       # Semicolon ;
    RightArrow = "RightArrow"     # Right Arrow ->
    Comma = "Comma"               # Comma ,

    # Keywords
    And = "And"           # Logical AND and
    Or = "Or"             # Logical OR or
    Not = "Not"           # Logical NOT not
    Def = "Def"           # Function definition def
    Return = "Return"     # Return Statement return
    If = "If"             # If Statement if
    Else = "Else"         # Else Statement else
    ElIf = "ElIf"         # Else If Statement elif
    While = "While"       # While Statement while
    For = "For"           # For Statement for
    In = "In"             # In Statement in
    Break = "Break"       # Break Statement break
    Continue = "Continue" # Continue Statement continue
    Class = "Class"       # Class
    True_ = "True"        # Boolean true true
    False_ = "False"      # Boolean false false
    # Maybe = "Maybe"          # Maybe type maybe
    None_ = "None"        # None type none

class Token:
    def __init__(self, type: TokenType, literal: str, line: int, start_column: int, end_column: int):
        self.type: TokenType = type
        self.literal: str = literal
        self.line: int = line
        self.start_column: int = start_column
        self.end_column: int = end_column

    def __str__(self):
        return f"{self.type.name=} {self.literal=} {self.line=} {self.start_column=} {self.end_column=}"
    
    def __repr__(self):
        return self.__str__()