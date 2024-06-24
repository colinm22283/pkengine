#pragma once

#include <libbmp.h>
#include <png.h>

namespace PKEngine {
    template<typename InputStream>
    class ImageReader {
    public:
        enum InputFormat {
            INFER_FROM_EXTENSION = 0,
            BMP,
            PNG,
        };

    protected:
        InputStream & input_stream;

        inline void read_bmp() {

        }

    public:
        explicit inline ImageReader(InputStream & _input_stream): input_stream(_input_stream) { }

        inline void read()
    };
}