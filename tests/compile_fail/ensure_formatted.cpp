#include "rutils/ensure.h"

consteval
{
	rutils::ensure(false, "{} is not {}", ^^bool, ^^int);
}

int main()
{
}
