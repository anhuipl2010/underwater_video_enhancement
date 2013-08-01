#include "dummy.h"

/*
 * Information about the plugin.
 */
const gchar* pluginName = "dummy";
const gchar* pluginLongName = "Dummy";
const gchar* pluginClassification = "Generic/Filter";
const gchar* pluginDesc = "Does nothing.";
const gchar* pluginOrigin = "https://github.com/vchernov/gstAssist";
const gchar* pluginAuthor = "Vladimir Chernov <vladimir.chernov@student.uva.fi>";
const gchar* pluginLicense = "LGPL";
const gchar* pluginPackage = "";

#ifndef VERSION
#define VERSION "0.1.0"
#endif

// The GST_PLUGIN_DEFINE macro needs PACKAGE to be defined.
#ifndef PACKAGE
#define PACKAGE ""
#endif

/*
 * A GstStaticPadTemplate is a description of a pad that the element will (or might) create and use.
 * It contains:
 * 1. A short name for the pad.
 * 2. Pad direction.
 * 3. Existence property.
 * 4. Supported types by this element.
 */

/*
 * Sink pad (output).
 */
static GstStaticPadTemplate sinkTemplate = GST_STATIC_PAD_TEMPLATE(
	"sink",
	GST_PAD_SINK,
	GST_PAD_ALWAYS,
	GST_STATIC_CAPS_ANY
);

/*
 * Source pad (input).
 */
static GstStaticPadTemplate srcTemplate = GST_STATIC_PAD_TEMPLATE(
	"src",
	GST_PAD_SRC,
	GST_PAD_ALWAYS,
	GST_STATIC_CAPS_ANY
);

/*
 * Signals.
 */
enum
{
	LAST_SIGNAL,
};

/*
 * Arguments.
 */
enum
{
	ARG_0,
};

/*
 * The plugin_init() function is called as soon as the plugin is loaded.
 * It should return TRUE or FALSE depending on whether it is loaded correctly or not.
 * In this function, any supported element type in the plugin should be registered.
 */
static gboolean plugin_init(GstPlugin* plugin)
{
	return gst_element_register(plugin, pluginName, GST_RANK_NONE, GST_TYPE_DUMMY);
}

/*
 * GStreamer looks for the gst_plugin_desc structure to register the plugin.
 */
GST_PLUGIN_DEFINE(
	GST_VERSION_MAJOR,
	GST_VERSION_MINOR,
	pluginName,
	pluginDesc,
	plugin_init,
	VERSION,
	pluginLicense,
	pluginPackage,
	pluginOrigin
)

/*
 * Boilerplate functions declaration.
 * The GST_BOILERPLATE macro setups the GObject basics in this source file
 * so that all functions will be called appropriately.
 */
GST_BOILERPLATE(GstDummy, gst_dummy, GstElement, GST_TYPE_ELEMENT)
;
static GstFlowReturn gst_dummy_chain(GstPad* pad, GstBuffer* buf);
static GstStateChangeReturn gst_dummy_change_state(GstElement* element, GstStateChange transition);
static gboolean gst_dummy_setcaps(GstPad* pad, GstCaps* caps);
static void gst_dummy_set_property(GObject* obj, guint propId, const GValue* val, GParamSpec* pspec);
static void gst_dummy_get_property(GObject* obj, guint propId, GValue* val, GParamSpec* pspec);
static void gst_dummy_finalize(GObject* obj);

/*
 * The _init() function is used to initialize a specific instance of this type.
 */
static void gst_dummy_init(GstDummy* filter, GstDummyClass* filterClass)
{
	filter->sinkpad = gst_pad_new_from_static_template(&sinkTemplate, "sink");
	gst_pad_set_setcaps_function(filter->sinkpad, gst_dummy_setcaps);
	gst_pad_set_getcaps_function(filter->sinkpad, gst_pad_proxy_getcaps);
	gst_pad_set_chain_function(filter->sinkpad, gst_dummy_chain);

	filter->srcpad = gst_pad_new_from_static_template(&srcTemplate, "src");
	gst_pad_set_getcaps_function(filter->srcpad, gst_pad_proxy_getcaps);

	gst_element_add_pad(GST_ELEMENT(filter), filter->sinkpad);
	gst_element_add_pad(GST_ELEMENT(filter), filter->srcpad);
}

/*
 * The _class_init() function is used to initialize the class.
 * It is called only once and must specify what signals, arguments and virtual functions the class has.
 */
static void gst_dummy_class_init(GstDummyClass* filterClass)
{
	parent_class = (GstElementClass*)g_type_class_ref(GST_TYPE_ELEMENT);

	GObjectClass* gobjClass = G_OBJECT_CLASS(filterClass);
	gobjClass->set_property = gst_dummy_set_property;
	gobjClass->get_property = gst_dummy_get_property;
	gobjClass->finalize = gst_dummy_finalize;

	GstElementClass* elemClass = GST_ELEMENT_CLASS(filterClass);
	elemClass->change_state = gst_dummy_change_state;
}

static void gst_dummy_finalize(GObject* obj)
{
	G_OBJECT_CLASS(parent_class)->finalize(obj);
}

/*
 * The _base_init() function is meant to initialize class and child class properties
 * during each new child class creation.
 */
static void gst_dummy_base_init(gpointer ptr)
{
	GstElementClass* elemClass = GST_ELEMENT_CLASS(ptr);
	gst_element_class_set_details_simple(elemClass, pluginLongName, pluginClassification, pluginDesc, pluginAuthor);
	gst_element_class_add_pad_template(elemClass, gst_static_pad_template_get(&srcTemplate));
	gst_element_class_add_pad_template(elemClass, gst_static_pad_template_get(&sinkTemplate));
}

/*
 * The _set_property() function will be notified if an application changes the value of a property,
 * and can then take action required for that property to change value internally.
 */
static void gst_dummy_set_property(GObject* obj, guint propId, const GValue* val, GParamSpec* pspec)
{
}

/*
 * The _get_property() function will be notified if an application requests the value of a property,
 * and can then fill in the value.
 */
static void gst_dummy_get_property(GObject* obj, guint propId, GValue* val, GParamSpec* pspec)
{
}

/*
 * The _setcaps() function is called during caps negotiation.
 * This is the process where the linked pads decide on the streamtype that will transfer between them.
 */
static gboolean gst_dummy_setcaps(GstPad* pad, GstCaps* caps)
{
	GstDummy* filter = GST_DUMMY(gst_pad_get_parent(pad));
	GstPad* otherpad = (pad == filter->srcpad) ? filter->sinkpad : filter->srcpad;
	gst_object_unref(filter);
	return gst_pad_set_caps(otherpad, caps);
}

/*
 * The _change_state() is called when state is changed.
 */
static GstStateChangeReturn gst_dummy_change_state(GstElement* element, GstStateChange transition)
{
	GstStateChangeReturn ret = GST_STATE_CHANGE_SUCCESS;

	switch (transition)
	{
	case GST_STATE_CHANGE_NULL_TO_READY:
		break;
	default:
		break;
	}
	if (ret == GST_STATE_CHANGE_FAILURE)
		return ret;

	ret = GST_ELEMENT_CLASS(parent_class)->change_state(element, transition);
	if (ret == GST_STATE_CHANGE_FAILURE)
		return ret;

	switch (transition)
	{
	case GST_STATE_CHANGE_READY_TO_NULL:
		break;
	default:
		break;
	}
	return ret;
}

/*
 * The _chain() function is the function in which all data processing takes place.
 */
static GstFlowReturn gst_dummy_chain(GstPad* pad, GstBuffer* buf)
{
	GstDummy* filter = GST_DUMMY(GST_OBJECT_PARENT(pad));
	return gst_pad_push(filter->srcpad, buf);
}
