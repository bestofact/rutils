#include "rutils/enum_to_string.h"

#include "test_support.h"

#include <cstdint>
#include <string_view>

namespace
{
	enum class color : std::int8_t
	{
		red = -2,
		green = 3,
		blue = 7,
	};

	enum direction
	{
		north,
		south,
	};

	static_assert(rutils::enum_to_string(color::red) == "red");
	static_assert(rutils::enum_to_string(color::green) == "green");
	static_assert(rutils::enum_to_string(color::blue) == "blue");
	static_assert(rutils::enum_to_string(north) == "north");
	static_assert(rutils::enum_to_string(static_cast<color>(0)) == "<unsupported-enum-entry>");
} // namespace

void test_enum_to_string(test_context& context)
{
	color runtime_value = color::blue;

	context.expect_equal(rutils::enum_to_string(runtime_value), "blue", "converts a scoped enum");
	context.expect_equal(rutils::enum_to_string(south), "south", "converts an unscoped enum");
	context.expect_equal(
		rutils::enum_to_string(static_cast<color>(42)),
		"<unsupported-enum-entry>",
		"reports a value with no enumerator"
	);
}
