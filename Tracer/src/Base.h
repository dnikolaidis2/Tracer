#pragma once

#include <cmath>
#include <limits>
#include <numbers>
#include <vector>
#include <random>
#include <chrono>
#include <memory>
#include <thread>
#include <atomic>

const double infinity = std::numeric_limits<double>::infinity();
const double epsilon = std::numeric_limits<double>::epsilon();
const double pi = std::numbers::pi;

namespace TC {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}