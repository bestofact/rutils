#include "rutils/format.h"

#include "test_support.h"

#include <string_view>

namespace
{
	enum class status
	{
		ready,
		busy,
	};

	constexpr std::string_view empty = rutils::format("");
	constexpr std::string_view literal = rutils::format("no replacements");
	constexpr std::string_view at_edges = rutils::format("{} middle {}", "left", "right");
	constexpr std::string_view adjacent = rutils::format("{}{}{}", 1, 2, 3);
	constexpr std::string_view supported_types = rutils::format(
		"{} {} {} {}",
		status::ready,
		^^bool,
		-42,
		std::string_view{
			"text"
		}
	);
	constexpr std::string_view unmatched_braces = rutils::format("{left} and {right");
	constexpr std::string_view escaped_braces = rutils::format("{{}}", "value");

	static_assert(empty.empty());
	static_assert(literal == "no replacements");
	static_assert(at_edges == "left middle right");
	static_assert(adjacent == "123");
	static_assert(supported_types == "ready bool -42 text");
	static_assert(unmatched_braces == "{left} and {right");
	static_assert(escaped_braces == "{value}");
} // namespace

void test_format(test_context& context)
{
	context.expect_equal(empty, "", "formats an empty string");
	context.expect_equal(literal, "no replacements", "keeps text without placeholders");
	context.expect_equal(at_edges, "left middle right", "replaces placeholders at both edges");
	context.expect_equal(adjacent, "123", "replaces adjacent placeholders");
	context.expect_equal(
		supported_types,
		"ready bool -42 text",
		"formats enums, reflections, constants, and string views"
	);
	context.expect_equal(unmatched_braces, "{left} and {right", "keeps unmatched braces");
	context.expect_equal(escaped_braces, "{value}", "keeps doubled outer braces");
}
