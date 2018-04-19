#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int read_number(char **cursor, double *float_, int64_t *int_);

int main(int argc, char *argv[])
{
	if (argc != 2) {
		return EXIT_FAILURE;
	}
	double float_ = 0.0;
	int64_t int_ = 0LL;
	int r = read_number(&argv[1], &float_, &int_);
	printf("r      = %d\nfloat_ = %f\nint_   = %lld\n", r, float_, int_);
	return EXIT_SUCCESS;
}

