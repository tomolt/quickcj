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
	char *cursor = argv[1];
	int r = read_number(&cursor, &float_, &int_);
	printf("r      = %d\nfloat_ = %f\nint_   = %ld\n", r, float_, int_);
	double atof_ = atof(argv[1]);
	int64_t atol_ = atol(argv[1]);
	printf("atof() = %f\natol() = %ld\n", atof_, atol_);
	return EXIT_SUCCESS;
}

