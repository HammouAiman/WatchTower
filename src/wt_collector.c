#include "wt_collector.h"
#include "wt_common.h"

int collector_init(struct collector *c)
{
	if (c == NULL || c->ops == NULL || c->ops->init == NULL)
		return -METRIC_ERR_INVALID;
	return c->ops->init(c);
}

int collector_shutdown(struct collector *c)
{
	if (c == NULL || c->ops == NULL || c->ops->shutdown == NULL)
		return -METRIC_ERR_INVALID;
	return c->ops->shutdown(c);
}

int collector_collect(struct collector *c, struct metric_value *out)
{
	if (c == NULL || c->ops == NULL || c->ops->shutdown == NULL ||
	    out == NULL)
		return -METRIC_ERR_INVALID;
	return c->ops->collect(c, out);
}