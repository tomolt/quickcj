#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "quickcj.h"

int read_number(char **cursor, double *float_, int64_t *int_);
int read_string(char **cursor, qcb_handler_t *handler);
int read_keyword(char **cursor, qcb_handler_t *handler);

#if 0
static void int_number(char const *arg)
{
	double float_ = 0.0;
	int64_t int_ = 0LL;
	char *cursor = arg;
	int r = read_number(&cursor, &float_, &int_);
	printf("r      = %d\nfloat_ = %f\nint_   = %ld\n", r, float_, int_);
	double atof_ = atof(arg);
	int64_t atol_ = atol(arg);
	printf("atof() = %f\natol() = %ld\n", atof_, atol_);
}
#endif

static void string_handler(void *userdata, char const *string, size_t length)
{
	printf("string = \"");
	for (size_t i = 0; i < length; ++i) {
		char c = string[i];
		if (c < 0x20) {
			c = '?';
		}
		putchar(c);
	}
	printf("\" (length: %lu)\n", length);
}

static void int_string(char const *arg)
{
	qcb_handler_t handler = {.string_func = string_handler};
	size_t length = strlen(arg) + 1;
	char *data = malloc(length), *cursor = data;
	memcpy(data, arg, length);
	int r = read_string(&cursor, &handler);
	printf("r      = %d\n", r);
	free(data);
}

static void bool_handler(void *userdata, int value)
{
	printf("bool   = %s\n", value ? "true" : "false");
}

static void int_keyword(char const *arg)
{
	qcb_handler_t handler = {.bool_func = bool_handler};
	size_t length = strlen(arg) + 1;
	char *data = malloc(length), *cursor = data;
	memcpy(data, arg, length);
	int r = read_keyword(&cursor, &handler);
	printf("r      = %d\n", r);
	free(data);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		return EXIT_FAILURE;
	}
	// int_number(argv[1]);
	// int_string(argv[1]);
	int_keyword(argv[1]);
	return EXIT_SUCCESS;
}

