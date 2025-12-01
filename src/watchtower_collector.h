#ifndef _WT_COLLECTOR_H
#define _WT_COLLECTOR_H

#include "watchtower_common.h"

struct collector;
struct collector_ops
{
	const char *name;
	int unit;

	int (*init)(struct collector *self);
	int (*shutdown)(struct collector *self);
	int (*collect)(struct collector *self, struct metric_value *out);

	void (*print)(struct collector *self, const struct metric_value *val);
};

struct collector
{
	bool enabled;
	const struct collector_ops *ops;
	uint32_t acquisition_interval_ms;
	void *interval_data;
	struct collector *next_collector;
};

//helpers function (wrappers)
int collector_init(struct collector *c);
int collector_shutdown(struct collector *c);
int collector_collect(struct collector *c, struct metric_value *out);
void collector_print(struct collector *c, struct metric_value *out);

void collector_init_base(struct collector *base, struct collector_ops *, bool, void *, struct collector *, uint32_t);
#endif