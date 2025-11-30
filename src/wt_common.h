#ifndef _WT_COMMON_H
#define _WT_COMMON_H

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#define LOG_ERROR(fmt, ...)                                                   \
	do                                                                        \
	{                                                                         \
		time_t _now	   = time(NULL);                                          \
		struct tm *_tm = localtime(&_now);                                    \
		char _tbuf[32];                                                       \
		strftime(_tbuf, sizeof(_tbuf), "%Y-%m-%d %H:%M:%S", _tm);             \
		printf("[%s] ERROR (%s:%d:%s): " fmt "\n", _tbuf, __FILE__, __LINE__, \
			   __func__, ##__VA_ARGS__);                                      \
	} while (0)

#define container_of(ptr, type, member) \
	((type *)((char *)(ptr)-offsetof(type, member)))

#define COLLECTOR_DEFINE(name)                                  \
	struct name;                                                \
	static const collector_ops_t name##_ops;                    \
	static inline struct name *name##_from_base(collector_t *c) \
	{                                                           \
		return container_of(c, struct name, base);              \
	}

typedef enum
{
	METRIC_OK		   = 0,
	METRIC_ERR_INIT	   = -1,
	METRIC_ERR_COLLECT = -2,
	METRIC_ERR_INVALID = -3,
} metric_error_e;

enum
{
	UNIT_PERCENT = 0,
};

struct metric_value
{
	int unit; // percentage, raw, tick....
	double value;
	time_t acquisition_timestamp;
};

#endif