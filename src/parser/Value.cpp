#include "Value.h"
#include <stdexcept>
#include <iostream>
matt::parser::Value::Value()
	:mData(std::monostate{})
{
}

matt::parser::Value::Value(bool v)
	:mData(v)
{
}

matt::parser::Value::Value(std::int64_t v)
	:mData(v)
{
}

matt::parser::Value::Value(double v)
	:mData(v)
{
}

matt::parser::Value::Value(const std::string& v)
	:mData(v)
{
}

matt::parser::Value::Value(std::string_view v)
	:mData(std::string(v))
{
}

matt::parser::Value::Value(const List& v)
	:mData(v)
{
}

matt::parser::Value::Value(const Map& v)
	:mData(v)
{
}

matt::parser::Value::Value(std::string&& v)
	:mData(std::move(v))
{
}

matt::parser::Value::Value(List&& v)
	:mData(std::move(v))
{
}

matt::parser::Value::Value(Map&& v)
	:mData(std::move(v))
{
}

matt::parser::Value& matt::parser::Value::operator[](const std::string& key)
{
	if (std::holds_alternative<Map>(mData))
		return std::get<Map>(mData)[key];

	mData = Map{};
	return std::get<Map>(mData)[key];
}

matt::parser::Value& matt::parser::Value::operator[](size_t index)
{
	if (isList())
	{
		auto& list = asList();
		if (index >= list.size())
			throw std::out_of_range("Value::operator[] - index out of bounds.");

		return list[index];
	}
	throw std::runtime_error("Value::operator[] - Value is not a List.");
}

const matt::parser::Value& matt::parser::Value::operator[](size_t index) const
{
	if (isList())
	{
		const auto& list = asList();
		return list.at(index);
	}

	throw std::runtime_error("Value::operator[] const - Value is not a List.");
}

bool& matt::parser::Value::asBool()
{
	return std::get<bool>(mData);
}

std::int64_t& matt::parser::Value::asInt()
{
	return std::get<std::int64_t>(mData);
}

double& matt::parser::Value::asDouble()
{
	return std::get<double>(mData);
}

std::string& matt::parser::Value::asString()
{
	return std::get<std::string>(mData);
}

matt::parser::Value::List& matt::parser::Value::asList()
{
	return std::get<List>(mData);
}

matt::parser::Value::Map & matt::parser::Value::asMap()
{
	return std::get<Map>(mData);
}

bool matt::parser::Value::asBool() const
{
	return std::get<bool>(mData);
}

std::int64_t matt::parser::Value::asInt() const
{
	return std::get<std::int64_t>(mData);
}

double matt::parser::Value::asDouble() const
{
	return std::get<double>(mData);
}

float matt::parser::Value::asFloat() const
{
	return static_cast<float>(asDouble());
}

const std::string& matt::parser::Value::asString() const
{
	return std::get<std::string>(mData);
}

const matt::parser::Value::List& matt::parser::Value::asList() const
{
	return std::get<List>(mData);
}

const matt::parser::Value::Map& matt::parser::Value::asMap() const
{
	return std::get<Map>(mData);
}

bool matt::parser::Value::isMonostate() const
{
	return std::holds_alternative<std::monostate>(mData);
}

bool matt::parser::Value::isBool() const
{
	return std::holds_alternative<bool>(mData);
}

bool matt::parser::Value::isInt() const
{
	return std::holds_alternative<std::int64_t>(mData);
}

bool matt::parser::Value::isDouble() const
{
	return std::holds_alternative<double>(mData);
}

bool matt::parser::Value::isString() const
{
	return std::holds_alternative<std::string>(mData);
}

bool matt::parser::Value::isList() const
{
	return std::holds_alternative<List>(mData);
}

bool matt::parser::Value::isMap() const
{
	return std::holds_alternative<Map>(mData);
}

void matt::parser::Value::debugPrint(int indent) const
{
	std::string space(indent, ' '); // Create a string of 'indent' spaces

	if (isBool()) 
	{
		std::cout << (asBool() ? "true" : "false");
	}
	else if (isInt()) 
	{
		std::cout << asInt();
	}
	else if (isDouble()) 
	{
		std::cout << asDouble();
	}
	else if (isString()) 
	{
		std::cout << "\"" << asString() << "\"";
	}
	else if (isList()) 
	{
		const auto& list = asList();
		std::cout << "[\n";
		for (size_t i = 0; i < list.size(); ++i)
		{
			std::cout << space << "  "; // Indent the items
			list[i].debugPrint(indent + 2);
			if (i < list.size() - 1) std::cout << ",";
			std::cout << "\n";
		}
		std::cout << space << "]";
	}
	else if (isMap()) 
	{
		const auto& map = asMap();
		std::cout << "{\n";
		for (auto it = map.begin(); it != map.end(); ++it) 
		{
			std::cout << space << it->first << ": ";
			it->second.debugPrint(indent + 2);
			if (std::next(it) != map.end()) std::cout << ",";
			std::cout << "\n";
		}
		std::cout << space << "}";
	}
}
