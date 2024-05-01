#include "include/tokens.hpp"

std::string token::Token::toString()
{
    // Define ANSI escape codes for colors
    static const std::string colorReset = "\x1b[0m";
    static const std::string colorBlue = "\x1b[34m";
    static const std::string colorYellow = "\x1b[33m";
    static const std::string colorGreen = "\x1b[32m";
    static const std::string colorMagenta = "\x1b[35m";

    // Convert variables to strings
    std::string typeString = tokenTypeString(type);
    std::string literalString = "\"" + literal + "\"";
    std::string lineNoString = std::to_string(line_no);
    std::string colNoString = std::to_string(col_no);

    // Apply padding to each field for alignment
    typeString += std::string((literalString.length() > 10) ? 0 : 10 - literalString.length(), ' ');
    literalString += std::string((literalString.length() > 5) ? 0 : 5 - literalString.length(), ' ');
    lineNoString += std::string((literalString.length() > 2) ? 0 : 2 - literalString.length(), ' ');
    colNoString += std::string((literalString.length() > 2) ? 0 : 2 - literalString.length(), ' ');
    // Construct the formatted string with colors
    return "Token{type: " + colorBlue + typeString + colorReset +
           ", literal: " + colorYellow + literalString + colorReset +
           ", line_no: " + colorGreen + lineNoString + colorReset +
           ", col_no: " + colorMagenta + colNoString + colorReset + "}";
}

std::string token::tokenTypeString(tokenType type)
{
    switch (type)
    {
    case tokenType::Identifier:
        return "Identifier";
    case tokenType::Integer:
        return "INT";
    case tokenType::Float:
        return "Float";
    case tokenType::PlusEqual:
        return "PlusEqual";
    case tokenType::DashEqual:
        return "DashEqual";
    case tokenType::AsteriskEqual:
        return "AsteriskEqual";
    case tokenType::PercentEqual:
        return "PercentEqual";
    case tokenType::CaretEqual:
        return "CaretEqual";
    case tokenType::ForwardSlashEqual:
        return "ForwardSlashEqual";
    case tokenType::BackwardSlashEqual:
        return "BackwardSlashEqual";
    case tokenType::Increment:
        return "Increment";
    case tokenType::Decrement:
        return "Decrement";
    case tokenType::Plus:
        return "Plus";
    case tokenType::Dash:
        return "Dash";
    case tokenType::Asterisk:
        return "Asterisk";
    case tokenType::Percent:
        return "Percent";
    case tokenType::Caret:
        return "Caret";
    case tokenType::ForwardSlash:
        return "ForwardSlash";
    case tokenType::BackwardSlash:
        return "BackwardSlash";
    case tokenType::LeftParen:
        return "LeftParen";
    case tokenType::RightParen:
        return "RightParen";
    case tokenType::LeftBrace:
        return "LeftBrace";
    case tokenType::RightBrace:
        return "RightBrace";
    case tokenType::Colon:
        return "Colon";
    case tokenType::Semicolon:
        return "Semicolon";
    case tokenType::RightArrow:
        return "RightArrow";
    case tokenType::Equals:
        return "Equals";
    case tokenType::Illegal:
        return "Illegal";
    case tokenType::EndOfFile:
        return "EndOfFile";
    case tokenType::Var:
        return "Var";
    case tokenType::Def:
        return "Def";
    case tokenType::Return:
        return "Return";
    case tokenType::GreaterThan:
        return "GreaterThan";
    case tokenType::LessThan:
        return "LessThan";
    case tokenType::GreaterThanOrEqual:
        return "GreaterThanOrEqual";
    case tokenType::LessThanOrEqual:
        return "LessThanOrEqual";
    case tokenType::EqualEqual:
        return "EqualEqual";
    case tokenType::NotEquals:
        return "NotEquals";
    default:
        return "unknown token type(" + std::to_string(static_cast<int>(type)) + ")";
    }
}

void token::Token::print()
{
    std::cout << toString() << std::endl;
}
