#pragma once

namespace PKEngine {
    template<typename T>
    class OptionalPtr {
    protected:
        T * ptr;

    public:
        inline OptionalPtr():
          ptr(nullptr) { }
        inline OptionalPtr(T * _ptr):
          ptr(_ptr) { }

        inline operator bool() { return ptr != nullptr; }
        inline operator T *() { return ptr; }
        inline  T * operator->() { return ptr; }
    };
}