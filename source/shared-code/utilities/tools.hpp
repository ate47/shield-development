#pragma once

#include "string.hpp"

#include <optional>
// #include <type_traits>

namespace utilities::tools
{
	template <typename ReturnType, typename ArgType>
	class PeriodicUpdate
	{
	private:
		std::optional<ReturnType> cachedResult;
		int callCounter = 0;
		int recomputeThreshold;
		ReturnType (*method)(ArgType);

		// void printArg(const ArgType& arg)
		// {
		// 	if constexpr (std::is_same_v<ArgType, std::string>)
		// 	{
		// 		logger::write(logger::LOG_TYPE_INFO, utilities::string::va("updated %s", arg.c_str()));
		// 	}
		// 	else if constexpr (std::is_arithmetic_v<ArgType>)
		// 	{
		// 		logger::write(logger::LOG_TYPE_INFO, utilities::string::va("updated %s", std::to_string(arg).c_str()));
		// 	}
		// }

	public:
		PeriodicUpdate(ReturnType (*func)(ArgType), int threshold) : recomputeThreshold(threshold), method(func) {}

		ReturnType operator()(ArgType arg)
		{
			if (!cachedResult.has_value())
			{
				cachedResult = (*method)(arg);
			}
			callCounter++;
			if (callCounter >= recomputeThreshold)
			{
				cachedResult = (*method)(arg);
				callCounter = 0;
				// printArg(arg);
			}
			return cachedResult.value();
		}

		void setThreshold(int threshold)
		{
			recomputeThreshold = threshold;
		}
	};
}