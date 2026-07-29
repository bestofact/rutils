#pragma once

#include <algorithm>
#include <cstddef>
#include <string_view>

namespace rutils
{
	// can be used as NTTP
	template<std::size_t Size>
	struct fixed_string
	{
		char m_data[Size]{};

		constexpr fixed_string(const char (&str)[Size])
		{
			std::copy_n(str, Size, m_data);
		}

		constexpr std::string_view view() const
		{
			return {m_data, Size - 1}; // drop the '\0'
		}

		constexpr bool operator==(const fixed_string&) const = default;
	};
} // namespace rast::detail
