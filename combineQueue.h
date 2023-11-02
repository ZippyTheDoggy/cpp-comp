#pragma once

#include <vector>
#include <functional>

/*

Combining Queue:
A queue where any item in the queue can combine with either its left or right element and form into one;
With this, all elements to the right of the combined element must have their index changed.

The idea for this is:
1 + 2 + 3 + 4
1 + 2 will combine to 3 leaving: 3 + 3 + 4
3 + 3 will combine to 6 leaving: 6 + 4
which will combine down to 10.

HOWEVER;
In the case of 1 + 2 * 3 + 4
we must do 2 * 3 first so: 1 + 6 + 4, then combine LTR.  PEMDAS iterates L-T-R for each operator


[head], index, [tail] -> [head -], [index + last(head)], [tail]
[head], index, [tail] -> [head], [index + first(tail)], [- tail]


LINKED LIST
a -> b -> c -> d
if combining b
relation +1: b = combine(b, b.next);  while(b.next.next != null) b.next = b.next.next;  // SHIFT ALL TO LEFT;
relation -1: b = combine(b, b.last);  while(b.last.last != null) b.last = b.last.last;  // SHIFT ALL TO RIGHT;

*/

template <typename T>
struct Linked {
  T item;
  struct Linked<T>* last;
  struct Linked<T>* next;
};

template <typename T>
class LinkedList {
  private:
    std::vector<struct Linked<T>*> items;
  public:
    LinkedList(std::vector<T> list) {
        for(T item : list)
            items.push_back(new struct Linked<T>({item, nullptr, nullptr}));
        for(int i = 0; i < list.size(); i++) {
          if(i > 0) items[i]->last = items[i-1];
          if(i < list.size()) items[i]->next = items[i+1];
        }
    };
    Linked<T>* append(T item) {
        if(items.size() == 0) items.push_back(new struct Linked<T>({item, nullptr, nullptr}));
        else {
            items.push_back(new struct Linked<T>({item, items[items.size() - 1], nullptr}));
            for(int i = 0; i < items.size() - 1; i++) {
                if(!(i < items.size() - 2))
                    items[i]->next = items[i+1];
            }
        }
        return items[items.size()];
    }
    Linked<T>* prepend(T item) {
        items.insert(items.begin(), new struct Linked<T>({item, nullptr, nullptr}));
        if(items.size() >= 2) {
            items[1]->last = items[0];
            items[0]->next = items[1];
        }
        return items[0];
    }
    Linked<T>* insert(int pos, T item) {
        if(pos < 0) return nullptr;
        if(pos > items.size()) return nullptr;
        Linked<T>* ptr;
        items.insert(items.begin() + pos, new struct Linked<T>({item, nullptr, nullptr}));
        ptr = items[pos];
        if(pos < items.size()) {
            ptr->next = items[pos + 1];
            ptr->next->last = items[pos];
        }
        if(pos >= 1) {
            ptr->last = items[pos - 1];
            ptr->last->next = items[pos];
        }
        return ptr;
    }
    void erase(int pos) {
       if(pos == 0) {
           items[pos]->next->last = nullptr;
           delete items[pos];
       } else if(pos == items.size()) {
           items[pos]->last->next = nullptr;
           delete items[pos];
       } else {
           items[pos]->last->next = items[pos]->next;
           items[pos]->next->last = items[pos]->last;
           delete items[pos];
       }
    }
    Linked<T>* at(int i) {
      if(i >= 0 && i < items.size()) return items[i];
      else return nullptr;
    }
    Linked<T>* operator[](int i) {
        return at(i);
    }
    void print(bool endline = 1) {
        std::cout << "{ ";
        Linked<T>* ptr = items[0];
        while(true) {
            std::cout << ptr->item;
            if(ptr -> next != nullptr) std::cout << ", ";
            else break;
            ptr = ptr->next;
        }
        std::cout << " }";
        if(endline) std::cout << std::endl;
    }
    // Linked<T>* combine(int pos, int pos2, std::function<T(T, T)> fct) {
    //     Linked<T>* first = items[pos];
    //     Linked<T>* second = items[pos2];
    //     T data = fct(first->item, second->item);
    //     first->item = data;
    //     // fixing pointers so that items around pos2 point correctly after its deleted
    //     if(second->next != nullptr) second->next->last = second->last;
    //     if(second->last != nullptr) second->last->next = second->next;
    //     if(pos2 == (pos - 1)) {           // pos2 is immediately left of pos
    //         first->last = second->last;
    //     } else if((pos - pos2) > 0) {     // pos2 is immediately right of pos
    //         first->next = second->next;
    //     }
    //     // shift all the items to the left in the array so that [pos2] is not in array
    //     for(int i = pos2; i < items.size(); i++)
    //         if(i != items.size() -1) items[i] = items[i + 1];
    //     items.resize(items.size() - 1);
    //     return first;
    // }
    // int size() {
    //     return items.size();
    // }
};
