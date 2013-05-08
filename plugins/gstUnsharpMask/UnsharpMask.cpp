#include "UnsharpMask.h"

namespace gstPluginWrap
{

void configure(Customizer*& customizer)
{
	customizer = new HandlerAndPropWithListenerCustomizer<UnsharpMask, UnsharpMaskProperties>();
	customizer->getParameters = &UnsharpMaskProperties::getParameters;
}

} // namespace gstPluginWrap

// UnsharpMaskProperties

const double UnsharpMaskProperties::defaultSigma = 1.0;
const double UnsharpMaskProperties::defaultAlpha = 1.0;
const double UnsharpMaskProperties::defaultBeta = 0.0;

void UnsharpMaskProperties::getParameters(gstPluginWrap::ParamIdSpecMap& parameters)
{
	parameters[ARG_SIGMA] = g_param_spec_double("sigma", "Sigma", "Gaussian kernel standard deviation",
		DBL_MIN, DBL_MAX, defaultSigma, (GParamFlags)G_PARAM_READWRITE);

	parameters[ARG_ALPHA] = g_param_spec_double("alpha", "Alpha", "Weight of the original image elements",
		-DBL_MAX, DBL_MAX, defaultAlpha, (GParamFlags)G_PARAM_READWRITE);

	parameters[ARG_BETA] = g_param_spec_double("beta", "Beta", "Weight of the blurred image elements",
		-DBL_MAX, DBL_MAX, defaultBeta, (GParamFlags)G_PARAM_READWRITE);
}

UnsharpMaskProperties::UnsharpMaskProperties()
{
	sigma = defaultSigma;
	alpha = defaultAlpha;
	beta = defaultBeta;

	imgType = CV_8UC1;
}

UnsharpMaskProperties::~UnsharpMaskProperties()
{
}

void UnsharpMaskProperties::setMediaInfo(gchar* mime, GstStructure* params)
{
	imgType = (strcmp(mime, "video/x-raw-rgb") == 0) ? CV_8UC4 : CV_8UC1;

	ImagePropertyHolder::setMediaInfo(mime, params);
}

bool UnsharpMaskProperties::set(guint id, const GValue* val)
{
	switch (id)
	{
	case ARG_SIGMA:
		sigma = g_value_get_double(val);
		break;
	case ARG_ALPHA:
		alpha = g_value_get_double(val);
		break;
	case ARG_BETA:
		beta = g_value_get_double(val);
		break;
	default:
		return false;
	}
	return ImagePropertyHolder::set(id, val);
}

bool UnsharpMaskProperties::get(guint id, GValue* val)
{
	switch (id)
	{
	case ARG_SIGMA:
		g_value_set_double(val, sigma);
		break;
	case ARG_ALPHA:
		g_value_set_double(val, alpha);
		break;
	case ARG_BETA:
		g_value_set_double(val, beta);
		break;
	default:
		return false;
	}
	return ImagePropertyHolder::get(id, val);
}

// UnsharpMask

UnsharpMask::UnsharpMask()
{
}

UnsharpMask::~UnsharpMask()
{
}

void UnsharpMask::mediaInfoChanged()
{
	blurred.create(properties->bufferHeight, properties->bufferWidth, properties->imgType);
}

void UnsharpMask::process(uint8_t* buffer)
{
	cv::Mat img(properties->bufferHeight, properties->bufferWidth, properties->imgType, buffer);
	cv::GaussianBlur(img, blurred, cv::Size(), properties->sigma);
	cv::addWeighted(img, properties->alpha, blurred, properties->beta, 0, img);
}
