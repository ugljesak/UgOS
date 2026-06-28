#ifndef UGOS_LIST_HPP
#define UGOS_LIST_HPP

#include "../lib/hw.h"
#include "MemoryAllocator.hpp"

template<typename T>
struct Node {
    Node(T val) : val(val), next(nullptr) {}
    T val;
    Node* next;
};

template<typename T>
class List {
public:
    List() : head(nullptr), tail(nullptr) {}
    
    void push(T val) {
        Node<T>* node = (Node<T>*)MemoryAllocator::mem_alloc(sizeof(Node<T>));
        node->next = nullptr;
        node->val = val;

        if(tail == nullptr) {
            head = tail = node;
            return;
        }
        tail->next = node;
        tail = node;
    }
    
    T pop() {
        if(head == nullptr) return T();
        
        Node<T>* node = head;
        T val = node->val;
        head = head->next;
        if(head == nullptr) tail = nullptr;
        
        MemoryAllocator::mem_free(node);
        return val;
    }
    
    void* operator new(size_t size) {
        return MemoryAllocator::mem_alloc(size);
    }
    void* operator new[](size_t size) {
        return MemoryAllocator::mem_alloc(size);
    }
    
    void operator delete(void *ptr) {
        MemoryAllocator::mem_free(ptr);
    }
    void operator delete[](void *ptr) {
        MemoryAllocator::mem_free(ptr);
    }

private:
    Node<T>* head;
    Node<T>* tail;
};

#endif