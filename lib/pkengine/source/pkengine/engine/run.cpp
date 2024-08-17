#include <pkengine/engine.hpp>

bool PKEngine::engine_instance::run() noexcept {
    logger << "Starting PKEngine...";

    static auto do_cleanup = []() {
        try {
            free();
        }
        catch (const std::exception & ex) {
            logger.error() << "Exception occurred during PKEngine cleanup";
            logger.error() << "what(): " << ex.what();

            return false;
        }

        return true;
    };

    try {
        Time::start();

        init();
    }
    catch (const Exception::InternalException & ex) {
        logger.error() << "Exception occurred during PKEngine initialization";
        logger.error() << "\tWhat: " << ex.what();
        if (ex.is_glfw_error()) {
            logger.error() << "\tGLFW Error String: " << GLFW::glfw_error_string;
        }

        do_cleanup();

        return false;
    }

    try {
        ::init();
    }
    catch (const std::exception & ex) {
        logger.error() << "Exception occurred during user code initialization";
        logger.error() << "What(): " << ex.what();

        do_cleanup();

        return false;
    }


    try {
        object_tree.start();
    }
    catch (const std::exception & ex) {
        logger.error() << "Exception occurred during user code start";
        logger.error() << "What(): " << ex.what();

        do_cleanup();

        return false;
    }

    try {
        enter_main_loop();
    }
    catch (const std::exception & ex) {
        logger.error() << "Exception occurred during user code update";
        logger.error() << "what(): " << ex.what();

        do_cleanup();

        return false;
    }

    if (!do_cleanup()) return false;

    logger.success() << "PKEngine exited with no internal errors";

    return true;
}