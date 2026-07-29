#pragma once

#include "rutils/enum_to_string.h"

#include <array>
#include <meta>
#include <string_view>

namespace rutils::details
{
	template<typename Type>
	constexpr std::string_view to_string_view(const Type in_arg)
	{
		if constexpr (std::is_enum_v<Type>)
		{
			return rutils::enum_to_string(in_arg);
		}

		else if constexpr (std::is_same_v<Type, std::meta::info>)
		{
			return std::meta::display_string_of(in_arg);
		}

		else if constexpr (std::is_convertible_v<Type, std::string_view>)
		{
			return in_arg;
		}
		else if consteval
		{
			return rutils::details::to_string_view(std::meta::reflect_constant(in_arg));
		}
		else
		{
			return {};
		}
	}

	template<typename... Args>
	constexpr void format_to(std::string& out_string, const std::string_view in_format, const Args&... in_args)
	{
		const std::size_t format_size = in_format.size();
		const std::size_t arg_count = sizeof...(Args);
		const std::array<std::string_view, arg_count> arg_texts = {rutils::details::to_string_view<Args>(in_args)...};

		std::size_t format_cursor = 0;
		std::size_t arg_index = 0;

		for (std::size_t i = 0; i + 1 < format_size; ++i)
		{
			if (in_format[i] == '{' && in_format[i + 1] == '}')
			{
				// append the text so far and update begin
				out_string.append(in_format.substr(format_cursor, i - format_cursor));
				format_cursor = i + 2;
				++i;

				// convert arg to text
				const auto arg_text = arg_texts[arg_index];
				++arg_index;

				// append arg
				out_string.append(arg_text);
			}
		}

		if (format_cursor <= format_size)
		{
			out_string.append(in_format.substr(format_cursor, format_size - format_cursor));
		}
	}
} // namespace rutils::details

namespace rutils
{
	template<typename... Args>
	consteval auto format(const std::string_view in_format, const Args&... in_args)
	{
		std::string buffer;
		buffer.reserve(in_format.size());
		rutils::details::format_to(buffer, in_format, in_args...);
		return std::define_static_string(buffer);
	}
} // namespace rutils
