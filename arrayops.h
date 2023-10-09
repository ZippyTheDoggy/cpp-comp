#pragma once

#include <cstring>
#include <map>

std::map<void*, size_t> _arrayops_size_map = {};
std::map<void*, size_t> _arrayops_indexes  = {};

template <typename T>
T* create_array(size_t size) {
  void* array = malloc(size);
  _arrayops_size_map[array] = size;
  _arrayops_indexes[array] = 0;
  return (T*) array;
};

template <typename T>
void* add_to_array(T* array, T value) {
  size_t current_index =  _arrayops_indexes[array];
  size_t max_index     = _arrayops_size_map[array];
  if((current_index + 1) <= max_index) {
    array[current_index++] = value;
    _arrayops_indexes[array]++;
    return &array[current_index];
  }
  else return nullptr;
};

template <typename T>
void* is_in_array(T* array, T value) {
  size_t max_index = _arrayops_size_map[array];
  for(size_t i = 0; i < max_index; i++) {
    if(array[i] == value) return &array[i];
  }
  return nullptr;
};

template <typename T>
size_t array_size(T* array) {
  if(_arrayops_size_map[array]) return _arrayops_size_map[array];
  return -1;
};

template <typename T>
void delete_array(T* array) {
  free(array);
  delete _arrayops_size_map[array];
  delete _arrayops_indexes[array];
};

template <typename T>
void* resize_array(T* array, size_t newSize) {
  T* newArray = create_array<T>(newSize);
  for(size_t i = 0; i < array_size<T>(array); i++)
    newArray[i] = array[i];
  delete_array<T>(array);
  return newArray;
};
