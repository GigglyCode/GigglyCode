#include "./tokens.hpp"
#include <string>
#include <unordered_map>

std::string token::Token::toString(bool color) {
    // Define ANSI escape codes for colors
    const std::string colorReset = "\x1b[0m";
    const std::string colorRed = "\x1b[91m";
    const std::string colorYellow = "\x1b[93m";
    const std::string colorGreen = "\x1b[92m";
    const std::string colorBlue = "\x1b[94m";
    const std::string colorMagenta = "\x1b[95m";

    // Convert variables to strings
    std::string typeString = *tokenTypeString(type);
    std::string literalString = literal;
    std::unordered_map<std::string, std::string> replacements = {
        {"\n", "\\$(n)"}, {"\t", "\\$(t)"}};

    // Replace special characters in literalString
    for (auto &replacement : replacements) {
        size_t pos = literalString.find(replacement.first);
        while (pos != std::string::npos) {
            literalString.replace(pos, 1, replacement.second);
            pos = literalString.find(replacement.first,
                                     pos + replacement.second.size());
        }
    }

    std::string lineNoString = std::to_string(line_no);
    std::string colNoString = std::to_string(col_no);
    std::string endColNoString = std::to_string(end_col_no);

    // Calculate padding for literals
    size_t literalPadding =
        literalString.length() >= 10
            ? 0
            : (10 - literalString.length()) / 2 + (literalString.length() % 2);
    std::string literalPaddingStr(literalPadding, ' ');

    // Apply padding to each field for alignment
    if (typeString.length() < 15)
        typeString += std::string(15 - typeString.length(), ' ');
    if (literalString.length() < 2)
        lineNoString += std::string(2 - lineNoString.length(), ' ');
    if (colNoString.length() < 2)
        colNoString += std::string(2 - colNoString.length(), ' ');
    if (endColNoString.length() < 2)
        endColNoString += std::string(2 - endColNoString.length(), ' ');

    // Construct the formatted string with colors
    return (color ? colorRed : "") +
           "Token{type: " + (color ? colorReset + colorBlue : "") + typeString +
           (color ? colorRed : "") +
           ", literal: " + (color ? colorGreen + "\"" + colorYellow : "") +
           literalPaddingStr + literalString + literalPaddingStr +
           (color ? colorGreen + "\"" : "") +
           ", line_no: " + (color ? colorReset + colorGreen : "") +
           lineNoString + (color ? colorReset : "") +
           ", col_no: " + (color ? colorReset + colorMagenta : "") +
           colNoString + (color ? colorReset : "") +
           ", end_col_no: " + (color ? colorReset + colorMagenta : "") +
           endColNoString + (color ? colorReset : "") +
           (color ? colorRed : "") + "};" + (color ? colorReset : "");
}

std::shared_ptr<std::string> token::tokenTypeString(tokenType type) {
    switch (type) {
    case tokenType::Identifier:
        return std::make_shared<std::string>("Identifier");
    case tokenType::Integer:
        return std::make_shared<std::string>("INT");
    case tokenType::Float:
        return std::make_shared<std::string>("Float");
    case tokenType::PlusEqual:
        return std::make_shared<std::string>("PlusEqual");
    case tokenType::DashEqual:
        return std::make_shared<std::string>("DashEqual");
    case tokenType::AsteriskEqual:
        return std::make_shared<std::string>("AsteriskEqual");
    case tokenType::PercentEqual:
        return std::make_shared<std::string>("PercentEqual");
    case tokenType::CaretEqual:
        return std::make_shared<std::string>("CaretEqual");
    case tokenType::ForwardSlashEqual:
        return std::make_shared<std::string>("ForwardSlashEqual");
    case tokenType::BackwardSlashEqual:
        return std::make_shared<std::string>("BackwardSlashEqual");
    case tokenType::Increment:
        return std::make_shared<std::string>("Increment");
    case tokenType::Decrement:
        return std::make_shared<std::string>("Decrement");
    case tokenType::Dot:
        return std::make_shared<std::string>("Dot");
    case tokenType::Ellipsis:
        return std::make_shared<std::string>("Ellipsis");
    case tokenType::Plus:
        return std::make_shared<std::string>("Plus");
    case tokenType::Dash:
        return std::make_shared<std::string>("Dash");
    case tokenType::Asterisk:
        return std::make_shared<std::string>("Asterisk");
    case tokenType::AsteriskAsterisk:
        return std::make_shared<std::string>("AsteriskAsterisk");
    case tokenType::Percent:
        return std::make_shared<std::string>("Percent");
    case tokenType::ForwardSlash:
        return std::make_shared<std::string>("ForwardSlash");
    case tokenType::BackwardSlash:
        return std::make_shared<std::string>("BackwardSlash");
    case tokenType::LeftParen:
        return std::make_shared<std::string>("LeftParen");
    case tokenType::RightParen:
        return std::make_shared<std::string>("RightParen");
    case tokenType::LeftBrace:
        return std::make_shared<std::string>("LeftBrace");
    case tokenType::RightBrace:
        return std::make_shared<std::string>("RightBrace");
    case tokenType::LeftBracket:
        return std::make_shared<std::string>("LeftBracket");
    case tokenType::RightBracket:
        return std::make_shared<std::string>("RightBracket");
    case tokenType::Colon:
        return std::make_shared<std::string>("Colon");
    case tokenType::Semicolon:
        return std::make_shared<std::string>("Semicolon");
    case tokenType::RightArrow:
        return std::make_shared<std::string>("RightArrow");
    case tokenType::Comma:
        return std::make_shared<std::string>("Comma");
    case tokenType::Equals:
        return std::make_shared<std::string>("Equals");
    case tokenType::Illegal:
        return std::make_shared<std::string>("Illegal");
    case tokenType::EndOfFile:
        return std::make_shared<std::string>("EndOfFile");
    case tokenType::Return:
        return std::make_shared<std::string>("Return");
    case tokenType::GreaterThan:
        return std::make_shared<std::string>("GreaterThan");
    case tokenType::LessThan:
        return std::make_shared<std::string>("LessThan");
    case tokenType::GreaterThanOrEqual:
        return std::make_shared<std::string>("GreaterThanOrEqual");
    case tokenType::LessThanOrEqual:
        return std::make_shared<std::string>("LessThanOrEqual");
    case tokenType::EqualEqual:
        return std::make_shared<std::string>("EqualEqual");
    case tokenType::NotEquals:
        return std::make_shared<std::string>("NotEquals");
    case tokenType::BitwiseAnd:
        return std::make_shared<std::string>("BitwiseAnd");
    case tokenType::BitwiseOr:
        return std::make_shared<std::string>("BitwiseOr");
    case tokenType::BitwiseXor:
        return std::make_shared<std::string>("BitwiseXor");
    case tokenType::BitwiseNot:
        return std::make_shared<std::string>("BitwiseNot");
    case tokenType::LeftShift:
        return std::make_shared<std::string>("LeftShift");
    case tokenType::RightShift:
        return std::make_shared<std::string>("RightShift");
    case tokenType::And:
        return std::make_shared<std::string>("And");
    case tokenType::Or:
        return std::make_shared<std::string>("Or");
    case tokenType::Not:
        return std::make_shared<std::string>("Not");
    case tokenType::Def:
        return std::make_shared<std::string>("Def");
    case tokenType::If:
        return std::make_shared<std::string>("If");
    case tokenType::Else:
        return std::make_shared<std::string>("Else");
    case tokenType::ElIf:
        return std::make_shared<std::string>("ElIf");
    case tokenType::Is:
        return std::make_shared<std::string>("Is");
    case tokenType::While:
        return std::make_shared<std::string>("While");
    case tokenType::For:
        return std::make_shared<std::string>("For");
    case tokenType::In:
        return std::make_shared<std::string>("In");
    case tokenType::Break:
        return std::make_shared<std::string>("Break");
    case tokenType::Continue:
        return std::make_shared<std::string>("Continue");
    case tokenType::True:
        return std::make_shared<std::string>("True");
    case tokenType::False:
        return std::make_shared<std::string>("False");
    case tokenType::Maybe:
        return std::make_shared<std::string>("Maybe");
    case tokenType::None:
        return std::make_shared<std::string>("None");
    case tokenType::String:
        return std::make_shared<std::string>("String");
    default:
        return std::make_shared<std::string>(
            "unknown token type(" + std::to_string(static_cast<int>(type)) +
            ")");
    }
}

void token::Token::print() { std::cout << toString() << std::endl; }
