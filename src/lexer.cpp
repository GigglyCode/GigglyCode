#include <string>

enum TokenTypes
{
    END_OF_FILE,
    NONE,
    ILLEGAL,

    INT,
    FLOAT,

    PLUS,
    MINUS,
    ASTERISK,
    F_SLASH,
    MODULUS,

    LPAREN,
    RPAREN,
};

class Token
{
public:
    TokenTypes type;
    std::string literal;
    int line_number;
    int position;

    Token(TokenTypes type, std::string literal, int line_number, int position)
    {
        this->type = type;
        this->literal = literal;
        this->line_number = line_number;
        this->position = position;
    };

    std::string to_string()
    {
        return "Token(" + std::to_string(this->type) + ", " + this->literal + ", " + std::to_string(this->line_number) + ", " + std::to_string(this->position) + ")";
    };
};

class Lexer
{
public:
    std::string source;
    int position;
    int read_position;
    int line_number;
    char current_char;

    Lexer(std::string source)
    {
        this->source = source;
        this->position = -1;
        this->read_position = 0;
        this->line_number = 1;
        this->current_char = '\0';
        this->read_char();
    };

    Token next_token()
    {
        this->skip_whitespace();

        Token tok = Token(NONE, "", this->line_number, this->position);
        switch (this->current_char)
        {
        case '+':
            tok = new_token(PLUS, std::string(1, this->current_char));
            break;
        case '-':
            tok = new_token(MINUS, std::string(1, this->current_char));
            break;
        case '*':
            tok = new_token(ASTERISK, std::string(1, this->current_char));
            break;
        case '/':
            tok = new_token(F_SLASH, std::string(1, this->current_char));
            break;
        case '%':
            tok = new_token(MODULUS, std::string(1, this->current_char));
            break;
        case '(':
            tok = new_token(LPAREN, std::string(1, this->current_char));
            break;
        case ')':
            tok = new_token(RPAREN, std::string(1, this->current_char));
            break;
        case '\0':
            tok = new_token(END_OF_FILE, "");
            break;
        default:
            if (isdigit(this->current_char))
            {
                tok = this->read_number();
            }
            else
            {
                tok = new_token(ILLEGAL, std::string(1, this->current_char));
            }
            break;
        }

        this->read_char();
        return tok;
    };

private:
    void read_char()
    {
        if (this->read_position >= this->source.length())
        {
            this->current_char = '\0';
        }
        else
        {
            this->current_char = this->source[this->read_position];
        }

        this->position = this->read_position;
        this->read_position += 1;
    };

    void skip_whitespace()
    {
        while (this->current_char == ' ' || this->current_char == '\t' || this->current_char == '\n' || this->current_char == '\r')
        {
            if (this->current_char == '\n')
            {
                this->line_number += 1;
            }
            this->read_char();
        }
    };

    Token new_token(TokenTypes type, std::string literal)
    {
        return Token(type, literal, this->line_number, this->position);
    };
};
