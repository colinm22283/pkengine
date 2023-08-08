#pragma once

#include <exception>

namespace PKEngine::Exception {
    enum _exception_type_t { ET_INTERNAL };

    class ExceptionBase : public std::exception {
    public:
        [[nodiscard]] virtual _exception_type_t exception_type() const noexcept = 0;
    };
}