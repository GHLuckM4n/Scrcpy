#ifndef SC_DEMUXER_H
#define SC_DEMUXER_H

#include "common.h"

#include <stdbool.h>
#include <stdint.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>

#include "trait/packet_sink.h"
#include "util/net.h"
#include "util/thread.h"

#define SC_DEMUXER_MAX_SINKS 2

struct sc_demuxer {
    const char *name; // must be statically allocated (e.g. a string literal)

    sc_socket socket;
    sc_thread thread;

    struct sc_packet_sink *sinks[SC_DEMUXER_MAX_SINKS];
    unsigned sink_count;

    const struct sc_demuxer_callbacks *cbs;
    void *cbs_userdata;
};

enum sc_demuxer_status {
    SC_DEMUXER_STATUS_EOS,
    SC_DEMUXER_STATUS_DISABLED,
    SC_DEMUXER_STATUS_ERROR,
};

struct sc_demuxer_callbacks {
    void (*on_ended)(struct sc_demuxer *demuxer, enum sc_demuxer_status,
                     void *userdata);
};

// The name must be statically allocated (e.g. a string literal)
void
sc_demuxer_init(struct sc_demuxer *demuxer, const char *name, sc_socket socket,
                const struct sc_demuxer_callbacks *cbs, void *cbs_userdata);

void
sc_demuxer_add_sink(struct sc_demuxer *demuxer, struct sc_packet_sink *sink);

bool
sc_demuxer_start(struct sc_demuxer *demuxer);

void
sc_demuxer_join(struct sc_demuxer *demuxer);

#endif
