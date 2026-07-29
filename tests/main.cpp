#include "rutils/format.h"
#include "rutils/enum_to_string.h"
#include "rutils/ensure.h"
#include <print>
#include <iostream>

template<typename T>
struct Test
{
	consteval
	{
		rutils::ensure(std::meta::is_same_type(^^T, ^^int), "T ({}) is not int.", ^^T);
	}
};


consteval 
{
}

enum class m
{
	a
};

int main()
{
	constexpr auto s = rutils::format("Hello {} asd {} 232 {} asas", 12, m::a, ^^bool);
	
	Test<bool> a;

	std::println("{}", s);
	std::println("Enum = {}", rutils::enum_to_string(m::a));
	return 0;
}
