#pragma once

#include "rutils/format.h"

#include <meta>
#include <string_view>

namespace rutils::details
{
	template<auto& Message>
	constexpr void ensure_impl()
	{
		static_assert(false, std::string_view(Message));
	}

	consteval void invoke_ensure(const std::string_view in_message)
	{
		constexpr std::meta::info k_impl = ^^rutils::details::ensure_impl;
		const std::meta::info message = std::meta::reflect_constant_string(in_message);
		const std::meta::info impl = std::meta::substitute(k_impl, {message});
		std::meta::extract<void (*)()>(impl)();
	}
} // namespace rutils::details

namespace rutils
{
	consteval void ensure(const bool in_condition, const std::string_view in_message = "<compile-error>")
	{
		if (!in_condition)
		{
			rutils::details::invoke_ensure(in_message);
		}
	}

	template<typename... Args>
	consteval void ensure(const bool in_condition, const std::string_view in_message, const Args&... in_args)
	{
		if (!in_condition)
		{
			rutils::details::invoke_ensure(rutils::format(in_message, in_args...));
		}
	}
} // namespace rutils
