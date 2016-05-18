#ifndef MEDIANFILTER_H_
#define MEDIANFILTER_H_

#define GST_PLUGIN_TYPE GstMedFilt

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <gstPluginWrap.h>

namespace gstPluginWrap {

namespace details {
const char* name = "medfilt";
const char* longName = "Median filter";
const char* classification = "Generic/Filter";
const char* description = "Performs median filtering.";
const char* originUrl = "https://github.com/vchernov/gstassist";
const char* author = "Vladimir Chernov <vladimir.chernov@student.uva.fi>";
const char* license = "LGPL";
const char* version = "0.2";
} // details

const char* allowedCaps = "video/x-raw-yuv, format=(fourcc)I420; "
                          "video/x-raw-rgb, bpp=32, depth=24";

} // namespace gstPluginWrap

class MedianFilterProperties: public gstPluginWrap::ImagePropertyHolder {
public:
    enum Arguments {
        ARG_ENABLED = 1,
        ARG_RADIUS_CH1,
        ARG_RADIUS_CH2,
        ARG_RADIUS_CH3,
    };

    enum ColorSpace {
        CS_UNKNOWN,
        CS_YUV,
        CS_RGB,
    };

    static const bool defaultEnabled;
    static const int defaultRadius;

    static void getParameters(gstPluginWrap::ParamIdSpecMap& parameters);

    MedianFilterProperties();
    virtual ~MedianFilterProperties();

    void setMediaInfo(gchar* mime, GstStructure* params);

    bool set(guint id, const GValue* val);
    bool get(guint id, GValue* val);

    bool enabled;
    int radius[3];
    ColorSpace colorSpace;
};

class MedianFilter: public gstPluginWrap::FrameHandler<MedianFilterProperties> {
public:
    MedianFilter();
    virtual ~MedianFilter();

    void process(uint8_t* buffer);
};

#endif // MEDIANFILTER_H_
