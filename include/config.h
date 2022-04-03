//
// Created by cleve on 2/23/2022.
//
#pragma once

// Check the libc++ version
#include <version>

#ifdef __clang__ // clang request libc++
#ifndef _LIBCPP_VERSION
#error "chclang must be link against libc++"
#elif _LIBCPP_VERSION < 13000
#error "chclang must be link against libc++-13 and above"
#endif
#endif

#include <string_view>

namespace chclang::config
{

struct predefined_configurations
{
	static inline constexpr std::string_view PROGRAM_NAME = "chclang";
	static inline constexpr  std::string_view  PROGRAM_VERSION = "0.0.1";

	static inline constexpr size_t MAX_FILES=256;
};
}