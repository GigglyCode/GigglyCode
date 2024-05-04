#include <string>
#include <unordered_map>
#include "./tokens.hpp"

std::string token::Token::toString()
{
    // Define ANSI escape codes for colors
    static const std::string colorReset = "\x1b[0m";
    static const std::string colorRed = "\x1b[91m";     // Red
    static const std::string colorYellow = "\x1b[93m";  // Yellow
    static const std::string colorGreen = "\x1b[92m";   // Green
    static const std::string colorBlue = "\x1b[94m";    // Blue
    static const std::string colorMagenta = "\x1b[95m"; // Magenta

    // Convert variables to strings
    std::string typeString = tokenTypeString(type);
    std::string literalString = literal;
    static const std::unordered_map<std::string, std::string> replacements = {{"\n", "\\$(n)"}, {"\t", "\\$(t)"}};

    for (const auto &replacement : replacements)
    {
        size_t pos = literalString.find(replacement.first);
        while (pos != std::string::npos)
        {
            literalString.replace(pos, 1, replacement.second);
            pos = literalString.find(replacement.first, pos + replacement.second.size());
        }
    };
    std::string lineNoString = std::to_string(line_no);
    std::string colNoString = std::to_string(col_no);
    std::string endColNoString = std::to_string(end_col_no);

    // Calculate padding for literals
    size_t literalPadding = literalString.length() >= 10 ? 0 : (10 - literalString.length()) / 2 + (literalString.length() % 2);
    std::string literalPaddingStr(literalPadding, ' ');

    // Apply padding to each field for alignment
    if (typeString.length() < 15)
    {
        typeString += std::string(15 - typeString.length(), ' ');
    }
    if (literalString.length() < 2)
    {
        lineNoString += std::string(2 - lineNoString.length(), ' ');
    }
    if (colNoString.length() < 2)
    {
        colNoString += std::string(2 - colNoString.length(), ' ');
    }
    if (endColNoString.length() < 2)
    {
        endColNoString += std::string(2 - endColNoString.length(), ' ');
    }
    // Construct the formatted string with colors
    return colorRed + "Token{type: " + colorReset + colorBlue + typeString + colorReset +
           colorRed + ", literal: " + colorGreen + "\"" + colorYellow + literalPaddingStr + literalString + literalPaddingStr + colorGreen + "\"" +
           colorRed + ", line_no: " + colorReset + colorGreen + lineNoString + colorReset +
           colorRed + ", col_no: " + colorReset + colorMagenta + colNoString + colorReset +
           colorRed + ", end_col_no: " + colorReset + colorMagenta + endColNoString + colorReset +
           colorRed + "};" + colorReset;
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
    case tokenType::Dot:
        return "Dot";
    case tokenType::Ellipsis:
        return "Ellipsis";
    case tokenType::Plus:
        return "Plus";
    case tokenType::Dash:
        return "Dash";
    case tokenType::Asterisk:
        return "Asterisk";
    case tokenType::AsteriskAsterisk:
        return "AsteriskAsterisk";
    case tokenType::Percent:
        return "Percent";
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
    case tokenType::LeftBracket:
        return "LeftBracket";
    case tokenType::RightBracket:
        return "RightBracket";
    case tokenType::Colon:
        return "Colon";
    case tokenType::Semicolon:
        return "Semicolon";
    case tokenType::RightArrow:
        return "RightArrow";
    case tokenType::Comma:
        return "Comma";
    case tokenType::Equals:
        return "Equals";
    case tokenType::Illegal:
        return "Illegal";
    case tokenType::EndOfFile:
        return "EndOfFile";
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
    case tokenType::BitwiseAnd:
        return "BitwiseAnd";
    case tokenType::BitwiseOr:
        return "BitwiseOr";
    case tokenType::BitwiseXor:
        return "BitwiseXor";
    case tokenType::BitwiseNot:
        return "BitwiseNot";
    case tokenType::LeftShift:
        return "LeftShift";
    case tokenType::RightShift:
        return "RightShift";
    case tokenType::And:
        return "And";
    case tokenType::Or:
        return "Or";
    case tokenType::Not:
        return "Not";
    case tokenType::Var:
        return "Var";
    case tokenType::Def:
        return "Def";
    case tokenType::If:
        return "If";
    case tokenType::Else:
        return "Else";
    case tokenType::ElIf:
        return "ElIf";
    case tokenType::Is:
        return "Is";
    case tokenType::While:
        return "While";
    case tokenType::For:
        return "For";
    case tokenType::In:
        return "In";
    case tokenType::Break:
        return "Break";
    case tokenType::Continue:
        return "Continue";
    case tokenType::True:
        return "True";
    case tokenType::False:
        return "False";
    case tokenType::Maybe:
        return "Maybe";
    case tokenType::None:
        return "None";
    case tokenType::String:
        return "String";
    default:
        return "unknown token type(" + std::to_string(static_cast<int>(type)) + ")";
    }
}

void token::Token::print()
{
    std::cout << toString() << std::endl;
}
