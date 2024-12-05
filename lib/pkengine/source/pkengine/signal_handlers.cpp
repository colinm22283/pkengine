#include <execinfo.h>

#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>

#include <pkengine/util/signal_handler.hpp>
#include <pkengine/util/stack_trace.hpp>

#include <pkengine/logger/logger.hpp>

using namespace PKEngine;
using namespace Util;

#ifdef ENABLE_SIGSEGV_HANDLER
const auto seg_fault_handler = SignalHandler(
    SignalHandler::Signal::SEGMENTATION_FAULT,
    [](SignalHandler::Signal) {
        static constexpr auto logger = Logger<"SIGSEGV">().error();

        logger << "Received SIGSEGV signal";
        logger << "Stack trace:";

        static StackTrace<1024> stack_trace;

        static constexpr auto indented_logger = logger.indent();
        for (const auto & trace : stack_trace) {
            if (trace.name[0] == '\0') indented_logger << trace.file << "(" << std::hex << trace.address << ")";
            else indented_logger << trace.file << "(" << std::hex << trace.address << ") " << trace.name;
        }

        if (!std::filesystem::exists("logs")) std::filesystem::create_directory("logs");

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "logs/%m-%d-%Y--%H-%M-%S.log");

        logger.error() << "";
        logger.error() << "Dumped debug log to '" << oss.view() << "'";

        dump_log_to(oss.str().c_str());

        ::kill(getpid(), SIGKILL);

        while (true);

//        std::exit(SignalHandler::Signal::SEGMENTATION_FAULT);
    }
);
#endif