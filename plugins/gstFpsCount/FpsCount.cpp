#include "FpsCount.h"

namespace gstPluginWrap {

void configure(Customizer*& customizer) {
    customizer = new HandlerCustomizer<FpsCount>();
}

} // namespace gstPluginWrap

FpsCount::FpsCount() {
}

FpsCount::~FpsCount() {
}

void FpsCount::process(uint8_t* buffer) {
    if (fpsCounter.update())
        g_print("FPS: %d\n", fpsCounter.framesPerSec());
}
