#ifndef INFRA_ALLOC_H
#define INFRA_ALLOC_H

#include <cstddef>
#include <unordered_map>
#include <sys/mman.h>
#include <iostream>

struct Chunk {
    size_t size;
    void* ptr;
    Chunk* next;
    Chunk(size_t s = 0, void* p = nullptr) : size{s}, ptr{p}, next{nullptr} {}
};

class Allocator {
    size_t chunkSize;
    Chunk* freeList;
    std::unordered_map<void*, Chunk*> usedMap;
public:
    Allocator(size_t freeListSize, size_t cSize = 128 * 1024) : chunkSize{cSize} {
        while (freeListSize--) {
            pushFrontFreeList();
        }
    }
    ~Allocator() noexcept {
        Chunk* next;
        while (freeList) {
            next = freeList->next;
            free(freeList->ptr);
            free(freeList);
            freeList = next;
        }
    }
    void* allocate(size_t size) {
        if (size > chunkSize) {
            std::cout << size << "B allocating via mmap ...\n";
            void* p = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
            if (p == MAP_FAILED) {
                std::cerr << "mmap failed: " << errno << '\n';
                return nullptr;
            }
            Chunk* chunk = newChunk();
            chunk->size = size;
            chunk->ptr = p;
            usedMap.insert({p, chunk});
            return p;
        }
        std::cout << size << "B allocating via chunks ...\n";
        if (!freeList) {
            pushFrontFreeList();
        }
        void* p = freeList->ptr;
        Chunk* next = freeList->next;
        freeList->next = nullptr;
        usedMap.insert({p, freeList});
        freeList = next;
        return p;
    }
    void deallocate(void* ptr) {
        std::cout << ptr << " deallocating ...\n";

        auto it = usedMap.find(ptr);
        if (it == usedMap.end()) {
            // error, shouldn't happen
            return;
        }
        if (it->second->size > chunkSize) {
            munmap(ptr, it->second->size);
            std::cout << ptr << ": freed via unmap\n";
        } else {
            it->second->next = freeList;
            freeList = it->second;
            usedMap.erase(it->first);
            std::cout << ptr << ": recycled chunk\n";
        }
    }
private:
    Chunk* newChunk() {
        return static_cast<Chunk*>(malloc(sizeof(Chunk)));
    }
    void pushFrontFreeList() {
        std::cout << "start of pushFrontFreeList\n";

        Chunk* cp = newChunk();
        void* p = malloc(chunkSize);
        new (cp) Chunk(chunkSize, p);
        cp->next = freeList;
        freeList = cp;
    }
};

#endif