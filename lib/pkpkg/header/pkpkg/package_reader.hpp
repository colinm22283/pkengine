#pragma once

namespace PKPKG {
    class PackageReader {
    protected:
        const char * const path;
        std::ifstream fs;

        struct sector_t {
            std::string tag;
            std::ifstream::pos_type start, length;
        };

    public:
        explicit inline PackageReader(const char * _path): path(_path), fs(path, std::ifstream::binary) {

        }
    };
}