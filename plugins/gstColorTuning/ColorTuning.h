#ifndef COLORTUNING_H_
#define COLORTUNING_H_

#define GST_PLUGIN_TYPE GstColorTuning

#include <gstPluginWrap.h>

namespace gstPluginWrap {

namespace details {
const char* name = "colortuning";
const char* longName = "Color tuning filter";
const char* classification = "Generic/Filter";
const char* description = "Performs color tuning of frames.";
const char* originUrl = "https://github.com/vchernov/underwater_video_enhancement";
const char* author = "Vladimir Chernov <vladimir.chernov@student.uva.fi>";
const char* license = "LGPL";
const char* version = "0.1";
} // namespace details

const char* allowedCaps = "video/x-raw-yuv, format=(fourcc)I420";

} // namespace gstPluginWrap

class ColorTuningProperties: public gstPluginWrap::ImagePropertyHolder {
public:
    enum Arguments {
        ARG_ENABLED = 1,
        ARG_CB_EXP,
        ARG_CR_EXP,
    };

    static const bool defaultEnabled;
    static const double defaultCbExp;
    static const double defaultCrExp;

    static void getParameters(gstPluginWrap::ParamIdSpecMap& parameters);

    ColorTuningProperties();
    virtual ~ColorTuningProperties();

    bool set(guint id, const GValue* val);
    bool get(guint id, GValue* val);

    bool enabled;
    double cbExp;
    double crExp;
};

class ColorTuning:
    public gstPluginWrap::FrameHandler<ColorTuningProperties>,
    public gstPluginWrap::PropertyListener {
public:
    ColorTuning();
    virtual ~ColorTuning();

    void propertyChanged(guint id);

    void process(uint8_t* buffer);

private:
    void power(uint8_t* lut, int cnt, double e, int mid);

    uint8_t cbLut[256];
    uint8_t crLut[256];
};

#endif // COLORTUNING_H_
