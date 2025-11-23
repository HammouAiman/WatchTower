#ifndef _WT_COMMON_H
#define _WT_COMMON_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr)-offsetof(type, member)))

#define COLLECTOR_DEFINE(name)                                      \
	struct name;                                                \
	static const collector_ops_t name##_ops;                    \
	static inline struct name *name##_from_base(collector_t *c) \
	{                                                           \
		return container_of(c, struct name, base);          \
	}

typedef enum {
	METRIC_OK = 0,
	METRIC_ERR_INIT = -1,
	METRIC_ERR_COLLECT = -2,
	METRIC_ERR_INVALID = -3,
} metric_error_e;

struct metric_value {
	double value;
	time_t acquisition_timestamp;
	const char *unit; // percentage, raw, tick....
};

#endif