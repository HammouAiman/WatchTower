#include "watchtower_registry.h"
#include "watchtower_common.h"
#include "watchtower_collector.h"
#include <string.h>

static inline const char *collector_get_name(struct collector *cl)
{
	if (cl && cl->ops && cl->ops->name)
		return cl->ops->name;

	return "unknown";
}

int collector_registry_init(struct collector_registry *self)
{
	if (self == NULL)
	{
		LOG_ERROR("collector registry is null!\n");
		return -METRIC_ERR_INVALID;
	}

	self->head	= NULL;
	self->count = 0;

	return METRIC_OK;
}

int collector_registry_destroy(struct collector_registry *self)
{
	int ret = METRIC_OK;

	if (self == NULL)
	{
		LOG_ERROR("collector registry is null!\n");
		return -METRIC_ERR_INVALID;
	}

	ret = collector_registry_shutdown_all(self);
	if (ret != METRIC_OK)
	{
		//for now ignore.
		LOG_ERROR("shutdown failed!\n");
	}

	self->head	= NULL;
	self->count = 0;

	return METRIC_OK;
}

int collector_registry_add(struct collector_registry *self, struct collector *cl)
{
	if (self == NULL)
	{
		LOG_ERROR("collector registry is null!\n");
		return -METRIC_ERR_INVALID;
	}
	else if (cl == NULL)
	{
		LOG_ERROR("collector is null!\n");
		return -METRIC_ERR_INVALID;
	}

	for (struct collector *curr = self->head; curr != NULL; curr = curr->next_collector)
	{
		//this should not happen.
		if (curr == cl)
		{
			LOG_ERROR("collector already registered.\n");
			return -METRIC_ERR_INVALID;
		}
	}

	cl->next_collector = self->head;
	self->head		   = cl;
	self->count		   = self->count + 1;

	return METRIC_OK;
}

int collector_registry_remove(struct collector_registry *self, struct collector *cl)
{
	struct collector **ptr = NULL;

	if (self == NULL)
	{
		LOG_ERROR("collector registry is null!\n");
		return -METRIC_ERR_INVALID;
	}
	else if (cl == NULL)
	{
		LOG_ERROR("collector is null!\n");
		return -METRIC_ERR_INVALID;
	}

	ptr = &self->head;
	while (*ptr)
	{
		if (*ptr == cl)
		{
			*ptr			   = cl->next_collector;
			cl->next_collector = NULL;
			self->count		   = self->count - 1;

			return METRIC_OK;
		}

		ptr = &(*ptr)->next_collector;
	}

	//not found
	return -METRIC_ERR_INVALID;
}

struct collector *collector_registry_find_by_name(struct collector_registry *self, const char *name)
{
	if (self == NULL)
	{
		LOG_ERROR("collector registry is null!\n");
		return NULL;
	}
	else if (name == NULL)
	{
		LOG_ERROR("name of collector is null!\n");
		return NULL;
	}

	for (struct collector *curr = self->head; curr != NULL; curr = curr->next_collector)
	{
		if (curr->ops && curr->ops->name && strcmp(curr->ops->name, name) == 0)
		{
			return curr;
		}
	}

	return NULL;
}

static int collector_collect_visitor(struct collector *cl, void *context)
{
	int ret;
	struct metric_value val;

	(void)context;

	if (cl->enabled == false)
	{
		return METRIC_OK;
	}

	ret = collector_collect(cl, &val);
	if (ret != METRIC_OK)
	{
		LOG_ERROR("collector %s failed to collect!\n", collector_get_name(cl));
	}

	if (cl->ops->print == NULL)
	{
		printf("[INFo] print function in %s collector is null!\n", collector_get_name(cl));
	}
	else
	{
		collector_print(cl, &val);
	}

	return METRIC_OK;
}

int collector_registry_collect_all(struct collector_registry *self)
{
	if (self == NULL)
	{
		LOG_ERROR("collector registry is null!\n");
		return -METRIC_ERR_INVALID;
	}

	return collector_registry_foreach(self, collector_collect_visitor, NULL);
}

static int collector_shutdown_visitor(struct collector *cl, void *context)
{
	int ret = METRIC_OK;

	(void)context;

	ret = collector_shutdown(cl);
	if (ret != METRIC_OK)
	{
		LOG_ERROR("collector %s failed to shutdown\n", collector_get_name(cl));
	}

	return METRIC_OK;
}

int collector_registry_shutdown_all(struct collector_registry *self)
{
	if (self == NULL)
	{
		LOG_ERROR("collector registry is null!\n");
		return -METRIC_ERR_INVALID;
	}

	return collector_registry_foreach(self, collector_shutdown_visitor, NULL);
}

int collector_registry_foreach(struct collector_registry *self, register_visitor_fn fn, void *context)
{
	int ret = METRIC_OK;

	if (self == NULL)
	{
		LOG_ERROR("collector registry is null!\n");
		return -METRIC_ERR_INVALID;
	}
	else if (fn == NULL)
	{
		LOG_ERROR("visitor function is null!\n");
		return -METRIC_ERR_INVALID;
	}

	for (struct collector *curr = self->head; curr != NULL; curr = curr->next_collector)
	{
		ret = fn(curr, context);
		if (ret != METRIC_OK)
		{
			//what to do here? should we stop?
		}
	}

	return METRIC_OK;
}