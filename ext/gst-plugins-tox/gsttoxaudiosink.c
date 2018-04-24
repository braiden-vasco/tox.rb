#include "gsttoxaudiosink.h"

GST_DEBUG_CATEGORY_STATIC(gst_tox_audio_sink_debug);
#define GST_CAT_DEFAULT gst_tox_audio_sink_debug

G_DEFINE_TYPE(GstToxAudioSink, gst_tox_audio_sink, GST_TYPE_AUDIO_SINK);

/******************************************************************************
 * Types and constants
 ******************************************************************************/

enum {
  PROP_0,
};

/******************************************************************************
 * Function declarations
 ******************************************************************************/

static void gst_tox_audio_sink_class_init(GstToxAudioSinkClass *klass);

static void gst_tox_audio_sink_init(GstToxAudioSink *self);

static void gst_tox_audio_sink_set_property(
  GObject *object,
  guint prop_id,
  const GValue *value,
  GParamSpec *pspec
);

static void gst_tox_audio_sink_get_property(
  GObject *object,
  guint prop_id,
  GValue *value,
  GParamSpec *pspec
);

static gboolean gst_tox_audio_sink_open(GstAudioSink *gst_audio_sink);

static gboolean gst_tox_audio_sink_prepare(
  GstAudioSink *gst_audio_sink,
  GstAudioRingBufferSpec *gst_audio_ring_buffer_spec
);

static gboolean gst_tox_audio_sink_unprepare(GstAudioSink *gst_audio_sink);

static gboolean gst_tox_audio_sink_close(GstAudioSink *gst_audio_sink);

static gint gst_tox_audio_sink_write(
  GstAudioSink *gst_audio_sink,
  gpointer data,
  guint length
);

static void gst_tox_audio_sink_reset(GstAudioSink *gst_audio_sink);

static guint gst_tox_audio_sink_delay(GstAudioSink *gst_audio_sink);

/******************************************************************************
 * Variables
 ******************************************************************************/

static GstStaticPadTemplate sink_template = GST_STATIC_PAD_TEMPLATE(
  "sink",
  GST_PAD_SINK,
  GST_PAD_ALWAYS,
  GST_STATIC_CAPS(
    "audio/x-opus, "
    "rate = (int) 48000, "
    "channels = (int) 1, "
    "channel-mapping-family = (int) 0, "
    "stream-count = (int) 1, "
    "coupled-count = (int) 0"
  )
);

/******************************************************************************
 * Function implementations
 ******************************************************************************/

void gst_tox_audio_sink_class_init(GstToxAudioSinkClass *const klass)
{
  GST_DEBUG_CATEGORY_INIT(
    gst_tox_audio_sink_debug,
    "toxaudiosink",
    0,
    "toxaudiosink element"
  );

  GObjectClass      *const gobject_class        = (GObjectClass*)klass;
  GstElementClass   *const gst_element_class    = (GstElementClass*)klass;
  GstAudioSinkClass *const gst_audio_sink_class = (GstAudioSinkClass*)klass;

  gobject_class->set_property = GST_DEBUG_FUNCPTR(gst_tox_audio_sink_set_property);
  gobject_class->get_property = GST_DEBUG_FUNCPTR(gst_tox_audio_sink_get_property);

  gst_audio_sink_class->open      = GST_DEBUG_FUNCPTR(gst_tox_audio_sink_open);
  gst_audio_sink_class->prepare   = GST_DEBUG_FUNCPTR(gst_tox_audio_sink_prepare);
  gst_audio_sink_class->unprepare = GST_DEBUG_FUNCPTR(gst_tox_audio_sink_unprepare);
  gst_audio_sink_class->close     = GST_DEBUG_FUNCPTR(gst_tox_audio_sink_close);
  gst_audio_sink_class->write     = GST_DEBUG_FUNCPTR(gst_tox_audio_sink_write);
  gst_audio_sink_class->reset     = GST_DEBUG_FUNCPTR(gst_tox_audio_sink_reset);
  gst_audio_sink_class->delay     = GST_DEBUG_FUNCPTR(gst_tox_audio_sink_delay);

  gst_element_class_set_details_simple(
    gst_element_class,
    "ToxAudioSink",
    "Sink/Audio",
    "Sends Opus audio to Tox",
    "Braiden Vasco <braiden-vasco@users.noreply.github.com>"
  );

  gst_element_class_add_pad_template(
    gst_element_class,
    gst_static_pad_template_get(&sink_template)
  );
}

void gst_tox_audio_sink_init(GstToxAudioSink *const self)
{
}

void gst_tox_audio_sink_set_property(
  GObject *const object,
  const guint prop_id,
  const GValue *const value,
  GParamSpec *const pspec
)
{
  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
  }
}

void gst_tox_audio_sink_get_property(
  GObject *const object,
  const guint prop_id,
  GValue *const value,
  GParamSpec *const pspec
)
{
  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(object, prop_id, pspec);
  }
}

gboolean gst_tox_audio_sink_open(GstAudioSink *gst_audio_sink)
{
  return TRUE;
}

gboolean gst_tox_audio_sink_prepare(
  GstAudioSink *gst_audio_sink,
  GstAudioRingBufferSpec *gst_audio_ring_buffer_spec
)
{
  return TRUE;
}

gboolean gst_tox_audio_sink_unprepare(GstAudioSink *gst_audio_sink)
{
  return TRUE;
}

gboolean gst_tox_audio_sink_close(GstAudioSink *gst_audio_sink)
{
  return TRUE;
}

gint gst_tox_audio_sink_write(
  GstAudioSink *gst_audio_sink,
  gpointer data,
  guint length
)
{
  return length;
}

void gst_tox_audio_sink_reset(GstAudioSink *gst_audio_sink)
{
}

guint gst_tox_audio_sink_delay(GstAudioSink *gst_audio_sink)
{
  return 0;
}
