#ifndef SHARED_PTR_HEADER
#define SHARED_PTR_HEADER
#include <iostream>

class ControlBlockBase {
public:
    ControlBlockBase() : refcount(0){
    }

    // dtor is virtual, so that we can call derived class's dtor from a ptr to this base class.
    virtual ~ControlBlockBase(){
    }

    // pure virtual function; must be overriden by derived classes
    virtual void* managedAddress() = 0;

    // Delete copies, which also implicitly deletes moves.
    ControlBlockBase(const ControlBlockBase&) = delete;
    ControlBlockBase& operator=(const ControlBlockBase&) = delete;

    long increment()
    {
        // TODO: increment refcount by 1 and return result.
        return ++refcount;
    }

    long decrement()
    {
        // TODO: decrement refcount by 1 and return result.
        return --refcount;
    }

    long refCount() const
    {
        // TODO: just return the refcount.
        return refcount;
    }

private:
    // TODO: add field(s) which both control block types need to have
    long refcount;
};

template <typename T>
class ControlBlock : public ControlBlockBase{
public:
    ControlBlock(T* ptr = nullptr) : mPtr(ptr){
    }

    ~ControlBlock() override{
        if (mPtr != nullptr){
            delete mPtr;
            mPtr = nullptr;
        }
    }

    void* managedAddress() override{
        return mPtr;
    }

private:
    T* mPtr;
};

template <typename T>
class SharedPtr{
public:
    // default constructor: control block is nullptr
    SharedPtr() : storedPtr(nullptr), cbPtr(nullptr){
    }

    // constructor: control block manages nullptr
    SharedPtr(T* ptr) : storedPtr(ptr), cbPtr(new ControlBlock(ptr)){
        cbPtr->increment();
    }

    // destructor
    ~SharedPtr(){
        if (cbPtr == nullptr){
            return;
        }
        if (cbPtr->decrement() == 0){
            delete cbPtr;
            cbPtr = nullptr;
            storedPtr = nullptr;
        }
    }

    // copy constructor
    SharedPtr(SharedPtr<T> &other) : storedPtr(other.storedPtr), cbPtr(other.cbPtr){
        cbPtr->increment();
    }

    // move constructor
    SharedPtr(SharedPtr<T> &&other){
        T* ptr = other.get();
        storedPtr = ptr;
        cbPtr = other.cbPtr;
        other.reset();
    }

    // copy assignment operator
    SharedPtr<T>& operator=(SharedPtr<T>& other){
        if (cbPtr->decrement() == 0){
            delete cbPtr;
        }

        T* ptr = other.get();
        storedPtr = ptr;
        cbPtr = other.cbPtr;
    }

    // move assignment operator
    SharedPtr<T>& operator=(SharedPtr<T>&& other){
        T* ptr = other.get();
        storedPtr = ptr;
        cbPtr = other.cbPtr;
        other.reset();
    }

    T& operator*() const{
        return *storedPtr;
    }

    T* operator->() const{
        return storedPtr;
    }

    T* get() const{
        return storedPtr;
    }

    bool operator==(const SharedPtr<T>& other){
        return storedPtr == other.storedPtr;
    }

    operator bool() const{
        return storedPtr != nullptr;
    }

    // cppreference says swapping doesn't change reference counts
    void swap(SharedPtr<T>& other){
        T* temp1 = other.storedPtr;
        ControlBlockBase* temp2 = other.cbPtr;
        other.storedPtr = storedPtr;
        other.cbPtr = cbPtr;
        storedPtr = temp1;
        cbPtr = temp2;
    }

    void reset(){
        SharedPtr<int>().swap(*this);
    }

    void reset(T* other){
        SharedPtr<int>(other).swap(*this);
    }

    long useCount() const{
        return cbPtr->refCount();
    }


private:
    T* storedPtr;
    ControlBlockBase* cbPtr;
};

#endif
