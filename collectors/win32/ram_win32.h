#ifndef _WT_COLLECTOR_RAM_H
#define _WT_COLLECTOR_RAM_H

#include "watchtower_collector.h"
#include "watchtower_common.h"
#include <windows.h>

struct ram_collector
{
	struct collector base;

	DWORD memory_load;

	unsigned long long memory_total_phy;
	unsigned long long memory_available_phy;

	unsigned long long memory_total_page_file;
	unsigned long long memory_available_page_file;

	unsigned long long memory_total_virtual;
	unsigned long long memory_available_virtual;

	unsigned long long memory_available_extended_virtual;
};

static inline struct ram_collector *
ram_collector_from_base(struct collector *collector)
{
	return container_of(collector, struct ram_collector, base);
}

int ram_collector_init(struct ram_collector *self, uint32_t interval_ms);
int ram_collector_destroy(struct ram_collector *self);

#endif