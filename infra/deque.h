#ifndef INFRA_DEQUE_H
#define INFRA_DEQUE_H

#include <list>

#include <iostream>

template<typename T>
struct Chunk {
    size_t size_;
    Chunk* prev_;
    Chunk* next_;
    T* data_;
    Chunk(const size_t& size, Chunk* p = nullptr, Chunk* n = nullptr)
        : size_{size}, prev_{p}, next_{n}, data_{size ? new T[size] : nullptr} {}
    ~Chunk() noexcept {if (data_) delete[] data_;}
};

template<typename T>
struct Iter {
    T* curr_;
    T* cb_;
    T* ce_;
    Chunk<T>* chunk_;
    Iter(Chunk<T>* chunk = nullptr): chunk_{chunk},
        curr_{chunk?chunk->data_:nullptr}, 
        cb_{chunk?chunk->data_:nullptr}, 
        ce_{chunk?(chunk->data_+chunk->size_):nullptr} {}
    bool incre() {
        if (curr_ == ce_-1) {
            if (!chunk_->next_ || !chunk_->next_->data_) return false;
            chunk_ = chunk_->next_;
            cb_ = chunk_->data_;
            ce_ = chunk_->data_+chunk_->size_;
            curr_ = cb_;
        } else {
            ++curr_;
        }
        return true;
    }
    bool decre() {
        if (curr_ == cb_) {
            if (!chunk_->prev_ || !chunk_->prev_->data_) return false;
            chunk_ = chunk_->prev_;
            cb_ = chunk_->data_;
            ce_ = chunk_->data_+chunk_->size_;
            curr_ = ce_ - 1;
        } else {
            --curr_;
        }
        return true;
    }
};
template<typename T>
std::ostream& operator<<(std::ostream& os, const Iter<T>& iter) {
    return os << "[" << iter.chunk_ << "," << iter.cb_ << "," << iter.ce_ << "," << iter.curr_ << "]";
}

template<typename T>
class Deq {
    size_t numChunk_;
    size_t chunkCap_;
    size_t size_;

    Chunk<T>* chunkBeg_;
    Chunk<T>* chunkEnd_;

    Iter<T> beg_;
    Iter<T> end_;
public:
    Deq(const size_t& numChunk, const size_t& chunkCap) : 
        numChunk_{numChunk}, chunkCap_{chunkCap}, size_{0}, chunkBeg_{new Chunk<T>{0}}, chunkEnd_{new Chunk<T>{0}} {
        
        chunkBeg_->next_ = chunkEnd_;
        chunkEnd_->prev_ = chunkBeg_;

        size_t halfChunk {numChunk/2};
        for (size_t i {0}; i < numChunk; ++i) {
            Chunk<T>* chunk = new Chunk<T>(chunkCap);
            // std::cout << i << " chunk: " << chunk << '\n';
            insertChunkEnd(chunk);
            if (i == halfChunk) {
                beg_ = Iter(chunk);
                end_ = beg_;
            }
        }
    }
    ~Deq() noexcept {
        std::cout << "destroying Deq...\n";
        Chunk<T>* chunk = chunkBeg_;
        Chunk<T>* nchunk;
        while (chunk) {
            nchunk = chunk->next_;
            delete chunk;
            chunk = nchunk;
        }
    }
    bool pushFront(const T& x) {
        if (!empty()) {
            if (!beg_.decre()) return false; // TODO should insert new chunk
        }
        ++size_;
        *(beg_.curr_) = x;
        return true;
    }
    bool popFront(T& x) {
        if (empty()) return false;
        x = *(beg_.curr_);
        --size_;
        beg_.incre();
        return true;
    }
    bool pushBack(const T& x) {
        if (!empty()) {
            if (!end_.incre()) return false; // TODO should insert new chunk
        }
        ++size_;
        *(end_.curr_) = x;
        return true;
    }
    bool popBack(T& x) {
        if (empty()) return false;
        x = *(end_.curr_);
        --size_;
        end_.decre();
        return true;
    }

    void print() const {
        std::cout << "beg:" << beg_ << ",end:" << end_ << "\n";
    }
    bool empty() const {return size_ == 0;}
private:
    void insertChunkBeg(Chunk<T>* chunk) {
        chunkBeg_->next_->prev_ = chunk;
        chunk->next_ = chunkBeg_->next_;
        chunkBeg_->next_ = chunk;
        chunk->prev_ = chunkBeg_;
    }
    void insertChunkEnd(Chunk<T>* chunk) {
        chunkEnd_->prev_->next_ = chunk;
        chunk->prev_ = chunkEnd_->prev_;
        chunkEnd_->prev_ = chunk;
        chunk->next_ = chunkEnd_;
    }
};

#endif