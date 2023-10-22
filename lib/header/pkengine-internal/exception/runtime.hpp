#pragma once

#include "exception_base.hpp"

namespace PKEngine::Exception {
    class RuntimeException : public ExceptionBase {
    public:
        [[nodiscard]] _exception_type_t exception_type() const noexcept final { return ET_RUNTIME; }
    };

}
#define PKENGINE_RUNTIME_EXCEPTION(name, message)                              \
class name : public PKEngine::Exception::RuntimeException {                    \
    [[nodiscard]] const char * what() const noexcept final { return message; } \
}

#define PKENGINE_INLINE_RUNTIME_THROW(message)                  \
do {                                                            \
    PKENGINE_RUNTIME_EXCEPTION(_exception_to_throw_t, message); \
    throw _exception_to_throw_t();                              \
} while (0);
