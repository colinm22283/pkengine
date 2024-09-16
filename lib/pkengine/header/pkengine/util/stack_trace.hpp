#pragma once

#include <array>
#include <cstring>

#include <dlfcn.h>
#include <execinfo.h>
#include <cxxabi.h>

namespace PKEngine::Util {
    template<std::size_t maximum_length>
    class StackTrace {
        static constexpr std::size_t string_max_length = 2048;

    protected:
        std::size_t call_stack_length = 0;
        std::array<void *, maximum_length + 1> call_stack;

    public:
        __attribute__((always_inline)) inline StackTrace() {
            backtrace(call_stack.data(), maximum_length);
            call_stack[maximum_length] = nullptr;

            while (call_stack_length < maximum_length && call_stack[call_stack_length] != nullptr) call_stack_length++;
        }

        struct ResolvedFrame {
            void * address;
            void * file_base;
            char file[string_max_length];
            char name[string_max_length];
        };

        class Iterator {
            friend class StackTrace;

        protected:
            void ** current_frame_ptr;
            ResolvedFrame current_frame;

            explicit inline Iterator(void ** _current_frame_ptr) : current_frame_ptr(_current_frame_ptr) {
                decode_frame();
            }

            inline void decode_frame() {
                current_frame.address = *current_frame_ptr;

                Dl_info dl_info;
                if (dladdr(*current_frame_ptr, &dl_info) != 0) {
                    current_frame.file_base = dl_info.dli_fbase;

                    std::strncpy(current_frame.file, dl_info.dli_fname, string_max_length - 1);
                    if (dl_info.dli_sname != nullptr) {
                        int status;
                        std::size_t demangled_length = string_max_length;
                        abi::__cxa_demangle(dl_info.dli_sname, current_frame.name, &demangled_length, &status);

                        if (status != 0) std::strncpy(current_frame.name, dl_info.dli_sname, string_max_length - 1);
                    }
                    else current_frame.name[0] = '\0';
                }
                else {
                    current_frame.file[0] = '\0';
                    current_frame.name[0] = '\0';
                }
            }

        public:
            inline Iterator & operator++() {
                current_frame_ptr++;

                decode_frame();

                return *this;
            }

            inline Iterator & operator++(int) { return this->operator++(); }

            inline bool operator==(Iterator & other) const { return current_frame_ptr == other.current_frame_ptr; }

            inline bool operator!=(Iterator & other) const { return current_frame_ptr != other.current_frame_ptr; }

            inline const ResolvedFrame & operator*() const { return current_frame; }
        };

        inline Iterator begin() { return Iterator(call_stack.data()); }

        inline Iterator end() { return Iterator(call_stack.data() + call_stack_length); }

        inline void print(auto & stream) {
            std::size_t max_fname = 0;
            for (std::size_t i = 0; i < call_stack_length; i++) {
                Dl_info dl_info;

                dladdr(call_stack[i], &dl_info);

                if (dl_info.dli_sname != nullptr) {
                    std::size_t temp_len = std::strlen(dl_info.dli_fname);

                    if (temp_len > max_fname) max_fname = temp_len;
                }
            }

            for (std::size_t i = 0; i < call_stack_length; i++) {
                Dl_info dl_info;

                dladdr(call_stack[i], &dl_info);

                if (dl_info.dli_sname != nullptr) {
                    int status;
                    std::size_t demangled_length = 1024;
                    char demangled[1024];
                    abi::__cxa_demangle(dl_info.dli_sname, demangled, &demangled_length, &status);

                    auto stream_2 = stream << dl_info.dli_fname;

                    for (std::size_t j = std::strlen(dl_info.dli_fname); j < max_fname; j++) stream_2 << " ";

                    if (status == 0) stream_2 << " - " << demangled;
                    else stream_2 << " - " << dl_info.dli_sname;
                }
            }
        }
    };
}