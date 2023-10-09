#include <iostream>
#include <vector>
#include <any>

#include "charops.h"
#include "arrayops.h"
#include "is.h"

#define _DEBUG false
#define debug(x) if(_DEBUG) std::cout << x << std::endl
#define debugt(x) std::cout << x << std::endl;

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
}

typedef struct {
  size_t line;
  TokenType type;
  std::string value;
} TokenData;

bool eol(char* ptr) {
  return (*ptr == '\0') || (ptr == nullptr);
};

std::vector<TokenData> lexer(std::string code) {
  const char* purestr = code.c_str();
  std::vector<TokenData> tokens;
  char* ptr = (char*) &purestr[0];
  TokenType tokenBuffer;
  std::string valueBuffer = "";
  size_t line = 0;

  auto finish_token = [&]() {
    tokens.push_back(TokenData {line, tokenBuffer, valueBuffer});
    tokenBuffer = TokenType::INVALID;
    valueBuffer = "";
  };

  while(!eol(ptr)) {
    if(is::number(*ptr)) {
      tokenBuffer = TokenType::NUMBER;
      while(is::number(*ptr) && !eol(ptr)) {
        valueBuffer += *ptr;
        ptr++;
      }
      ptr--;
      finish_token();
    } else if((is::character(*ptr) || is::identifier_prefix(*ptr)) && !eol(ptr)) {
      tokenBuffer = TokenType::IDENTIFIER;
      while(is::identifier(*ptr)) {
        valueBuffer += *ptr;
        ptr++;
      }
      ptr--;
      finish_token();
    } else if(is::double_quote(*ptr) && !eol(ptr)) {
      tokenBuffer = TokenType::STRING;
      ptr++;
      while(!is::double_quote(*ptr)) {
        valueBuffer += *ptr;
        ptr++;
      }
      debug("Ended a string, \"" << valueBuffer << "\"");
      // !! explicit no ptr-- else end quote seen as new quote
      finish_token();
    }

    // operators
    else if(*ptr == '=') tokens.push_back(TokenData {line, TokenType::ASSIGNMENT, "="});
    else if(*ptr == '+') tokens.push_back(TokenData {line, TokenType::ADD, "+"});
    else if(*ptr == '-') tokens.push_back(TokenData {line, TokenType::SUBTRACT, "-"});
    else if(*ptr == '/') tokens.push_back(TokenData {line, TokenType::DIVIDE, "/"});
    else if(*ptr == '*') tokens.push_back(TokenData {line, TokenType::ASTERISK, "*"});

    else if(*ptr == '\n') line++;

    ptr++;

    if(valueBuffer != "") finish_token();
  }
  debug("token num is " << tokens.size());
  for(TokenData td : tokens) {
    debug("LEXER TokenData { " << td.line << ", "
      << fancy_tt(td.type)
      << ", " << td.value << "};");
  }
  return tokens;
};

typedef struct {
    std::string type;
    std::string data;
    std::function<std::any(std::string)> parse;
} ASTData;

void combineElements(std::vector<TokenData> tokens) {
  std::vector<ASTData> asts = {};
  for(size_t i = 0; i < tokens.size(); i++) {
    TokenData data = tokens[i];
    size_t line = data.line;
    TokenType type = data.type;
    std::string val = data.value;
    switch(type) {
      case TokenType::MULTIPLY: {
        if(!ensure(-1)) {
          debug("Exited combine MULTIPLY, failed ensure -1");
          std::cout << "Failed to combine multiply token." << std::endl;
          exit(EXIT_FAILURE);
        };
        if(!ensure(1)) {
          debug("Exited combine MULTIPLY, failed ensure 1");
          std::cout << "Failed to combine multiply token." << std::endl;
          exit(EXIT_FAILURE);
        }
        int lvalue = std::stoi(tokens[i-1].value);
        int rvalue = std::stoi(tokens[i-1].value);
        asts.push_back({
          "multiply",
          std::string(lvalue * rvalue),
          [&](std::string s) {
            return std::any_cast<int>(std::stoi(s));
          }
        })
      } break;
    }
  }
};
//
//
//
/* CONSIDERATION ON PROGRAMMING OUTLINE
  1/2/3 WILL CONFLICT AS BOTH ARE TRYING TO USE 2 AS A NUMBER
  WILL RESULT IN (1/2) as .5 and (2/3) as .66, resulting in .5/.66 instead of .5/3
  EITHER MULTIPLE ITERATIONS TAKING IN THE LAST INPUT, STARTING w/ TOKENS, REQUIRES WHOLE
  REVISION OF WHAT INPUTS THE AST PARSER TAKES,

  OTHER OPTION IS TO ADD AN "EXPRESSION" WHICH IS ANY NUMBER,
  CAN BE EITHER AST OR TOKEN, AST MUST STORE INDEX OF TOKEN
  FOR LATER USAGE

  if lvalue.is_expression:
    lvalue = get_expression(-1, token.index) // get the left expression including index
  if rvalue.is_expression:
    rvalue = get_expression(1, token.index); // get the right expression including index
  1/2/3 takes 1/2 first, turns it into expr/3, recognizes, takes value .5/3
  returns expression of .5/3

  EXPRESSION IS ANY AST;
  USAGE CASE FOR STRINGS IS "STR" + "STR"
  + takes TWO STRINGS RETURNS A STRING WHICH WILL BE WRAPPED BY expression
  "STR" + "STR" + "STR" WILL BE EXPR + "STR", LVALUE IS EXPR
*/
//
//
//

int main(int argc, char const *argv[]) {

  std::string code = "4 / 5";
  std::vector<TokenData> lexed = lexer(code);
  combineElements(code);

  return EXIT_SUCCESS;
}
