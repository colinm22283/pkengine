#include <pkengine/time.hpp>

PKEngine::Time::clock::time_point PKEngine::Time::start_time;
PKEngine::Time::clock::time_point PKEngine::Time::last_update_time;
PKEngine::Time::duration PKEngine::Time::_delta_time;