#ifndef TOKENS_HPP
#define TOKENS_HPP
#include <string>
#include <iostream>
#include <memory>
namespace token
{
    enum class tokenType
    {
        // Special Tokens
        EndOfFile,
        Illegal,

        // Comparison Operators
        GreaterThan,        // Greater than >
        LessThan,           // Less than <
        GreaterThanOrEqual, // Greater than or equal to >=
        LessThanOrEqual,    // Less than or equal to <=
        EqualEqual,         // Equal to ==
        NotEquals,          // Not equal to !=

        // Data Types
        Identifier, // Identifier token [a-zA-Z_][a-zA-Z0-9_]*
        Integer,    // Integer token [0-9]+
        Float,      // Float token [0-9]+.[0-9]+
        String,     // String token ".*"

        // Assignment Operators
        PlusEqual,          // Addition assignment +=
        DashEqual,          // Subtraction assignment -=
        AsteriskEqual,      // Multiplication assignment *=
        PercentEqual,       // Modulus assignment %=
        CaretEqual,         // Exponentiation assignment ^=
        ForwardSlashEqual,  // Division assignment /=
        BackwardSlashEqual, // Division assignment \=
        Equals,             // Equals sign =
        Is,                 // Is sign is

        // Increment and Decrement Operators
        Increment, // Increment ++
        Decrement, // Decrement --

        // Bitwise Operators
        BitwiseAnd, // Bitwise AND &&
        BitwiseOr,  // Bitwise OR ||
        BitwiseXor, // Bitwise XOR ^
        BitwiseNot, // Bitwise NOT ~
        LeftShift,  // Left shift <<
        RightShift, // Right shift >>

        // Arithmetic Operators
        Dot,              // Dot .
        Ellipsis,         // Ellipsis ...
        Plus,             // Addition +
        Dash,             // Subtraction -
        Asterisk,         // Multiplication *
        Percent,          // Modulus %
        AsteriskAsterisk, // Exponentiation **
        ForwardSlash,     // Division /
        BackwardSlash,    //

        // Symbols
        LeftParen,    // Left Parenthesis (
        RightParen,   // Right Parenthesis )
        LeftBrace,    // Left Brace {
        RightBrace,   // Right Brace }
        LeftBracket,  // Left Bracket [
        RightBracket, // Right Bracket ]
        Colon,        // Colon :
        Semicolon,    // Semicolon ;
        RightArrow,   // Right Arrow ->
        Comma,        // Comma ,

        // Keywords
        And,      // Logical AND and
        Or,       // Logical OR or
        Not,      // Logical NOT not
        Var,      // Variable declaration var
        Def,      // Function definition def
        Return,   // Return statement return
        If,       // If statement if
        Else,     // Else statement else
        ElIf,     // Else If statement elif
        While,    // While statement while
        For,      // For statement for
        In,       // In statement in
        Break,    // Break statement break
        Continue, // Continue statement continue
        True,     // Boolean true true
        False,    // Boolean false false
        Maybe,    // Maybe type maybe
        None,     // None type none
    };

    std::string tokenTypeString(tokenType type);

    class Token
    {
    public:
        tokenType type;
        std::string literal;
        int line_no;
        int col_no;
        int end_col_no;

        Token(){};
        Token(tokenType type, int line_no, int col_no)
            : type(type), line_no(line_no), end_col_no(col_no){};
        Token(tokenType type, std::string literal, int line_no, int col_no)
            : type(type), literal(literal), line_no(line_no), end_col_no(col_no)
        {
            this->col_no = end_col_no - literal.length();
        };
        std::string toString();
        void print();
    };
} // namespace token
#endif // TOKENS_HPP
