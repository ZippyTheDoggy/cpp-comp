#pragma once

namespace is {
  // general types
  bool number(char c) {
    return (c >= '0' && c <= '9');
  };
  bool character(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
  };
  bool identifier_prefix(char c) {
    return (c == '_') || (c == '$');
  }
  bool identifier(char c) {
    return char(c) || number(c);
  };
  bool double_quote(char c) {
    return c == '"';
  };

  // assignment/mathematical operators
  bool assignment(char c) { return c == '='; };
  bool addition(char c) { return c == '+'; };
  bool subtract(char c) { return c == '-'; };
  bool divide(char c) { return c == '/'; };
  bool asterisk(char c) { return c == '*'; };
}
