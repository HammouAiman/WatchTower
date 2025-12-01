#include "watchtower_common.h"

const char *format_timestamp(time_t timestamp, char *buffer, size_t buffer_size)
{
	struct tm *tm_info;

	if (!buffer || buffer_size == 0)
	{
		return "invalid buffer";
	}

	tm_info = localtime(&timestamp);
	if (!tm_info)
	{
		/* Fallback to raw timestamp on error */
		snprintf(buffer, buffer_size, "%ld", (long)timestamp);
		return buffer;
	}

	if (strftime(buffer, buffer_size, "%Y-%m-%d %H:%M:%S", tm_info) == 0)
	{
		/* Fallback to raw timestamp if format fails */
		snprintf(buffer, buffer_size, "%ld", (long)timestamp);
	}

	return buffer;
}