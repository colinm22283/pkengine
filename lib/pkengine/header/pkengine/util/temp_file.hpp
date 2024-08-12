#pragma once

#include <cstdlib>
#include <fstream>

namespace PKEngine::Util {
    class TempFile : public std::fstream {
    public:
        inline TempFile() : std::fstream() {
            char temp_name[] = "/tmp/tmpfileXXXXXX";

            int fd = mkstemp(temp_name);

            if (fd == -1) return;

            open(temp_name);

            ::close(fd);
        }
    };
}