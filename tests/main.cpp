#include "test_support.h"

#include <cstdlib>

void test_ensure(test_context& context);
void test_enum_to_string(test_context& context);
void test_fixed_string(test_context& context);
void test_format(test_context& context);

int main()
{
	test_context context;

	test_ensure(context);
	test_enum_to_string(context);
	test_fixed_string(context);
	test_format(context);

	return context.finish() ? EXIT_SUCCESS : EXIT_FAILURE;
}
