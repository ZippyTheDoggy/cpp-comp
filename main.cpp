#include <iostream>
#include <vector>
#include <any>
#include <functional>

#include "./charops.h"
#include "./arrayops.h"
#include "./is.h"

#include "./TokenTypes.h"

#include "./combineQueue.h"

#define _DEBUG false
#define debug(x) if(_DEBUG) std::cout << x << std::endl
#define debugt(x) std::cout << x << std::endl;

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

LinkedList<TokenData> combineElements(std::vector<TokenData> tokens) {
  std::function<TokenData(TokenData, TokenData)> math_divide = [&](TokenData a, TokenData b) {
    return TokenData {0, TokenType::NUMBER, "hi!"};
    // return TokenData {a.line, a.type, std::to_string(std::stoi(a.value) / std::stoi(b.value))};
  };

  LinkedList<TokenData> list(tokens);
  Linked<TokenData>* pointer = list[0];
  int pos = 0;
  while(pointer->next != nullptr) {
    TokenType type = pointer->item.type;
    std::string value = pointer->item.value;
    switch(type) {
      case TokenType::DIVIDE: {
        TokenData left = pointer->last->item;
        TokenData right = pointer->next->item;
        if(left.type != TokenType::NUMBER) {
          std::cout << "Divide on line " << pointer->item.line << " is expecting a LVALUE of type NUMBER, got " << fancy_tt(type) << " instead.\n";
        } else if(right.type != TokenType::NUMBER) {
          std::cout << "Divide on line " << pointer->item.line << " is expecting a RVALUE of type NUMBER, got " << fancy_tt(type) << " instead.\n";
        } else {
          int lNum = std::stoi(left.value);
          int rNum = std::stoi(right.value);
          std::cout << "Expecting combine value " << (lNum / rNum) << std::endl;
          // list.combine(pos-1, pos+1, math_divide);
          /*
          erase(pos-1);
          erase(pos);
          erase(pos+1);
          insert(pos, lnum/rnum);
          */
          list.erase(pos-1);
          list.erase(pos+1);
          Linked<TokenData>* i = list.insert(pos-1, TokenData {pointer->item.line, pointer->item.type, std::to_string(lNum / rNum)});
          list.erase(pos);
          std::cout << "A:" << i->item.value << std::endl;
        }
      } break;
    }
    pointer = pointer->next;
    pos++;
  }
  return list;
};

int main(int argc, char const *argv[]) {
  std::string code = "3 / 2";
  std::vector<TokenData> lexed = lexer(code);
  LinkedList<TokenData> combined = combineElements(lexed);
  combined.print();

  return EXIT_SUCCESS;
}
