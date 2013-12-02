#include "UnsharpMask.h"

namespace gstPluginWrap {

void configure(Customizer*& customizer) {
    customizer = new HandlerAndPropWithListenerCustomizer<UnsharpMask, UnsharpMaskProperties>();
    customizer->getParameters = &UnsharpMaskProperties::getParameters;
}

} // namespace gstPluginWrap

// UnsharpMaskProperties

const double UnsharpMaskProperties::defaultRadius = 1.0;
const double UnsharpMaskProperties::defaultAmount = 0.0;

void UnsharpMaskProperties::getParameters(gstPluginWrap::ParamIdSpecMap& parameters) {
    parameters[ARG_RADIUS_CH1] = g_param_spec_double(
                                     "radius1",
                                     "Radius 1",
                                     "Gaussian kernel standard deviation for channel 1.",
                                     DBL_MIN, DBL_MAX, defaultRadius,
                                     (GParamFlags)G_PARAM_READWRITE);

    parameters[ARG_AMOUNT_CH1] = g_param_spec_double(
                                     "amount1",
                                     "Amount 1",
                                     "Weight of the sum of original and blurred images for channel 1.",
                                     0.0, 1.0, defaultAmount,
                                     (GParamFlags)G_PARAM_READWRITE);

    parameters[ARG_RADIUS_CH2] = g_param_spec_double(
                                     "radius2",
                                     "Radius 2",
                                     "Gaussian kernel standard deviation for channel 2.",
                                     DBL_MIN, DBL_MAX, defaultRadius,
                                     (GParamFlags)G_PARAM_READWRITE);

    parameters[ARG_AMOUNT_CH2] = g_param_spec_double(
                                     "amount2",
                                     "Amount 2",
                                     "Weight of the sum of original and blurred images for channel 2.",
                                     0.0, 1.0, defaultAmount,
                                     (GParamFlags)G_PARAM_READWRITE);

    parameters[ARG_RADIUS_CH3] = g_param_spec_double(
                                     "radius3",
                                     "Radius 3",
                                     "Gaussian kernel standard deviation for channel 3.",
                                     DBL_MIN, DBL_MAX, defaultRadius,
                                     (GParamFlags)G_PARAM_READWRITE);

    parameters[ARG_AMOUNT_CH3] = g_param_spec_double(
                                     "amount3",
                                     "Amount 3",
                                     "Weight of the sum of original and blurred images for channel 3.",
                                     0.0, 1.0, defaultAmount,
                                     (GParamFlags)G_PARAM_READWRITE);
}

UnsharpMaskProperties::UnsharpMaskProperties() {
    channels[0].radius = defaultRadius;
    channels[0].amount = defaultAmount;
    channels[1].radius = defaultRadius;
    channels[1].amount = defaultAmount;
    channels[2].radius = defaultRadius;
    channels[2].amount = defaultAmount;

    colorSpace = CS_UNKNOWN;
}

UnsharpMaskProperties::~UnsharpMaskProperties() {
}

void UnsharpMaskProperties::setMediaInfo(gchar* mime, GstStructure* params) {
    if (strcmp(mime, "video/x-raw-yuv") == 0)
        colorSpace = CS_YUV;

    if (strcmp(mime, "video/x-raw-rgb") == 0)
        colorSpace = CS_RGB;

    ImagePropertyHolder::setMediaInfo(mime, params);
}

bool UnsharpMaskProperties::set(guint id, const GValue* val) {
    switch (id) {
    case ARG_RADIUS_CH1:
        channels[0].radius = g_value_get_double(val);
        break;
    case ARG_AMOUNT_CH1:
        channels[0].amount = g_value_get_double(val);
        break;
    case ARG_RADIUS_CH2:
        channels[1].radius = g_value_get_double(val);
        break;
    case ARG_AMOUNT_CH2:
        channels[1].amount = g_value_get_double(val);
        break;
    case ARG_RADIUS_CH3:
        channels[2].radius = g_value_get_double(val);
        break;
    case ARG_AMOUNT_CH3:
        channels[2].amount = g_value_get_double(val);
        break;
    default:
        return false;
    }
    return ImagePropertyHolder::set(id, val);
}

bool UnsharpMaskProperties::get(guint id, GValue* val) {
    switch (id) {
    case ARG_RADIUS_CH1:
        g_value_set_double(val, channels[0].radius);
        break;
    case ARG_AMOUNT_CH1:
        g_value_set_double(val, channels[0].amount);
        break;
    case ARG_RADIUS_CH2:
        g_value_set_double(val, channels[1].radius);
        break;
    case ARG_AMOUNT_CH2:
        g_value_set_double(val, channels[1].amount);
        break;
    case ARG_RADIUS_CH3:
        g_value_set_double(val, channels[2].radius);
        break;
    case ARG_AMOUNT_CH3:
        g_value_set_double(val, channels[2].amount);
        break;
    default:
        return false;
    }
    return ImagePropertyHolder::get(id, val);
}

// UnsharpMask

UnsharpMask::UnsharpMask() {
    for (int i = 0; i < 3; i++) {
        ChannelData& data = channelsData[i];
        data.offset = 0;
        data.width = 0;
        data.height = 0;
    }
}

UnsharpMask::~UnsharpMask() {
}

void UnsharpMask::mediaInfoChanged() {
    if (properties->colorSpace == UnsharpMaskProperties::CS_YUV) {
        const int lumLen = properties->bufferWidth * properties->bufferHeight;

        const int chromWidth = properties->bufferWidth / 2;
        const int chromHeight = properties->bufferHeight / 2;
        const int chromLen = chromWidth * chromHeight;

        channelsData[0].blurred.create(properties->bufferHeight, properties->bufferWidth, CV_8UC1);
        channelsData[0].offset = 0;
        channelsData[0].width = properties->bufferWidth;
        channelsData[0].height = properties->bufferHeight;

        channelsData[1].blurred.create(chromHeight, chromWidth, CV_8UC1);
        channelsData[1].offset = lumLen;
        channelsData[1].width = chromWidth;
        channelsData[1].height = chromHeight;

        channelsData[2].blurred = channelsData[1].blurred;
        channelsData[2].offset = lumLen + chromLen;
        channelsData[2].width = chromWidth;
        channelsData[2].height = chromHeight;
    }

    if (properties->colorSpace == UnsharpMaskProperties::CS_RGB) {
        cv::Mat blurred(properties->bufferHeight, properties->bufferWidth, CV_8UC1);
        int len = properties->bufferWidth * properties->bufferHeight;

        channelsData[0].blurred = blurred;
        channelsData[0].offset = 0;
        channelsData[0].width = properties->bufferWidth;
        channelsData[0].height = properties->bufferHeight;

        channelsData[1].blurred = blurred;
        channelsData[1].offset = len;
        channelsData[1].width = properties->bufferWidth;
        channelsData[1].height = properties->bufferHeight;

        channelsData[2].blurred = blurred;
        channelsData[2].offset = len + len;
        channelsData[2].width = properties->bufferWidth;
        channelsData[2].height = properties->bufferHeight;
    }
}

void UnsharpMask::process(uint8_t* buffer) {
    for (int i = 0; i < 3; i++) {
        UnsharpMaskProperties::ChannelParam& params = properties->channels[i];
        if (params.amount == 0.0)
            continue;

        ChannelData& data = channelsData[i];
        cv::Mat img(data.height, data.width, CV_8UC1, buffer + data.offset);
        cv::GaussianBlur(img, data.blurred, cv::Size(), params.radius);
        cv::addWeighted(img, 1.0 + params.amount, data.blurred, -params.amount, 0, img);
    }
}
