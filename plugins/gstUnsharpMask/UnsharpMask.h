#ifndef UNSHARPMASK_H_
#define UNSHARPMASK_H_

#define GST_PLUGIN_TYPE GstUnsharpMask

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <gstPluginWrap.h>

namespace gstPluginWrap {

namespace details {
const char* name = "unsharpmask";
const char* longName = "Unsharp Mask filter";
const char* classification = "Generic/Filter";
const char* description = "Performs unsharp mask filtering.";
const char* originUrl = "https://github.com/vchernov/gstassist";
const char* author = "Vladimir Chernov <vladimir.chernov@student.uva.fi>";
const char* license = "LGPL";
const char* version = "0.2";
} // namespace details

const char* allowedCaps = "video/x-raw-yuv, format=(fourcc)I420; "
                          "video/x-raw-rgb, bpp=32, depth=24";

} // namespace gstPluginWrap

class UnsharpMaskProperties: public gstPluginWrap::ImagePropertyHolder {
public:
    enum Arguments {
        ARG_ENABLED = 1,
        ARG_RADIUS_CH1,
        ARG_AMOUNT_CH1,
        ARG_RADIUS_CH2,
        ARG_AMOUNT_CH2,
        ARG_RADIUS_CH3,
        ARG_AMOUNT_CH3,
    };

    enum ColorSpace {
        CS_UNKNOWN,
        CS_YUV,
        CS_RGB,
    };

    struct ChannelParam {
        double radius;
        double amount;
    };

    static const bool defaultEnabled;
    static const double defaultRadius;
    static const double defaultAmount;

    static void getParameters(gstPluginWrap::ParamIdSpecMap& parameters);

    UnsharpMaskProperties();
    virtual ~UnsharpMaskProperties();

    bool set(guint id, const GValue* val);
    bool get(guint id, GValue* val);

    void setMediaInfo(gchar* mime, GstStructure* params);

    bool enabled;
    ChannelParam channels[3];
    ColorSpace colorSpace;
};

class UnsharpMask:
    public gstPluginWrap::FrameHandler<UnsharpMaskProperties>,
    public gstPluginWrap::PropertyListener {
public:
    UnsharpMask();
    virtual ~UnsharpMask();

    void mediaInfoChanged();

    void process(uint8_t* buffer);

private:
    struct ChannelData {
        int offset;
        int width;
        int height;
        cv::Mat blurred;
    };

    ChannelData channelsData[3];
};

#endif // UNSHARPMASK_H_
