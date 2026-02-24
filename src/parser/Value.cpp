#include "Value.h"

matt::parser::Value::Value()
	:mData(nullptr)
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

bool matt::parser::Value::isNull() const
{
	return std::holds_alternative<std::nullptr_t>(mData);
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
