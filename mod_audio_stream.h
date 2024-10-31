#ifndef MOD_AUDIO_STREAM_H
#define MOD_AUDIO_STREAM_H

#include <switch.h>
#include <switch_types.h>
#include <speex/speex_resampler.h>
#include "buffer/ringbuffer.h"

#define MY_BUG_NAME "audio_stream"
#define MAX_SESSION_ID (256)
#define MAX_WS_URI (4096)
#define MAX_METADATA_LEN (8192)

#define EVENT_CONNECT "mod_audio_stream::connect"
#define EVENT_DISCONNECT "mod_audio_stream::disconnect"
#define EVENT_ERROR "mod_audio_stream::error"
#define EVENT_JSON "mod_audio_stream::json"
#define EVENT_PLAY "mod_audio_stream::play"

typedef void (*responseHandler_t)(switch_core_session_t *session, const char *eventName, const char *json);

typedef struct
{
    switch_core_session_t *session;
    switch_codec_implementation_t *read_impl;
    switch_media_bug_t *read_bug;
    switch_audio_resampler_t *read_resampler;

    int talking;
    int talked;
    int talk_hits;
    int listen_hits;
    int hangover;
    int hangover_len;
    int divisor;
    int thresh;
    int channels;
    int sample_rate;
    int debug;
    int _hangover_len;
    int _thresh;
    int _listen_hits;
    switch_vad_state_t vad_state;
    switch_vad_t *svad;
} wavin_vad_t;

struct private_data
{
    switch_mutex_t *mutex;
    // switch_buffer_t *buffer;
    char sessionId[MAX_SESSION_ID];
    SpeexResamplerState *resampler;
    responseHandler_t responseHandler;
    void *pAudioStreamer;
    char ws_uri[MAX_WS_URI];
    int sampling;
    int channels;
    int audio_paused : 1;
    char initialMetadata[8192];
    RingBuffer *buffer;
    uint8_t *data;
    int rtp_packets;
};

typedef struct private_data private_t;

enum notifyEvent_t
{
    CONNECT_SUCCESS,
    CONNECT_ERROR,
    CONNECTION_DROPPED,
    MESSAGE
};

#endif // MOD_AUDIO_STREAM_H
