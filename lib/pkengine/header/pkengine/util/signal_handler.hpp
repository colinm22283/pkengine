#pragma once

#pragma once

#include <csignal>

namespace PKEngine::Util {
    class SignalHandler {
    public:
        enum Signal : int {
            INTERRUPT = SIGINT,
            TERMINATE = SIGTERM,
            BUS_ERROR = SIGBUS,
            ILLEGAL_COMMAND = SIGILL,
            ALARM = SIGALRM,
            ABORT = SIGABRT,
            STOP = SIGSTOP,
            SEGMENTATION_FAULT = SIGSEGV,
            FP_ERROR = SIGFPE,
            USER_DEFINED_1 = SIGUSR1,
            USER_DEFINED_2 = SIGUSR2,

            COUNT = 11,
        };

        inline SignalHandler(Signal signal, void (* handler)(Signal)) {
            std::signal(signal, (void (*)(int)) handler);
        }
    };
}