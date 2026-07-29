#pragma once

#include <string_view>
#include <meta>

namespace rutils
{
	template<typename Type>
	constexpr std::string_view enum_to_string(const Type in_value)
	{
		constexpr auto k_enumerators = std::define_static_array(std::meta::enumerators_of(^^Type));
		template for (constexpr std::meta::info k_entry : k_enumerators)
		{
			if ([:k_entry:] == in_value)
			{
				return std::meta::identifier_of(k_entry);
			}
		}

		constexpr std::string_view k_unsupported_enum_entry = "<unsupported-enum-entry>";
		return k_unsupported_enum_entry;
	}
}
