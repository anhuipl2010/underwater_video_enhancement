#include "MedianFilter.h"

#include <string.h>

namespace gstPluginWrap
{

void configure(Customizer*& customizer)
{
	customizer = new HandlerAndPropCustomizer<MedianFilter, MedianFilterProperties>();
	customizer->getParameters = &MedianFilterProperties::getParameters;
}

} // namespace gstPluginWrap

// MedianFilterProperties

const int MedianFilterProperties::defaultRadius = 0;

void MedianFilterProperties::getParameters(gstPluginWrap::ParamIdSpecMap& parameters)
{
	parameters[ARG_RADIUS] = g_param_spec_int("radius", "Kernel radius", "Radius of median filter kernel",
		0, INT_MAX, defaultRadius, (GParamFlags)G_PARAM_READWRITE);
}

MedianFilterProperties::MedianFilterProperties()
{
	radius = defaultRadius;

	imgType = CV_8UC1;
}

MedianFilterProperties::~MedianFilterProperties()
{
}

void MedianFilterProperties::setMediaInfo(gchar* mime, GstStructure* params)
{
	imgType = (strcmp(mime, "video/x-raw-rgb") == 0) ? CV_8UC4 : CV_8UC1;

	ImagePropertyHolder::setMediaInfo(mime, params);
}

bool MedianFilterProperties::set(guint id, const GValue* val)
{
	switch (id)
	{
	case ARG_RADIUS:
		radius = g_value_get_int(val);
		break;
	default:
		return false;
	}
	return ImagePropertyHolder::set(id, val);
}

bool MedianFilterProperties::get(guint id, GValue* val)
{
	switch (id)
	{
	case ARG_RADIUS:
		g_value_set_int(val, radius);
		break;
	default:
		return false;
	}
	return ImagePropertyHolder::get(id, val);
}

// MedianFilter

MedianFilter::MedianFilter()
{
}

MedianFilter::~MedianFilter()
{
}

void MedianFilter::process(uint8_t* buffer)
{
	cv::Mat img(properties->bufferHeight, properties->bufferWidth, properties->imgType, buffer);
	cv::medianBlur(img, img, properties->radius + properties->radius + 1);
}
