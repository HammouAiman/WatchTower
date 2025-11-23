#ifndef _WT_COLLECTOR_H
#define _WT_COLLECTOR_H

#include "wt_common.h"

struct collector;
struct collector_ops {
	int (*init)(struct collector *self);
	int (*shutdown)(struct collector *self);
	int (*collect)(struct collector *self, struct metric_value *out);
};

struct collector {
	const struct collector_ops *ops;
	uint32_t acquisition_interval_ms;
	bool enabled;
	void *interval_data;
	struct collector *next_collector;
};

//helpers function (wrappers)
int collector_init(struct collector *c);
int collector_shutdown(struct collector *c);
int collector_collect(struct collector *c, struct metric_value *out);

#endif