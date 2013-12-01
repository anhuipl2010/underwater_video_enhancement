#ifndef DUMMY_H_
#define DUMMY_H_

#include <gst/gst.h>

extern "C" {

    struct _GstDummy {
        GstElement element;
        GstPad* sinkpad;
        GstPad* srcpad;
    };

    struct _GstDummyClass {
        GstElementClass parent_class;
    };

    typedef struct _GstDummy GstDummy;
    typedef struct _GstDummyClass GstDummyClass;

    GType gst_dummy_get_type();

#define GST_TYPE_DUMMY \
    gst_dummy_get_type()

#define GST_DUMMY(obj) \
    G_TYPE_CHECK_INSTANCE_CAST(obj, GST_TYPE_DUMMY, GstDummy)

#define GST_IS_DUMMY(obj) \
    G_TYPE_CHECK_INSTANCE_TYPE(obj, GST_TYPE_DUMMY)

#define GST_DUMMY_CLASS(klass) \
    G_TYPE_CHECK_CLASS_CAST(klass, GST_TYPE_DUMMY, GstDummyClass)

#define GST_IS_DUMMY_CLASS(klass) \
    G_TYPE_CHECK_CLASS_TYPE(klass, GST_TYPE_DUMMY)

}

#endif // DUMMY_H_
