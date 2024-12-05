#include <render_config.hpp>

static_assert(PKEngine::Config::render_config.max_frames_in_flight > 0, "Max frames in flight must be grater than zero");