#include <pkengine/logger/logger.hpp>

PKEngine::Util::SpinLock PKEngine::_Logger_lock;
PKEngine::Util::TempFile PKEngine::_Logger_temp_file;