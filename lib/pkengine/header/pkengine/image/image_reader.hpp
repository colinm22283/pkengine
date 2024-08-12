#pragma once

#include <cbmp.h>
#include <png.h>

#include <pkengine/engine.hpp>

#include <pkengine/exception/runtime.hpp>

#include "pkengine/internal/vulkan/image.hpp"

namespace PKEngine {
    class ImageReader {
    protected:
        static constexpr auto logger = Logger<"Image Reader">();

    public:
        struct Exceptions {
            PKENGINE_RUNTIME_EXCEPTION(InvalidPath, "Invalid path provided to read_file call");
            PKENGINE_RUNTIME_EXCEPTION(InvalidFileExtension, "Unknown file extension");
            PKENGINE_RUNTIME_EXCEPTION(FileNotFound, "File not found");
            PKENGINE_RUNTIME_EXCEPTION(UnableToReadFile, "Unable to read file");
            PKENGINE_RUNTIME_EXCEPTION(PNGUnableToStartRead, "PNG: Unable to start file read");
        };

        enum InputFormat {
            INFER_FROM_EXTENSION = 0,
            BMP,
            PNG,
        };

    public:
        static inline Vulkan::Image read_file(const char * path, InputFormat format = INFER_FROM_EXTENSION) {
            logger << "Reading image from \"" << path << "\"";

            if (format == INFER_FROM_EXTENSION) {
                logger << "Inferring image type from extension...";

                std::size_t path_length = std::strlen(path);

                std::size_t i;
                for (i = path_length - 1; path[i] != '.'; i--) {
                    if (i == 0) throw Exceptions::InvalidPath();
                }

                const char * extension_view = path + i + 1;
                const std::size_t extension_view_length = path_length - i - 1;

                char extension[extension_view_length + 1];

                for (std::size_t j = 0; j < extension_view_length; j++) {
                    if (extension_view[j] >= 'A' && extension_view[j] <= 'Z') extension[j] = (char) (extension_view[j] - 'A' + 'a');
                    else extension[j] = extension_view[j];
                }

                extension[extension_view_length] = '\0';

                if (std::strcmp(extension, "bmp") == 0) format = BMP;
                else if (std::strcmp(extension, "png") == 0) format = PNG;
                else {
                    logger.error() << "Unknown file extension detected \"" << (const char *) extension << "\"";
                    throw Exceptions::InvalidFileExtension();
                }
            }

            std::ifstream fs(path);

            if (!fs.is_open()) {
                throw Exceptions::FileNotFound();
            }

            switch (format) {
                case BMP: {
                    logger << "Reading BMP image";

                    ::BMP * bmp_img = bopen((char *) path);

                    Vulkan::Image image(
                        get_width(bmp_img),
                        get_height(bmp_img)
                    );

                    Vulkan::Pixel * pixels = image.bind_staging_buffer();
                    for (std::size_t x = 0; x < image.width(); x++) {
                        for (std::size_t y = 0; y < image.height(); y++) {
                            Vulkan::Pixel & pixel = pixels[y * image.width() + x];

                            get_pixel_rgb(bmp_img, (int) x, (int) y, &pixel.r, &pixel.g, &pixel.b);

                            pixel.a = 255;
                        }
                    }

                    image.unbind_staging_buffer();
                    image.commit_staging_buffer();

                    bclose(bmp_img);

                    logger << "Loaded image with size " << image.width() << " x " << image.height();

                    return image;
                }

                case PNG: {
                    logger << "Reading PNG image";

                    png_image png_img;
                    memset(&png_img, 0, sizeof(png_image));
                    png_img.version = PNG_IMAGE_VERSION;

                    if (png_image_begin_read_from_file(&png_img, path) != 0) {
                        Vulkan::Image image(
                            png_img.width,
                            png_img.height
                        );

                        png_img.format = PNG_FORMAT_RGBA;

                        if (png_image_finish_read(
                            &png_img,
                            nullptr,
                            image.bind_staging_buffer(),
                            0,
                            nullptr
                        ) == 0) {
                            png_image_free(&png_img);

                            image.unbind_staging_buffer();

                            throw Exceptions::UnableToReadFile();
                        }

                        image.unbind_staging_buffer();
                        image.commit_staging_buffer();

                        logger << "Loaded image with size " << image.width() << " x " << image.height();

                        return image;
                    }
                    else throw Exceptions::PNGUnableToStartRead();
                }

                default: throw Exceptions::InvalidFileExtension();
            }
        }
    };
}