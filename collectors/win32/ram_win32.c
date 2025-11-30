#include "ram_win32.h"

#include "wt_collector.h"
#include "wt_common.h"

static int get_memory_status(struct ram_collector *self)
{
	MEMORYSTATUSEX memory_status;

	if (self == NULL)
	{
		LOG_ERROR("ram collector is null!\n\r");
		return -METRIC_ERR_INVALID;
	}

	memory_status.dwLength = sizeof(memory_status);
	if (!GlobalMemoryStatusEx(&memory_status))
	{
		LOG_ERROR("GlobalMemoryStatusEx failed with error %lu\n",
				  (long unsigned int)GetLastError());
		return -METRIC_ERR_COLLECT;
	}

	self->memory_load				 = memory_status.dwMemoryLoad;
	self->memory_total_phy			 = memory_status.ullTotalPhys;
	self->memory_available_phy		 = memory_status.ullAvailPhys;
	self->memory_total_virtual		 = memory_status.ullTotalVirtual;
	self->memory_available_virtual	 = memory_status.ullAvailVirtual;
	self->memory_total_page_file	 = memory_status.ullTotalPageFile;
	self->memory_available_page_file = memory_status.ullAvailPageFile;

	self->memory_available_extended_virtual =
		memory_status.ullAvailExtendedVirtual;

	return METRIC_OK;
}

static int ram_ops_init(struct collector *cl)
{
	int ret = METRIC_OK;

	if (cl == NULL)
	{
		LOG_ERROR("collector is null!\n");
		return -METRIC_ERR_INVALID;
	}

	struct ram_collector *self = ram_collector_from_base(cl);

	ret = get_memory_status(self);
	if (ret != METRIC_OK)
	{
		LOG_ERROR("failed to query memory status!");
		return ret;
	}

	return METRIC_OK;
}

static int ram_ops_shutdown(struct collector *cl)
{
	//nothing to cleanup
	return METRIC_OK;
}

static int ram_ops_collect(struct collector *cl, struct metric_value *out)
{
	int ret = METRIC_OK;

	struct ram_collector *self = NULL;

	if (cl == NULL)
	{
		LOG_ERROR("collector is null!\n");
		return -METRIC_ERR_INVALID;
	}
	else if (out == NULL)
	{
		LOG_ERROR("metric output is null!\n");
		return -METRIC_ERR_INVALID;
	}

	self = ram_collector_from_base(cl);

	ret = get_memory_status(self);
	if (ret != METRIC_OK)
	{
		return ret;
	}

	out->value				   = self->memory_load;
	out->unit				   = UNIT_PERCENT;
	out->acquisition_timestamp = time(NULL);

	return METRIC_OK;
}

static const struct collector_ops ram_collector_ops = {
	.init	  = ram_ops_init,
	.collect  = ram_ops_collect,
	.shutdown = ram_ops_shutdown,
};

//public functions
int ram_collector_init(struct ram_collector *self, uint32_t interval_ms)
{
	if (self == NULL)
	{
		LOG_ERROR("ram collector is null!\n");
		return -METRIC_ERR_INVALID;
	}

	collector_init_base(&self->base, (struct collector_ops *)&ram_collector_ops,
						true, NULL, NULL, interval_ms);

	self->memory_load				 = 0;
	self->memory_total_phy			 = 0;
	self->memory_available_phy		 = 0;
	self->memory_total_virtual		 = 0;
	self->memory_available_virtual	 = 0;
	self->memory_total_page_file	 = 0;
	self->memory_available_page_file = 0;

	self->memory_available_extended_virtual = 0;

	return collector_init(&self->base);
}

int ram_collector_destroy(struct ram_collector *self)
{
	if (self == NULL)
	{
		LOG_ERROR("ram collector is null!\n");
		return -METRIC_ERR_INVALID;
	}

	return collector_shutdown(&self->base);
}
