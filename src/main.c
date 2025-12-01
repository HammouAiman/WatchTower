#include "..\collectors\win32\ram_win32.h"
#include "watchtower_collector.h"
#include "watchtower_common.h"
#include "watchtower_registry.h"
#include <stdlib.h>
#include <signal.h>

static volatile sig_atomic_t keep_running = 1;

void signal_handler(int signum)
{
	(void)signum;
	keep_running = 0;
	printf("\n[MAIN] Shutdown signal received...\n");
}

int main(int argc, char **argv)
{
	int ret = METRIC_OK;

	struct collector_registry reg;
	struct ram_collector ram;

	signal(SIGINT, signal_handler);

	printf("[MAIN] Registry initialization: ");
	if (collector_registry_init(&reg) != METRIC_OK)
	{
		printf("fail! Exiting...");
		return EXIT_FAILURE;
	}
	printf("success!\n");

	printf("[MAIN] Ram collector initialization: ");
	if (ram_collector_init(&ram, 1000) != METRIC_OK)
	{
		printf("fail! Exiting...");
		return EXIT_FAILURE;
	}
	printf("success!\n");

	//add to registry
	printf("[MAIN] Registration of ram collector: ");
	if (collector_registry_add(&reg, &ram.base) != METRIC_OK)
	{
		printf("fail! Exiting...");
		collector_registry_destroy(&reg);
		return EXIT_FAILURE;
	}
	printf("success!\n");
	printf("[MAIN] Total collectors: %zu\n\n", reg.count);

	printf("[[MAIN] Starting collection!\n");
	while (keep_running)
	{
		ret = collector_registry_collect_all(&reg);
		if (ret != METRIC_OK)
		{
			fprintf(stderr, "[MAIN] Collection failed: %d\n", ret);
		}

		Sleep(1000);
	}

	printf("[MAIN] Shutting down.");

	collector_registry_destroy(&reg);

	return EXIT_SUCCESS;
}