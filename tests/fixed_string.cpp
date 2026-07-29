#include "rutils/fixed_string.h"

#include "test_support.h"

#include <string_view>

namespace
{
	template<rutils::fixed_string Value>
	struct string_constant
	{
		static constexpr auto value = Value;
	};

	constexpr rutils::fixed_string empty{""};
	constexpr rutils::fixed_string greeting{"hello"};
	constexpr rutils::fixed_string greeting_copy{"hello"};
	constexpr rutils::fixed_string other{"world"};
	constexpr rutils::fixed_string embedded_null{"a\0b"};

	static_assert(empty.view().empty());
	static_assert(greeting.view() == "hello");
	static_assert(greeting == greeting_copy);
	static_assert(greeting != other);
	static_assert(embedded_null.view() == std::string_view{"a\0b", 3});
	static_assert(string_constant<"non-type template parameter">::value.view() == "non-type template parameter");
} // namespace

void test_fixed_string(test_context& context)
{
	context.expect_equal(empty.view(), "", "views an empty string");
	context.expect_equal(greeting.view(), "hello", "drops the terminating null from its view");
	context.check(greeting == greeting_copy, "equal values compare equal");
	context.check(greeting != other, "different values compare unequal");
	context.expect_equal(embedded_null.view(), std::string_view{"a\0b", 3}, "preserves embedded null characters");
}
