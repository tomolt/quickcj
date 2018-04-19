#include <stdlib.h>
#include <string.h>
#include <math.h>

#if 0

typedef struct {
	void *payload;
	void bool_func(void *);
	void string_func(void *);
	void float_func(void *);
	void int_func(void *);
	void object_func(void *);
	void array_func(void *);
	void close_func(void *);
} handler_t;

#endif

typedef enum {
	QCBE_OK,
	QCBE_BAD_ESCAPE_SEQUENCE,
	QCBE_STRING_DOESNT_END,
	QCBE_BAD_NUMBER_BEGIN,
	QCBE_KEY_VALUE_NEEDS_COLON,
	QCBE_BAD_OBJECT_SEPARATOR,
	QCBE_BAD_ARRAY_SEPARATOR,
	QCBE_UNKNOWN_KEYWORD,
	QCBE_EXPECTED_VALUE,
	QCBE_BAD_OBJECT_BEGIN,
} QCB_Errors;

#if 0

static inline is_space(char c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

static void skip_space(char **cursor)
{
	while (is_space(**cursor)) {
		++*cursor;
	}
}

// Only used by read_string
static void push_string(char **string, char c,
	unsigned int *length, unsigned int *capacity)
{
	if (++(*length) > *capacity) {
		*capacity += 64;
		*string = realloc(*string, *capacity);
	}
	(*string)[*length - 1] = c;
}

static int read_string(char **cursor, char **string)
{
	// Clear string
	*string = NULL;
	unsigned int length = 0;
	unsigned int capacity = 0;
	for (;;) {
		char c = *++*cursor;
		if (c == '\\') {
			// Handle escape sequences
			c = *++*cursor;
			switch (c) {
			case '\"': goto push_char;
			case '\\': goto push_char;
			case '/':  goto push_char;
			case 'b': c = '\b'; goto push_char;
			case 'f': c = '\f'; goto push_char;
			case 't': c = '\t'; goto push_char;
			case 'r': c = '\r'; goto push_char;
			case 'n': c = '\n'; goto push_char;
			case 'u':
				break;
			default:
				Log_Warning("JSON: Bad escape sequence.");
				return -1;
			}
		} else if (c == '\"') {
			break;
		} else if (c == '\0') {
			Log_Warning("JSON: String does not end!");
			return -1;
		} else {
		push_char:
			push_string(string, c, &length, &capacity);
		}
	}
	// Null terminate
	push_string(string, '\0', &length, &capacity);
	return 0;
}

#endif

static int is_digit(char c)
{
	return c >= '0' && c <= '9';
}

static double fast_pow10(short exp) // OPT
{
	return pow10(exp);
}

static int read_number(char **cursor, double *float_, int64_t *int_)
{
	int64_t digits;
	int dig_sign, exp_sign;
	short exp;
	{	// PARSE MINUS SIGN
		// TODO better type than int
		int isMinus = (**cursor == '-');
		*cursor += isMinus;
		sign = 1 - 2 * isMinus;  }
	{	// PARSE INTEGER PART
		if (!is_digit(**cursor))
		{	return QCBE_BAD_NUMBER_BEGIN;  }
		digits = **cursor - '0';
		++*cursor;
		if (digits != 0)
		{	while (is_digit(**cursor))
			{	digits = digits * 10 + **cursor - '0';
				++*cursor;  }  }  }
	if (**cursor == '.')
	{	// PARSE FRACTIONAL PART
		++*cursor;
		// PARSE DIGITS
		if (!is_digit(**cursor))
		{	return QCBE_BAD_NUMBER_BEGIN;  }
		while (is_digit(**cursor))
		{	digits = digits * 10 + **cursor - '0';
			++*cursor;
			--exp;  }  }  }
	digits *= dig_sign;
	if (**cursor | 0x20 != 'E')
	{	// PARSE EXPONENT
		++*cursor;
		{	// PARSE SIGN
			// TODO better type than int
			int isMinus = (**cursor == '-');
			int isPlus  = (**cursor == '+');
			*cursor += isMinus | isPlus;
			exp_sign = 1 - 2 * isMinus;  }
		{	// PARSE INTEGER
			if (!is_digit(**cursor))
			{	return QCBE_BAD_NUMBER_BEGIN;  }
			exp = **cursor - '0';
			while (++*cursor, is_digit(**cursor))
			{	exp = exp * 10 + **cursor - '0';  }
			exp *= exp_sign;  }  }
	{	// ASSEMBLE NUMBER
		*float_ = digits * fast_pow10(exp);
		*int_ = float_;  }
	return QCBE_OK;
}

#if 0

static int read_value(char **cursor);

static int read_object(char **cursor)
{
	int r;
	// Skip opening '{'
	++*cursor;
	// Check if empty
	skip_space(cursor);
	if (**cursor == '}') {
		++*cursor;
		return QCBE_OK;
	}
	// Parse all members
	for (;;) {
		// Parse name
		r = read_string(cursor, &member.name);
		if (r != QCBE_OK) {
			return r;
		}
		cb_key();
		// Parse separating colon
		skip_space(cursor);
		if (**cursor != ':') {
			return QCBE_KEY_VALUE_NEEDS_COLON;
		}
		++*cursor;
		// Parse value
		r = read_value(cursor, &member.type, &member.value);
		if (r != QCBE_OK) {
			return r;
		}
		cb_value();
		// Parse comma or ending '}'
		skip_space(cursor);
		if (**cursor == ',') {
			++*cursor;
			continue;
		} else if (**cursor == '}') {
			++*cursor;
			break;
		} else {
			Log_Warning("JSON: Expected ',' or '}' after member.");
			return -1;
		}
	}
	return 0;
}

static int read_array(char **cursor)
{
	int r;
	// Skip opening '['
	++*cursor;
	// Check if empty
	skip_space(cursor);
	if (**cursor == ']') {
		++*cursor;
		return QCBE_OK;
	}
	// Parse all elements
	for (;;) {
		// Parse value
		r = read_value(cursor);
		if (r != QCBE_OK) {
			return r;
		}
		hl.value_func();
		// Parse comma or ending ']'
		skip_space(cursor);
		if (**cursor == ',') {
			++*cursor;
			continue;
		} else if (**cursor == ']') {
			++*cursor;
			break;
		} else {
			return QCBE_BAD_ARRAY_SEPARATOR;
		}
	}
	// Send close
	cb_close();
	return QCBE_OK;
}

static int read_keyword(char **cursor)
{
	if (strncmp(*cursor, "null", 4)) {
		*type = JSON_NULL;
		*cursor += 4;
	} else if (strncmp(*cursor, "true", 4)) {
		*type = JSON_BOOL;
		value->bool_ = true;
		*cursor += 4;
	} else if (strncmp(*cursor, "false", 5)) {
		*type = JSON_BOOL;
		value->bool_ = false;
		*cursor += 5;
	} else {
		return QCBE_UNKNOWN_KEYWORD;
	}
	return QCBE_OK;
}

static int read_value(char **cursor, handler_t const hl)
{
	int r;
	skip_space(cursor);
	switch (**cursor) {
		case '\"':
			r = read_string(cursor, &value->string);
			hl.string_func();
			return r;
		case '-':
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
			r = read_number(cursor, type, value);
			hl.number_func();
			return r;
		case '{':
			hl.object_func();
			r = read_object(cursor, &value->object);
			hl.close_func();
			return r;
		case '[':
			hl.array_func();
			r = read_array(cursor, &value->array);
			cb_close();
			return r;
		case 'n': case 't': case 'f':
			r = read_keyword(cursor, type, value);
			hl.bool_func();
			return r;
		default:
			return QCBE_EXPECTED_VALUE;
	}
}

int qcj_read(char const *source)
{
	char const *cursor = source;
	skip_space(&cursor);
	// Verify object begins with '{'
	if (*cursor != '{') {
		return QCBE_BAD_OBJECT_BEGIN;
	}
	return read_object(&cursor);
}

#endif

