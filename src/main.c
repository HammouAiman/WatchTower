#include "..\collectors\win32\ram_win32.h"
#include "wt_collector.h"
#include "wt_common.h"

const char *symtable_unit[] = {
	[UNIT_PERCENT] = "%",
};

int main(int argc, char **argv)
{
	int ret = METRIC_OK;

	struct ram_collector ram;
	struct metric_value value;

	//init
	ret = ram_collector_init(&ram, 1000);
	if (ret != METRIC_OK)
	{
		LOG_ERROR("failed to init RAM controller!\n");
		return EXIT_FAILURE;
	}

	//collect
	for (int i = 0; i < 10; i++)
	{
		ret = collector_collect(&ram.base, &value);
		if (ret == METRIC_OK)
		{
			printf(
				"Memory usage: %.2f%s (total: %.2f GB, available: %.2f GB)\n",
				value.value, symtable_unit[value.unit],
				ram.memory_total_phy / (1024.0 * 1024.0 * 1024.0),
				ram.memory_available_phy / (1024.0 * 1024.0 * 1024.0));
		}

		Sleep(1000);
	}

	//cleanup
	ram_collector_destroy(&ram);

	return EXIT_SUCCESS;
}