#ifndef FPSCOUNT_H_
#define FPSCOUNT_H_

#define GST_PLUGIN_TYPE GstFpsCount

#include <timeutil/FpsCounter.h>

#include <gstPluginWrap.h>

namespace gstPluginWrap {

namespace details {
const char* name = "fpscount";
const char* longName = "FPS counter";
const char* classification = "Generic";
const char* description = "Counts frames per second.";
const char* originUrl = "https://github.com/vchernov/underwater_video_enhancement";
const char* author = "Vladimir Chernov <vladimir.chernov@student.uva.fi>";
const char* license = "LGPL";
const char* version = "0.2";
} // namespace details

const char* allowedCaps = "ANY";

} // namespace gstPluginWrap

class FpsCount: public gstPluginWrap::PureFrameHandler {
public:
    FpsCount();
    virtual ~FpsCount();

    void process(uint8_t* buffer);

private:
    timeutil::FpsCounter fpsCounter;
};

#endif // FPSCOUNT_H_
