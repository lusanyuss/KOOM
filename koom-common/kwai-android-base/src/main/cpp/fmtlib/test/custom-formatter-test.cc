// Formatting library for C++ - custom argument formatter tests
//
// Copyright (c) 2012 - present, Victor Zverovich
// All rights reserved.
//
// For the license information refer to format.h.

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "fmt/format.h"
#include "gtest-extra.h"

// MSVC 2013 is known to be broken.
#if !FMT_MSC_VER || FMT_MSC_VER > 1800

// A custom argument formatter that doesn't print `-` for floating-point values
// rounded to 0.
class custom_arg_formatter
	: public fmt::detail::arg_formatter<fmt::format_context::iterator, char> {
 public:
  using base = fmt::detail::arg_formatter<fmt::format_context::iterator, char>;

  custom_arg_formatter(fmt::format_context &ctx,
					   fmt::format_parse_context *parse_ctx,
					   fmt::format_specs *s = nullptr,
					   const char * = nullptr)
	  : base(ctx, parse_ctx, s) {}

  using base::operator();

  iterator operator()(double value) {
	// Comparing a float to 0.0 is safe.
	if (round(value * pow(10, specs()->precision)) == 0.0) value = 0;
	return base::operator()(value);
  }
};

std::string custom_vformat(fmt::string_view format_str, fmt::format_args args) {
  fmt::memory_buffer buffer;
  fmt::detail::buffer<char> &base = buffer;
  // Pass custom argument formatter as a template arg to vwrite.
  fmt::vformat_to<custom_arg_formatter>(std::back_inserter(base), format_str,
										args);
  return std::string(buffer.data(), buffer.size());
}

template<typename... Args>
std::string custom_format(const char *format_str, const Args &... args) {
  auto va = fmt::make_format_args(args...);
  return custom_vformat(format_str, va);
}

TEST(CustomFormatterTest, Format
) {
EXPECT_EQ("0.00", custom_format("{:.2f}", -.00001));
}
#endif
