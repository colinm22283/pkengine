#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <cstdint>

#include <util/engine_exception.hpp>

namespace PKEngine {
    std::vector<char> read_file(const std::string & filename) {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) throw Exceptions::UnableToOpenFile();

        uint32_t file_size = file.tellg();
        std::vector<char> buffer(file_size);

        file.seekg(0);
        file.read(buffer.data(), file_size * sizeof(char));

        file.close();

        return buffer;
    }
}