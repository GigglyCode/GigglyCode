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
        LessThan,           // Less than
        GreaterThanOrEqual, // Greater than or equal to
        LessThanOrEqual,    // Less than or equal to
        EqualEqual,         // Equal to
        NotEquals,          // Not equal to

        // Data Types
        Identifier,
        Integer,
        Float,

        // Assignment Operators
        PlusEqual,          // Addition assignment
        DashEqual,          // Subtraction assignment
        AsteriskEqual,      // Multiplication assignment
        PercentEqual,       // Modulus assignment
        CaretEqual,         // Exponentiation assignment
        ForwardSlashEqual,  // Division assignment
        BackwardSlashEqual, // Division assignment
        Equals,             // Equals sign

        // Increment and Decrement Operators
        Increment, // Increment
        Decrement, // Decrement

        // Arithmetic Operators
        Plus,         // Addition
        Dash,         // Subtraction
        Asterisk,     // Multiplication
        Percent,      // Modulus
        Caret,        // Exponentiation
        ForwardSlash, // Division
        BackwardSlash,

        // Symbols
        LeftParen,  // Left Parenthesis
        RightParen, // Right Parenthesis
        LeftBrace,  // Left Brace
        RightBrace, // Right Brace
        Colon,      // Colon
        Semicolon,  // Semicolon
        RightArrow, // Right Arrow (e.g., in function return type specification)

        // Keywords
        Var,    // Variable declaration
        Def,    // Function definition
        Return, // Return statement

        // Comment
        Comment // Comment token
    };

    std::string tokenTypeString(tokenType type);

    class Token
    {
    public:
        tokenType type;
        std::string literal;
        int line_no;
        int col_no;

        Token(){};
        Token(tokenType type, int line_no, int col_no)
            : type(type), line_no(line_no), col_no(col_no){};
        Token(tokenType type, std::string literal, int line_no, int col_no)
            : type(type), literal(literal), line_no(line_no), col_no(col_no){};
        std::string toString();
        void print();
    };
} // namespace token
#endif // TOKENS_HPP
