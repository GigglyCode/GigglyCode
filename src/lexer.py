from tokens import Token, TokenType


class Lexer:
    def __init__(self, source: str) -> None:
        self.tokens: list[Token] = []
        self.source = source
        self.pos = 0
        self.current_char: str = ""
        self.line = 1
        self.column = 0
        self._read_char()
        while self.current_char != "":
            self.tokens.append(self.next_token())

    def next_token(self) -> Token:
        tok: Token
        self._skip_blanks()
        match self.current_char:
            case "":
                tok = Token(TokenType.EOF, "", self.line, self.column, self.column)
            case "+":
                if self._peek_char() == "=":
                    tok = Token(TokenType.PlusEqual, "+=", self.line, self.column, self.column + 1)
                    self.pos += 1
                elif self._peek_char() == "+":
                    tok = Token(TokenType.Increment, "++", self.line, self.column, self.column + 1)
                else:
                    tok = Token(TokenType.Plus, "+", self.line, self.column, self.column)
            case "-":
                if self._peek_char() == "=":
                    tok = Token(TokenType.DashEqual, "-=", self.line, self.column, self.column + 1)
                    self.pos += 1
                elif self._peek_char() == "-":
                    tok = Token(TokenType.Decrement, "--", self.line, self.column, self.column + 1)
                elif self._peek_char() == ">":
                    tok = Token(TokenType.RightArrow, "->", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.Dash, "-", self.line, self.column, self.column)
            case "*":
                if self._peek_char() == "*":
                    tok = Token(TokenType.AsteriskAsterisk, "**", self.line, self.column, self.column + 1)
                    self.pos += 1
                elif self._peek_char() == "=":
                    tok = Token(TokenType.AsteriskEqual, "*=", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.Asterisk, "*", self.line, self.column, self.column)
            case "%":
                if self._peek_char() == "=":
                    tok = Token(TokenType.PercentEqual, "%=", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.Percent, "%", self.line, self.column, self.column)
            case "/":
                if self._peek_char() == "=":
                    tok = Token(TokenType.ForwardSlashEqual, "/=", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.ForwardSlash, "/", self.line, self.column, self.column)
            case "\\":
                if self._peek_char() == "=":
                    tok = Token(TokenType.BackwardSlashEqual, "\\=", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.BackwardSlash, "\\", self.line, self.column, self.column)
            case "=":
                if self._peek_char() == "=":
                    tok = Token(TokenType.EQ, "==", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.Equals, "=", self.line, self.column, self.column)
            case ".":
                if self._peek_char() == "." and self._peek_char(2) == ".":
                    tok = Token(TokenType.Ellipsis, "...", self.line, self.column, self.column + 2)
                    self.pos += 2
                else:
                    tok = Token(TokenType.Dot, ".", self.line, self.column, self.column)
            case "!":
                if self._peek_char() == "=":
                    tok = Token(TokenType.NOT_EQ, "!=", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.ILLEGAL, self.current_char, self.line, self.column, self.column)
            case "<":
                if self._peek_char() == "=":
                    tok = Token(TokenType.LTE, "<=", self.line, self.column, self.column + 1)
                    self.pos += 1
                elif self._peek_char() == "<":
                    tok = Token(TokenType.LeftShift, "<<", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.LT, "<", self.line, self.column, self.column)
            case ">":
                if self._peek_char() == "=":
                    tok = Token(TokenType.GTE, ">=", self.line, self.column, self.column + 1)
                    self.pos += 1
                elif self._peek_char() == ">":
                    tok = Token(TokenType.RightShift, ">>", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.GT, ">", self.line, self.column, self.column)
            case "^":
                tok = Token(TokenType.BitwiseXor, "^", self.line, self.column, self.column)
            case "~":
                tok = Token(TokenType.BitwiseNot, "~", self.line, self.column, self.column)
            case "&":
                if self._peek_char() == "&":
                    tok = Token(TokenType.BitwiseAnd, "&&", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.ILLEGAL, self.current_char, self.line, self.column, self.column)
            case "|":
                if self._peek_char() == "|":
                    tok = Token(TokenType.BitwiseOr, "||", self.line, self.column, self.column + 1)
                    self.pos += 1
                else:
                    tok = Token(TokenType.ILLEGAL, self.current_char, self.line, self.column, self.column)
            case "(":
                tok = Token(TokenType.LeftParen, "(", self.line, self.column, self.column)
            case ")":
                tok = Token(TokenType.RightParen, ")", self.line, self.column, self.column)
            case "{":
                tok = Token(TokenType.LeftBrace, "{", self.line, self.column, self.column)
            case "}":
                tok = Token(TokenType.RightBrace, "}", self.line, self.column, self.column)
            case "[":
                tok = Token(TokenType.LeftBracket, "[", self.line, self.column, self.column)
            case "]":
                tok = Token(TokenType.RightBracket, "]", self.line, self.column, self.column)
            case ":":
                tok = Token(TokenType.Colon, ":", self.line, self.column, self.column)
            case ";":
                tok = Token(TokenType.Semicolon, ";", self.line, self.column, self.column)
            case ",":
                tok = Token(TokenType.Comma, ",", self.line, self.column, self.column)
            case _:
                if self.current_char.isdigit():
                    st_col = self.column
                    num, float_ = self._read_num()
                    if float_:
                        tok = Token(TokenType.Float, num, self.line, st_col, self.column)
                    else:
                        tok = Token(TokenType.Int, num, self.line, st_col, self.column)
                    return tok
                elif self.current_char.isalpha() or self.current_char == "_":
                    st_col = self.column
                    ident = self.current_char
                    self._read_char()
                    while self.current_char.isalnum() or self.current_char == "_":
                        ident += self.current_char
                        self._read_char()
                    match ident:
                        case "is":
                            tok = Token(TokenType.Is, ident, self.line, st_col, self.column - 1)
                        case "and":
                            tok = Token(TokenType.And, ident, self.line, st_col, self.column - 1)
                        case "or":
                            tok = Token(TokenType.Or, ident, self.line, st_col, self.column - 1)
                        case "not":
                            tok = Token(TokenType.Not, ident, self.line, st_col, self.column - 1)
                        case "def":
                            tok = Token(TokenType.Def, ident, self.line, st_col, self.column - 1)
                        case "return":
                            tok = Token(TokenType.Return, ident, self.line, st_col, self.column - 1)
                        case "if":
                            tok = Token(TokenType.If, ident, self.line, st_col, self.column - 1)
                        case "else":
                            tok = Token(TokenType.Else, ident, self.line, st_col, self.column - 1)
                        case "elIf":
                            tok = Token(TokenType.ElIf, ident, self.line, st_col, self.column - 1)
                        case "while":
                            tok = Token(TokenType.While, ident, self.line, st_col, self.column - 1)
                        case "for":
                            tok = Token(TokenType.For, ident, self.line, st_col, self.column - 1)
                        case "in":
                            tok = Token(TokenType.In, ident, self.line, st_col, self.column - 1)
                        case "break":
                            tok = Token(TokenType.Break, ident, self.line, st_col, self.column - 1)
                        case "continue":
                            tok = Token(TokenType.Continue, ident, self.line, st_col, self.column - 1)
                        case "class":
                            tok = Token(TokenType.Class, ident, self.line, st_col, self.column - 1)
                        case "true":
                            tok = Token(TokenType.True_, ident, self.line, st_col, self.column - 1)
                        case "false":
                            tok = Token(TokenType.False_, ident, self.line, st_col, self.column - 1)
                        case "none":
                            tok = Token(TokenType.None_, ident, self.line, st_col, self.column - 1)
                        case _:
                            tok = Token(TokenType.Identifier, ident, self.line, st_col, self.column - 1)
                    return tok
                elif self.current_char == "\"":
                    st_col = self.column
                    self._read_char()
                    string = ""
                    while self.current_char != "\"":
                        string += self.current_char
                        self._read_char()
                    tok = Token(TokenType.String, string, self.line, st_col, self.column)
                elif self.current_char == "'":
                    st_col = self.column
                    self._read_char()
                    raw_string = ""
                    while self.current_char != "'":
                        raw_string += self.current_char
                        self._read_char()
                    tok = Token(TokenType.RawString, raw_string, self.line, st_col, self.column)
                else:
                    tok = Token(TokenType.ILLEGAL, self.current_char, self.line, self.column, self.column)
        self._read_char()
        return tok
    
    def _read_num(self) -> tuple[str, bool]:
        point = False
        num = ""
        while (self.current_char.isdigit() or self.current_char == ".") and self.current_char != "":
            if self.current_char == ".":
                point = True
            num += self.current_char
            self._read_char()
        return num, point

    def _read_char(self) -> None:
        if self.pos >= len(self.source):
            self.current_char = ""
        else:
            self.current_char = self.source[self.pos]
        self.pos += 1
        self.column += 1

    def _peek_char(self, n: int = 1) -> str:
        if self.pos + n >= len(self.source):
            return ""
        return self.source[self.pos - 1 + n]

    def _skip_blanks(self) -> None:
        while self.current_char.isspace():
            if self.current_char == "\n":
                self.line += 1
                self.column = 0
            self._read_char()


def test_all_possible_tokens():
    from rich import print
    # Sample input text containing all possible tokens
    input_text = """
if (x == 10) {
    print("Hello, World!");
} else {
    x = x + 1;
}
while (y != 20) {
    y -= 2;
}
for (int i = 0; i < 10; i++) {
    continue;
}
return x / y;
x += 1;
x -= 1;
x *= 1;
x /= 1;
x %= 1;
x \\= 1;
x && y;
x || y;
x ^ y;
~x;
x << 1;
x >> 1;
x++;
x--;
x.is(y);
x.in(y);
def func():
    pass
class MyClass:
    pass
True
False
None
"""
    
    # Expected tokens (adjust based on your lexer implementation)
    expected_tokens = [
    ]
    
    # Initialize the lexer
    lexer = Lexer(input_text)
    
    # Tokenize the input text
    tokens = lexer.tokens
    print(tokens)
    # Assert the tokens
    # assert tokens == expected_tokens

if __name__ == "__main__":
    test_all_possible_tokens()