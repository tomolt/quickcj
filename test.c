#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "quickcj.h"

int qcj_read(char *, qcb_handler_t *);

static void print_json_string(char const *string, size_t length)
{
	putchar('\"');
	for (size_t i = 0; i < length; ++i) {
		char c = string[i];
		if (c < 0x20) {
			c = '?';
		}
		putchar(c);
	}
	printf("\" (length: %lu)", length);
}

static void key_handler(void *userdata, char const *string, size_t length)
{
	printf("key    = ");
	print_json_string(string, length);
	printf("\n");
}

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

static void int_handler(void *userdata, int64_t value)
{
	printf("int    = %ld\n", value);
}

static void float_handler(void *userdata, double value)
{
	printf("float  = %f\n", value);
}

static void bool_handler(void *userdata, int value)
{
	printf("bool   = %s\n", value ? "true" : "false");
}

static void array_handler(void *userdata)
{
	printf("array  = {\n");
}

static void object_handler(void *userdata)
{
	printf("object = {\n");
}

static void close_handler(void *userdata)
{
	printf("}\n");
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		return EXIT_FAILURE;
	}
	size_t copyLength = strlen(argv[1]) + 1;
	char *copy = malloc(copyLength), *cursor = copy;
	memcpy(copy, argv[1], copyLength);
	qcb_handler_t handler =
	{	.key_func    = key_handler,
		.string_func = string_handler,
		.int_func    = int_handler,
		.float_func  = float_handler,
		.bool_func   = bool_handler,
		.array_func  = array_handler,
		.object_func = object_handler,
		.close_func  = close_handler  };
	int r = qcj_read(cursor, &handler);
	printf("r      = %d\n", r);
	free(copy);
	return EXIT_SUCCESS;
}

