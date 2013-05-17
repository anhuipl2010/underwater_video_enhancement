#include "ColorTuning.h"

#include <math.h>

namespace gstPluginWrap
{

void configure(Customizer*& customizer)
{
	customizer = new HandlerAndPropWithListenerCustomizer<ColorTuning, ColorTuningProperties>();
	customizer->getParameters = &ColorTuningProperties::getParameters;
}

} // namespace gstPluginWrap

// ColorTuningProperties

const double ColorTuningProperties::defaultCbExp = 1.0;
const double ColorTuningProperties::defaultCrExp = 1.0;

void ColorTuningProperties::getParameters(gstPluginWrap::ParamIdSpecMap& parameters)
{
	parameters[ARG_CB_EXP] = g_param_spec_double(
		"cbexp", "cb exponent", "Cb channel exponent",
		0.0, 1.0, defaultCbExp, (GParamFlags)G_PARAM_READWRITE);

	parameters[ARG_CR_EXP] = g_param_spec_double(
		"crexp", "cr exponent", "Cr channel exponent",
		0.0, 1.0, defaultCrExp, (GParamFlags)G_PARAM_READWRITE);
}

ColorTuningProperties::ColorTuningProperties()
{
	cbExp = defaultCbExp;
	crExp = defaultCrExp;

	initialParamIds.insert(ARG_CB_EXP);
	initialParamIds.insert(ARG_CR_EXP);
}

ColorTuningProperties::~ColorTuningProperties()
{
}

bool ColorTuningProperties::set(guint id, const GValue* val)
{
	switch (id)
	{
	case ARG_CB_EXP:
		cbExp = g_value_get_double(val);
		break;
	case ARG_CR_EXP:
		crExp = g_value_get_double(val);
		break;
	default:
		return false;
	}
	return ImagePropertyHolder::set(id, val);
}

bool ColorTuningProperties::get(guint id, GValue* val)
{
	switch (id)
	{
	case ARG_CB_EXP:
		g_value_set_double(val, cbExp);
		break;
	case ARG_CR_EXP:
		g_value_set_double(val, crExp);
		break;
	default:
		return false;
	}
	return ImagePropertyHolder::get(id, val);
}

// ColorTuning

ColorTuning::ColorTuning()
{
	for (int i = 0; i < 256; i++)
	{
		cbLut[i] = i;
		crLut[i] = i;
	}
}

ColorTuning::~ColorTuning()
{
}

void ColorTuning::propertyChanged(guint id)
{
	switch (id)
	{
	case ColorTuningProperties::ARG_CB_EXP:
		power(cbLut, 256, properties->cbExp, 128);
		break;
	case ColorTuningProperties::ARG_CR_EXP:
		power(crLut, 256, properties->crExp, 128);
		break;
	}
}

void ColorTuning::process(uint8_t* buffer)
{
	const int lumLen = properties->bufferWidth * properties->bufferHeight;

	const int chromWidth = properties->bufferWidth / 2;
	const int chromHeight = properties->bufferHeight / 2;
	const int chromLen = chromWidth * chromHeight;

	uint8_t* cb = buffer + lumLen;
	uint8_t* cr = buffer + lumLen + chromLen;

	for (int cnt = chromLen; cnt > 0; cnt--)
	{
		uint8_t& cbVal = *cb;
		uint8_t& crVal = *cr;

		cbVal = cbLut[cbVal];
		crVal = crLut[crVal];

		cb += 1;
		cr += 1;
	}
}

void ColorTuning::power(uint8_t* lut, int cnt, double e, int mid)
{
	double lmax = (double)mid;
	for (int i = 0; i < mid; i++)
	{
		double v = (mid - i) / lmax;
		v = mid - pow(v, e) * lmax;
		lut[i] = (uint8_t)(v + 0.5);
	}

	double rmax = (double)(cnt - mid - 1);
	for (int i = mid; i < cnt; i++)
	{
		double v = (i - mid) / rmax;
		v = mid + pow(v, e) * rmax;
		lut[i] = (uint8_t)(v + 0.5);
	}
}
