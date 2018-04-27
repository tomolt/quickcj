
typedef struct {
	void *userdata;
	void (*key_func)(void *, char const *, size_t);
	void (*bool_func)(void *, int);
	void (*string_func)(void *, char const *, size_t);
	void (*float_func)(void *, double);
	void (*int_func)(void *, int64_t);
	void (*object_func)(void *);
	void (*array_func)(void *);
	void (*close_func)(void *);
} qcj_handler_t;

typedef enum {
	QCJE_OK,
	QCJE_BAD_ESCAPE_SEQUENCE,
	QCJE_STRING_DOESNT_END,
	QCJE_BAD_UNICODE,
	QCJE_BAD_NUMBER_BEGIN,
	QCJE_KEY_VALUE_NEEDS_COLON,
	QCJE_BAD_OBJECT_SEPARATOR,
	QCJE_BAD_ARRAY_SEPARATOR,
	QCJE_UNKNOWN_KEYWORD,
	QCJE_EXPECTED_VALUE,
	QCJE_BAD_OBJECT_BEGIN,
} QCJ_Errors;

