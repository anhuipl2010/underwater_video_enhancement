#include "MedianFilter.h"

#include <string.h>

namespace gstPluginWrap {

void configure(Customizer*& customizer) {
    customizer = new HandlerAndPropCustomizer<MedianFilter, MedianFilterProperties>();
    customizer->getParameters = &MedianFilterProperties::getParameters;
}

} // namespace gstPluginWrap

// MedianFilterProperties

const int MedianFilterProperties::defaultRadius = 0;

void MedianFilterProperties::getParameters(gstPluginWrap::ParamIdSpecMap& parameters) {
    parameters[ARG_RADIUS_CH1] = g_param_spec_int(
                                     "radius1",
                                     "Kernel radius 1",
                                     "Radius of median filter kernel for channel 1",
                                     0, INT_MAX, defaultRadius,
                                     (GParamFlags)G_PARAM_READWRITE);

    parameters[ARG_RADIUS_CH2] = g_param_spec_int(
                                     "radius2",
                                     "Kernel radius 2",
                                     "Radius of median filter kernel for channel 2",
                                     0, INT_MAX, defaultRadius,
                                     (GParamFlags)G_PARAM_READWRITE);

    parameters[ARG_RADIUS_CH3] = g_param_spec_int(
                                     "radius3",
                                     "Kernel radius 3",
                                     "Radius of median filter kernel for channel 3",
                                     0, INT_MAX, defaultRadius,
                                     (GParamFlags)G_PARAM_READWRITE);
}

MedianFilterProperties::MedianFilterProperties() {
    radius[0] = defaultRadius;
    radius[1] = defaultRadius;
    radius[2] = defaultRadius;

    colorSpace = CS_UNKNOWN;
}

MedianFilterProperties::~MedianFilterProperties() {
}

void MedianFilterProperties::setMediaInfo(gchar* mime, GstStructure* params) {
    if (strcmp(mime, "video/x-raw-yuv") == 0)
        colorSpace = CS_YUV;

    if (strcmp(mime, "video/x-raw-rgb") == 0)
        colorSpace = CS_RGB;

    ImagePropertyHolder::setMediaInfo(mime, params);
}

bool MedianFilterProperties::set(guint id, const GValue* val) {
    switch (id) {
    case ARG_RADIUS_CH1:
        radius[0] = g_value_get_int(val);
        break;
    case ARG_RADIUS_CH2:
        radius[1] = g_value_get_int(val);
        break;
    case ARG_RADIUS_CH3:
        radius[2] = g_value_get_int(val);
        break;
    default:
        return false;
    }
    return ImagePropertyHolder::set(id, val);
}

bool MedianFilterProperties::get(guint id, GValue* val) {
    switch (id) {
    case ARG_RADIUS_CH1:
        g_value_set_int(val, radius[0]);
        break;
    case ARG_RADIUS_CH2:
        g_value_set_int(val, radius[1]);
        break;
    case ARG_RADIUS_CH3:
        g_value_set_int(val, radius[2]);
        break;
    default:
        return false;
    }
    return ImagePropertyHolder::get(id, val);
}

// MedianFilter

MedianFilter::MedianFilter() {
}

MedianFilter::~MedianFilter() {
}

void MedianFilter::process(uint8_t* buffer) {
    cv::Mat ch1, ch2, ch3;

    if (properties->colorSpace == MedianFilterProperties::CS_YUV) {
        const int lumLen = properties->bufferWidth * properties->bufferHeight;

        const int chromWidth = properties->bufferWidth / 2;
        const int chromHeight = properties->bufferHeight / 2;
        const int chromLen = chromWidth * chromHeight;

        ch1 = cv::Mat(properties->bufferHeight, properties->bufferWidth, CV_8UC1, buffer);
        ch2 = cv::Mat(chromHeight, chromWidth, CV_8UC1, buffer + lumLen);
        ch3 = cv::Mat(chromHeight, chromWidth, CV_8UC1, buffer + lumLen + chromLen);
    }

    if (properties->colorSpace == MedianFilterProperties::CS_RGB) {
        const int chLen = properties->bufferWidth * properties->bufferHeight;

        ch1 = cv::Mat(properties->bufferHeight, properties->bufferWidth, CV_8UC1,
                      buffer);
        ch2 = cv::Mat(properties->bufferHeight, properties->bufferWidth, CV_8UC1,
                      buffer + chLen);
        ch3 = cv::Mat(properties->bufferHeight, properties->bufferWidth, CV_8UC1,
                      buffer + chLen + chLen);
    }

    if (properties->radius[0] > 0)
        cv::medianBlur(ch1, ch1, properties->radius[0] + properties->radius[0] + 1);
    if (properties->radius[1] > 0)
        cv::medianBlur(ch2, ch2, properties->radius[1] + properties->radius[1] + 1);
    if (properties->radius[2] > 0)
        cv::medianBlur(ch3, ch3, properties->radius[2] + properties->radius[2] + 1);
}
