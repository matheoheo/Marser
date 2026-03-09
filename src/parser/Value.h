#pragma once
#include <map>
#include <vector>
#include <string>
#include <string_view>
#include <variant>
#include <concepts>
#include <type_traits>

/*
	Value class represents an universal type of data used by Parser.
	It can hold numbers, strings, lists, maps, bools
*/

namespace matt::parser
{
	class Value
	{
		
	public:
		using List = std::vector<Value>;
		using Map = std::map<std::string, Value, std::less<>>;
	
		using ValueType = std::variant
			<
			std::monostate,
			bool,
			std::int64_t,
			double,
			std::string,
			std::vector<Value>,
			std::map<std::string, Value, std::less<>>
			>;
	public:
		explicit Value();
		explicit Value(bool v);
		explicit Value(std::int64_t v);
		explicit Value(double v);
		explicit Value(const std::string& v);
		explicit Value(std::string_view v);
		explicit Value(const List& v);
		explicit Value(const Map& v);
		explicit Value(std::string&& v);
		explicit Value(List&& v) noexcept;
		explicit Value(Map&& v) noexcept;

		Value& operator[](const std::string& key);
		Value& operator[](size_t index);
		const Value& operator[](size_t index) const;
		Value& operator=(const Map& map);
		Value& operator=(const List& list);
		Value& operator=(Map&& map) noexcept;
		Value& operator=(List&& list) noexcept;

		template <typename T>
		requires requires(ValueType val, T arg) {
			val = std::forward<T>(arg);
		}
		inline Value& operator=(T&& other)
		{
			mData = other;
			return *this;
		}

		bool& asBool();
		std::int64_t& asInt();
		double& asDouble();
		std::string& asString();
		List& asList();
		Map& asMap();

		bool asBool() const;
		std::int64_t asInt() const;
		double asDouble() const;
		float asFloat() const;
		const std::string& asString() const;
		const List& asList() const;
		const Map& asMap() const;
	
		bool isMonostate() const;
		bool isBool() const;
		bool isInt() const;
		bool isDouble() const;
		bool isString() const;
		bool isList() const;
		bool isMap() const;

		const Value& at(std::string_view key) const;
		//Navigates the value tree using dotted path
		//Example of usage: int playerHp = get("player.stats.hp").asInt()
		const Value& get(std::string_view dottedPath) const;
		bool contains(std::string_view key) const;
		//For Debug Information
		void debugPrint(int indent = 0) const;
		//Returns string object representing the whole Value object.
		std::string emitString() const;
	private:
		void serialize(std::ostream& os, int indent = 0) const;
	private:
		ValueType mData;
	};

	/*template<typename T>
	requires requires(Value::ValueType val, T arg) {
		val = std::forward<T>(arg);
	}
	inline Value& Value::operator=(T&& other)
	{
		mData = other;
		return *this;
	}*/

}