#include <cctype>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using std::string;
using std::vector;

enum class TokenType {
  Number,
  StringLit,
  CharLit,
  Preproc,
  Comment,
  Reserved,
  Operator,
  Punct,
  Ident,
  Unknown
};

struct Token {
  string text;
  TokenType type;
};

static const char *typeName(TokenType t) {
  switch (t) {
  case TokenType::Number:
    return "Number";
  case TokenType::StringLit:
    return "String Literal";
  case TokenType::CharLit:
    return "Character Literal";
  case TokenType::Preproc:
    return "Preprocessor Directive";
  case TokenType::Comment:
    return "Comment";
  case TokenType::Reserved:
    return "Reserved Word";
  case TokenType::Operator:
    return "Operator";
  case TokenType::Punct:
    return "Punctuation";
  case TokenType::Ident:
    return "Identifier";
  default:
    return "Unknown";
  }
}

static const std::unordered_set<string> kKeywords = {
    "break",    "case",    "catch",      "class", "const",    "continue",
    "debugger", "default", "delete",     "do",    "else",     "enum",
    "export",   "extends", "finally",    "for",   "function", "if",
    "import",   "in",      "instanceof", "let",   "new",      "return",
    "super",    "switch",  "this",       "throw", "try",      "typeof",
    "var",      "void",    "while",      "with",  "yield",    "await",
    "null",     "true",    "false"};

static const auto RXF = std::regex::ECMAScript | std::regex::optimize;
static const std::regex reComment(R"(//[^\n]*|/\*[\s\S]*?\*/)", RXF);
static const std::regex reString(R"("(?:\\.|[^\\"])*"|'(?:\\.|[^\\'])*')", RXF);
static const std::regex reChar(R"('(?:\\.|[^\\'])')", RXF);
static const std::regex reNumber(
    R"((0[xX][0-9a-fA-F]+)|(\d+\.\d+([eE][-+]?\d+)?)|(\d+([eE][-+]?\d+)?))",
    RXF);
static const std::regex rePreproc(R"(#.*)", RXF);
static const std::regex reIdent(R"([a-zA-Z_$][a-zA-Z0-9_$]*)", RXF);
static const std::regex reOp(
    R"(\+\+|--|==|!=|<=|>=|&&|\|\||\+=|-=|\*=|/=|%=|<<|>>|>>>|&=|\|=|\^=|=>|[-+*/%&|^~<>!=]=?)",
    RXF);
static const std::regex rePunct(R"([{}()\[\],.;:?])", RXF);

enum class MatchKind {
  None,
  Comment,
  String,
  Char,
  Number,
  Preproc,
  Operator,
  Punct,
  Ident
};

struct Pattern {
  const std::regex &re;
  MatchKind kind;
};

vector<Token> scan(const string &code) {
  vector<Token> out;
  auto it = code.cbegin(), end = code.cend();
  std::smatch m;
  static const Pattern patterns[] = {
      {reComment, MatchKind::Comment}, {reString, MatchKind::String},
      {reChar, MatchKind::Char},       {reNumber, MatchKind::Number},
      {rePreproc, MatchKind::Preproc}, {reOp, MatchKind::Operator},
      {rePunct, MatchKind::Punct},     {reIdent, MatchKind::Ident}};
  while (it != end) {
    if (std::isspace((unsigned char)*it)) {
      ++it;
      continue;
    }
    MatchKind kind = MatchKind::None;
    for (auto &p : patterns)
      if (std::regex_search(it, end, m, p.re) && m.position() == 0) {
        kind = p.kind;
        break;
      }
    switch (kind) {
    case MatchKind::Comment:
      out.push_back({m.str(), TokenType::Comment});
      break;
    case MatchKind::String:
      out.push_back({m.str(), TokenType::StringLit});
      break;
    case MatchKind::Char:
      out.push_back({m.str(), TokenType::CharLit});
      break;
    case MatchKind::Number:
      out.push_back({m.str(), TokenType::Number});
      break;
    case MatchKind::Preproc:
      out.push_back({m.str(), TokenType::Preproc});
      break;
    case MatchKind::Operator:
      out.push_back({m.str(), TokenType::Operator});
      break;
    case MatchKind::Punct:
      out.push_back({m.str(), TokenType::Punct});
      break;
    case MatchKind::Ident: {
      string v = m.str();
      if (kKeywords.count(v))
        out.push_back({v, TokenType::Reserved});
      else
        out.push_back({v, TokenType::Ident});
      break;
    }
    default: {
      string bad(1, *it);
      out.push_back({bad, TokenType::Unknown});
      ++it;
      continue;
    }
    }
    it += m.length();
  }
  return out;
}

string readFile(const string &fname) {
  std::ifstream f(fname);
  if (!f.is_open()) {
    std::cerr << "Cannot open file: " << fname << "\n";
    std::exit(1);
  }
  std::ostringstream ss;
  ss << f.rdbuf();
  return ss.str();
}

int main() {
  string fname;
  std::cout << "Enter the JavaScript source file name: ";
  std::getline(std::cin, fname);
  string code = readFile(fname);
  auto toks = scan(code);
  std::cout << "\nTokens and their types:\n";
  for (auto &t : toks)
    std::cout << "<" << t.text << ", " << typeName(t.type) << ">\n";
  return 0;
}
