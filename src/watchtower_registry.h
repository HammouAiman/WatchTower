#ifndef _WATCHTOWER_REGISTRY_H
#define _WATCHTOWER_REGISTRY_H

//author: aiman hammou
//this file handles the registration of the collectors.

#include "watchtower_collector.h"

struct collector_registry
{
	struct collector *head;
	size_t count;
};

int collector_registry_init(struct collector_registry *self);
int collector_registry_destroy(struct collector_registry *self);

//collector management
int collector_registry_add(struct collector_registry *self, struct collector *cl);
int collector_registry_remove(struct collector_registry *self, struct collector *cl);
struct collector *collector_registry_find_by_name(struct collector_registry *self, const char *name);

//batch operations
int collector_registry_collect_all(struct collector_registry *self);
int collector_registry_shutdown_all(struct collector_registry *self);

//visitor
typedef int (*register_visitor_fn)(struct collector *cl, void *context);
int collector_registry_foreach(struct collector_registry *self, register_visitor_fn fn, void *context);

#endif