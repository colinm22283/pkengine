#include <pkengine/logger/file_stream.hpp>

#ifdef PKENGINE_LOG_FILE_ENABLE
std::ofstream PKEngine::_LoggerFileStream::log_file;
#endif