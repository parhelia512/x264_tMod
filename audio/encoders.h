#ifndef AUDIO_ENCODERS_H_
#define AUDIO_ENCODERS_H_

#include "audio/audio.h"
#include "filters/audio/audio_filters.h"

typedef struct audio_encoder_t
{
    hnd_t (*init)( hnd_t filter_chain, const char *opts );
    audio_info_t *(*get_info)( hnd_t handle );
    audio_packet_t *(*get_next_packet)( hnd_t handle );
    char *(*get_codec_name)( hnd_t handle ); // some encoders might output more than one possible codec
    void (*skip_samples)( hnd_t handle, uint64_t samplecount );
    audio_packet_t *(*finish)( hnd_t handle );
    void (*free_packet)( hnd_t handle, audio_packet_t *samples );
    void (*close)( hnd_t handle );
} audio_encoder_t;

extern const audio_encoder_t audio_encoder_raw;
#if HAVE_LAME
extern const audio_encoder_t audio_encoder_mp3;
#endif

/* the first available encoder on allowed_list is the prefered encoder if encoder is "auto"
 * allowed_list = NULL means any valid encoder is allowed
 * The 'none' case isn't handled by this function (will return NULL like with any other invalid encoder)
 * If the user wants 'none' to be a default, it must be tested outside of this function
 * If the user wants to allow any encoder, the default case must be tested outside of this function */
const audio_encoder_t *x264_select_audio_encoder( char *encoder, char* allowed_list[] );
const audio_encoder_t *x264_encoder_by_name( char *name );
hnd_t x264_audio_encoder_open( const audio_encoder_t *encoder, hnd_t filter_chain, const char *opts );

char *x264_audio_encoder_codec_name( hnd_t encoder );
audio_info_t *x264_audio_encoder_info( hnd_t encoder );
void x264_audio_encoder_skip_samples( hnd_t encoder, uint64_t samplecount );
audio_packet_t *x264_audio_encode_frame( hnd_t encoder );
audio_packet_t *x264_audio_encoder_finish( hnd_t encoder );
void x264_audio_free_frame( hnd_t encoder, audio_packet_t *frame );

void x264_audio_encoder_close( hnd_t encoder );

#endif
