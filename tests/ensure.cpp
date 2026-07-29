#include "rutils/ensure.h"

#include "test_support.h"

namespace
{
	consteval bool successful_ensures_compile()
	{
		rutils::ensure(true);
		rutils::ensure(true, "unused message");
		rutils::ensure(true, "{} is {}", ^^int, "an integer");
		return true;
	}

	static_assert(successful_ensures_compile());
} // namespace

void test_ensure(test_context& context)
{
	context.check(successful_ensures_compile(), "ensure accepts true conditions");
}
