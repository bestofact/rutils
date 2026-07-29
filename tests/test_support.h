#pragma once

#include <cstddef>
#include <iostream>
#include <string_view>

class test_context
{
public:
	void check(const bool condition, const std::string_view description)
	{
		++m_check_count;
		if (!condition)
		{
			++m_failure_count;
			std::cerr << "FAIL: " << description << '\n';
		}
	}

	void expect_equal(
		const std::string_view actual,
		const std::string_view expected,
		const std::string_view description
	)
	{
		++m_check_count;
		if (actual != expected)
		{
			++m_failure_count;
			std::cerr << "FAIL: " << description << "\n"
					  << "  expected: \"" << expected << "\"\n"
					  << "    actual: \"" << actual << "\"\n";
		}
	}

	[[nodiscard]]
	bool finish() const
	{
		if (m_failure_count != 0)
		{
			std::cerr << m_failure_count << " of " << m_check_count << " checks failed\n";
			return false;
		}

		std::cout << "All " << m_check_count << " checks passed\n";
		return true;
	}

private:
	std::size_t m_check_count{};
	std::size_t m_failure_count{};
};
