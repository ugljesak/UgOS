#include "../h/SleepQueue.hpp"
#include "../h/Scheduler.hpp"


void SleepQueue::insert(TCB* t, uint64 time) {
        
        SleepNode* node = (SleepNode*)MemoryAllocator::mem_alloc(sizeof(SleepNode));
        node->thread = t;
        node->dist = time;
        node->next = nullptr;

        SleepNode* curr = head;
        SleepNode* prev = nullptr;

        while(curr && curr->dist < node->dist) {
            node->dist -= curr->dist;
            prev = curr;
            curr = curr->next;
        }

        if(curr) {
            curr->dist -= node->dist;
        }
        node->next = curr;

        if(prev) {
            prev->next = node;
        }
        else {
            head = node;
        }
    }
    
    void SleepQueue::tick() {
        if(head == nullptr) return;
        head->dist--;
        
        while(head && head->dist == 0) {
            Scheduler::put(head->thread);
            SleepNode* old = head;
            head = head->next;
            MemoryAllocator::mem_free(old);
        }
    }