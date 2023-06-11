#pragma once

#include <fstream>

#include <util/const_string.hpp>

namespace PKEngine::_LoggerPrivate {
    using namespace PKEngine::Util;

    template<ConstString path>
    struct LogFile {
        static std::ofstream file;

        static constexpr bool is_logfile = true;
    };

    template<ConstString path>
    std::ofstream LogFile<path>::file = std::ofstream(path.data);
}