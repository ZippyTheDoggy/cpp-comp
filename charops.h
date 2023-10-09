#pragma once
#include <cstring>

char c_lowercase(char chr) {
  if(chr >= 'A' && chr <= 'Z') return chr + 32;
  return chr;
};
char c_uppercase(char chr) {
  if(chr >= 'a' && chr <= 'z') return chr - 32;
  return chr;
};

const char* cc_lowercase(const char* sequence) {
  size_t len = strlen(sequence);
  char* string = (char*) malloc(len);
  for(size_t i = 0; i <= len; i++) string[i] = c_lowercase(sequence[i]);
  return string;
};
const char* cc_uppercase(const char* sequence) {
  size_t len = strlen(sequence);
  char* string = (char*) malloc(len);
  for(size_t i = 0; i <= len; i++) string[i] = c_uppercase(sequence[i]);
  return string;
}
