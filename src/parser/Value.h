#pragma once
#include <map>
#include <vector>
#include <string>
#include <string_view>
#include <variant>

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
		using Map = std::map<std::string, Value>;
	
		using ValueType = std::variant
		<
			std::monostate,
			bool,
			std::int64_t,
			double,
			std::string,
			List,
			Map
		>;
	public:
		Value();
		Value(bool v);
		Value(std::int64_t v);
		Value(double v);
		Value(const std::string& v);
		Value(std::string_view v);
		Value(const List& v);
		Value(const Map& v);
		Value(std::string&& v);
		Value(List&& v);
		Value(Map&& v);

		Value& operator[](const std::string& key);
		Value& operator[](size_t index);
		const Value& operator[](size_t index) const;

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

		//For Debug Information
		void debugPrint(int indent = 0) const;
	private:
		ValueType mData;
	};

}