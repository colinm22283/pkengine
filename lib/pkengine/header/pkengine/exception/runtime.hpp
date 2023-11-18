#pragma once

#include <exception>

namespace PKEngine::Exception {
    class RuntimeException : public std::exception { };

}
#define PKENGINE_RUNTIME_EXCEPTION(name, message)                              \
class name : public PKEngine::Exception::RuntimeException {                    \
    [[nodiscard]] inline const char * what() const noexcept final { return message; } \
}

#define PKENGINE_INLINE_RUNTIME_THROW(message)                  \
do {                                                            \
    PKENGINE_RUNTIME_EXCEPTION(_exception_to_throw_t, message); \
    throw _exception_to_throw_t();                              \
} while (0);
