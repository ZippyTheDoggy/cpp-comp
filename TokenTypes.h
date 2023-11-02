#pragma once

#include "charops.h"

typedef enum {
  INVALID = 0, NUMBER, IDENTIFIER, STRING,
  ASSIGNMENT, ADD, SUBTRACT, DIVIDE, ASTERISK,

} TokenType;
std::string STokenType[] = {
  "Invalid", "Number", "Identifier", "String",
  "Assignment", "Add", "Subtract", "Divide", "Asterisk"
};
const char* fancy_tt(TokenType tt) {
  return cc_uppercase(STokenType[tt].c_str());
};

typedef struct {
  size_t line;
  TokenType type;
  std::string value;
} TokenData;

std::ostream& operator<<(std::ostream& output, TokenData td) {
  output << "{ line: " << td.line << ", type: " << fancy_tt(td.type) << ", value: " << td.value << " }";
  return output;
};
