#pragma once
#include <string_view>
#include <concepts>
#include <source_location>
#include <iostream>
#define MATT_INFO(...)  Logger::info(std::source_location::current(), __VA_ARGS__)
#define MATT_WARN(...)  Logger::warn(std::source_location::current(), __VA_ARGS__)
#define MATT_ERROR(...) Logger::error(std::source_location::current(), __VA_ARGS__)


namespace matt
{
	template <typename T>
	concept Printable = requires(T t, std::ostream out)
	{
		{out << t} -> std::same_as<std::ostream&>;
	};

	class Logger
	{
	public:
		template<Printable... T>
		static void info(std::source_location loc, T&&... t);
		template<Printable... T>
		static void warn(std::source_location loc, T&&... t);
		template<Printable... T>
		static void error(std::source_location loc, T&&... t);
	private:
		static std::string_view extractFilename(std::string_view fullName);
		static void printLocation(std::source_location loc);
		template<Printable... T>
		static void logImpl(std::source_location loc, T&&... t);
	};

	template<Printable ...T>
	inline void Logger::info(std::source_location loc, T && ...t)
	{
		std::cout << "[INFO]  ";
		logImpl(loc, std::forward<T>(t)...);
	}
	template<Printable ...T>
	inline void Logger::warn(std::source_location loc, T && ...t)
	{
		std::cout << "[WARN]  ";
		logImpl(loc, std::forward<T>(t)...);
	}
	template<Printable ...T>
	inline void Logger::error(std::source_location loc, T && ...t)
	{
		std::cout << "[ERROR] ";
		logImpl(loc, std::forward<T>(t)...);
	}
	template<Printable ...T>
	inline void Logger::logImpl(std::source_location loc, T && ...t)
	{
		printLocation(loc);
		((std::cout << t << " "), ...);
		std::cout << '\n';
	}
}