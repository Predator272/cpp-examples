#pragma once

#include <iostream>
#include <list>

#include "lang.hpp"

namespace lang
{
	template<typename type>
	using object = std::shared_ptr<type>;

	template<typename type, typename... types>
	inline object<type> new_object(const types&... arguments) { return std::make_shared<type>(arguments...); }

	using float64 = double;
	using string = std::string;


	class value
	{
	public:
		virtual float64 to_float64() = 0;
		virtual string to_string() = 0;
	};

	class expression
	{
	public:
		virtual object<value> eval() = 0;
	};


	class value_float64 : public value
	{
	public:
		value_float64(float64 value) : _value{ value } {}
	public:
		float64 to_float64() override { return _value; }
		string to_string() override { return std::to_string(_value); }
	private:
		float64 _value;
	};

	class value_string : public value
	{
	public:
		value_string(const string& value) : _value{ value } {}
	public:
		float64 to_float64() override { try { return std::stod(_value); } catch (...) { return 0.0; } }
		string to_string() override { return _value; }
	private:
		string _value;
	};


	class constant : public expression
	{
	public:
		constant(const object<value>& value) : _value{ value } {}
	public:
		object<value> eval() override { return _value; }
	private:
		object<value> _value;
	};

	class unary_plus : public expression
	{
	public:
		unary_plus(const object<expression>& expr) : _expr{ expr } {}
	public:
		object<value> eval() override { return new_object<value_float64>(+_expr->eval()->to_float64()); }
	private:
		const object<expression> _expr;
	};

	class unary_minus : public expression
	{
	public:
		unary_minus(const object<expression>& expr) : _expr{ expr } {}
	public:
		object<value> eval() override { return new_object<value_float64>(-_expr->eval()->to_float64()); }
	private:
		const object<expression> _expr;
	};

	class binary_add : public expression
	{
	public:
		binary_add(const object<expression>& expr1, const object<expression>& expr2) : _expr1{ expr1 }, _expr2{ expr2 } {}
	public:
		object<value> eval() override { return new_object<value_float64>(_expr1->eval()->to_float64() + _expr2->eval()->to_float64()); }
	private:
		const object<expression> _expr1, _expr2;
	};

	class binary_sub : public expression
	{
	public:
		binary_sub(const object<expression>& expr1, const object<expression>& expr2) : _expr1{ expr1 }, _expr2{ expr2 } {}
	public:
		object<value> eval() override { return new_object<value_float64>(_expr1->eval()->to_float64() - _expr2->eval()->to_float64()); }
	private:
		const object<expression> _expr1, _expr2;
	};

	class binary_mul : public expression
	{
	public:
		binary_mul(const object<expression>& expr1, const object<expression>& expr2) : _expr1{ expr1 }, _expr2{ expr2 } {}
	public:
		object<value> eval() override { return new_object<value_float64>(_expr1->eval()->to_float64() * _expr2->eval()->to_float64()); }
	private:
		const object<expression> _expr1, _expr2;
	};

	class binary_div : public expression
	{
	public:
		binary_div(const object<expression>& expr1, const object<expression>& expr2) : _expr1{ expr1 }, _expr2{ expr2 } {}
	public:
		object<value> eval() override { return new_object<value_float64>(_expr1->eval()->to_float64() / _expr2->eval()->to_float64()); }
	private:
		const object<expression> _expr1, _expr2;
	};
}
