#ifndef UNSHARPMASK_H_
#define UNSHARPMASK_H_

#define GST_PLUGIN_TYPE GstUnsharpMask

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <gstPluginWrap.h>

namespace gstPluginWrap
{

namespace details
{
const char* name = "unsharpmask";
const char* longName = "Unsharp Mask filter";
const char* classification = "Generic/Filter";
const char* description = "Performs unsharp mask filtering.";
const char* originUrl = "https://github.com/vchernov/gstAssist";
const char* author = "Vladimir Chernov <vladimir.chernov@student.uva.fi>";
const char* license = "LGPL";
const char* version = "0.1";
} // namespace details

const char* allowedCaps = "video/x-raw-yuv, format=(fourcc)I420; "
	"video/x-raw-rgb, bpp=32, depth=24";

} // namespace gstPluginWrap

class UnsharpMaskProperties: public gstPluginWrap::ImagePropertyHolder
{
public:
	enum Arguments
	{
		ARG_SIGMA = 1,
		ARG_ALPHA,
		ARG_BETA,
	};

	static const double defaultSigma;
	static const double defaultAlpha;
	static const double defaultBeta;

	static void getParameters(gstPluginWrap::ParamIdSpecMap& parameters);

	UnsharpMaskProperties();
	virtual ~UnsharpMaskProperties();

	bool set(guint id, const GValue* val);
	bool get(guint id, GValue* val);

	void setMediaInfo(gchar* mime, GstStructure* params);

	double sigma;
	double alpha;
	double beta;

	int imgType;
};

class UnsharpMask:
	public gstPluginWrap::FrameHandler<UnsharpMaskProperties>,
	public gstPluginWrap::PropertyListener
{
public:
	UnsharpMask();
	virtual ~UnsharpMask();

	void mediaInfoChanged();

	void process(uint8_t* buffer);

private:
	cv::Mat blurred;
};

#endif // UNSHARPMASK_H_
