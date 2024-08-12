#include <execinfo.h>

#include <ctime>
#include <sstream>
#include <iomanip>
#include <filesystem>

#include <pkengine/util/signal_handler.hpp>

#include <pkengine/logger/logger.hpp>

using namespace PKEngine;
using namespace Util;

const auto seg_fault_handler = SignalHandler(
    SignalHandler::Signal::SEGMENTATION_FAULT,
    [](SignalHandler::Signal) {
        static constexpr auto logger = Logger<"SIGSEGV">().error();

        logger << "Received SIGSEGV signal";

        static void * call_stack[1024];

        backtrace(call_stack, 1024);

        std::size_t call_stack_entries = 0;
        while (call_stack[call_stack_entries] != nullptr) call_stack_entries++;

        char ** symbols = backtrace_symbols(call_stack, 1024);

        logger << "Stack trace:";
        static constexpr auto indented_logger = logger.indent();
        for (std::size_t i = 0; i < call_stack_entries; i++) {
            indented_logger << symbols[i];
        }

        free(symbols);

        if (!std::filesystem::exists("logs")) std::filesystem::create_directory("logs");

        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        std::ostringstream oss;
        oss << std::put_time(&tm, "logs/%m-%d-%Y--%H-%M-%S.log");

        dump_log_to(oss.str().c_str());

        ::kill(getpid(), SIGKILL);

//        std::exit(SignalHandler::Signal::SEGMENTATION_FAULT);
    }
);