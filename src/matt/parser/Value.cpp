#include "Value.h"
#include <stdexcept>
#include <iostream>
#include <ranges>
#include <sstream>
#include <charconv>


matt::parser::Value::Value()
	:mData(std::monostate{})
{
}

matt::parser::Value::Value(bool v)
	:mData(v)
{
}

matt::parser::Value::Value(int v)
	:mData(static_cast<int64_t>(v))
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

matt::parser::Value::Value(List&& v) noexcept
	:mData(std::move(v))
{
}

matt::parser::Value::Value(Map&& v) noexcept
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

matt::parser::Value& matt::parser::Value::operator=(const Map& map)
{
	mData = map;
	return *this;
}

matt::parser::Value& matt::parser::Value::operator=(const List& list)
{
	mData = list;
	return *this;
}

matt::parser::Value& matt::parser::Value::operator=(Map&& map) noexcept
{
	mData = std::move(map);
	return *this;
}

matt::parser::Value& matt::parser::Value::operator=(List&& list) noexcept
{
	mData = std::move(list);
	return *this;
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

const matt::parser::Value& matt::parser::Value::at(std::string_view key) const
{
	if (!isMap())
		throw std::runtime_error("Value .at() - is not a map");
	const auto& map = asMap();
	auto it = map.find(key);
	if (it != std::end(map))
		return it->second;

	throw std::runtime_error("Value .at() - key doesn't exist");
}

const matt::parser::Value& matt::parser::Value::get(std::string_view dottedPath) const
{
	static Value nullValue{};
	auto splitStr = std::ranges::views::split(dottedPath, '.');
	const Value* current = this;

	for (auto&& token : splitStr)
	{
		if (token.begin() == token.end())
			return nullValue;

		std::string_view key{&*token.begin(), static_cast<size_t>(std::ranges::size(token))};
		if (current->isMap())
		{
			const auto& map = current->asMap();
			auto it = map.find(key);
			if (it == std::end(map))
				return nullValue;

			current = &it->second;
		}
		else if (current->isList())
		{
			//Check if the key is numeric one.
			size_t index = 0;
			auto [ptr, ec] = std::from_chars(key.data(), key.data() + key.size(), index);
			if (ec == std::errc() && ptr == key.data() + key.size())
			{
				const auto& list = current->asList();
				if (index >= list.size())
					return nullValue;
				current = &list[index];
			}
		}
		else
			return nullValue;

	}
	return *current;
}

bool matt::parser::Value::contains(std::string_view key) const
{
	if(!isMap())
		return false;

	return asMap().contains(key);
}

void matt::parser::Value::debugPrint(int indent) const
{
	serialize(std::cout, 0);
}

std::string matt::parser::Value::emitString() const
{
	std::stringstream stream;
	serialize(stream, 0);
	return stream.str();
}

void matt::parser::Value::serialize(std::ostream& os, int indent) const
{
	std::string space(indent, ' ');         
	std::string nextSpace(indent + 2, ' '); 

	if (isString()) 
		os << "\"" << asString() << "\"";
	
	else if (isBool()) 
		os << (asBool() ? "true" : "false");
	
	else if (isInt()) 
		os << asInt();
	
	else if (isDouble()) 
		os << asDouble();
	
	else if (isList()) 
	{
		const auto& list = asList();
		if (list.empty()) 
		{
			os << "[]";
			return;
		}
		os << "[\n";
		for (size_t i = 0; i < list.size(); ++i) 
		{
			os << nextSpace;
			list[i].serialize(os, indent + 2);
			if (i < list.size() - 1)
				os << ",";
			os << "\n";
		}
		os << space << "]";
	}
	else if (isMap()) 
	{
		const auto& map = asMap();
		if (map.empty()) 
		{
			os << "{}";
			return;
		}
		os << "{\n";
		bool first = true;
		for (const auto& [key, value] : map) 
		{
			if (!first) 
				os << ",\n";
			os << nextSpace <<  key << ": ";
			value.serialize(os, indent + 2);
			first = false;
		}
		os << "\n" << space << "}";
	}
	else if (isMonostate()) 
		os << "null";
}
