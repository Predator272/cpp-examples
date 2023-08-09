#include "util.hpp"

class tokenizer
{
public:
	using asv = lang::analyzer::string_view;
public:
	static std::list<asv::token> parse(const std::string_view& input)
	{
		std::list<asv::token> result{};
		size_t index{};
		while (index != input.size())
		{
			const auto token{ asv::match<
				asv::repeat<asv::any<'\t', '\n', '\v', '\f', '\r', ' '>>,
				asv::any<'+', '-', '*', '/', '(', ')', ';'>,
				asv::all<asv::repeat<asv::range<'0', '9'>>, asv::repeat<asv::all<'.', asv::repeat<asv::range<'0', '9'>>>, 0, 1>>
				>(input.substr(index)) };

			if (!token.second.empty())
			{
				result.push_back(token);
				index += token.second.size();
			}
			else
			{
				throw std::runtime_error{ std::format("undefined token") };
			}
		}
		return result;
	}
};

class parser
{
public:
	static void parse(const std::string_view& input)
	{
		//std::println(std::cout, "\"{}\"", lang::new_object<lang::binary_add>(
		//	lang::new_object<lang::constant>(lang::new_object<lang::value_string>("8")),
		//	lang::new_object<lang::constant>(lang::new_object<lang::value_string>("32"))
		//)->eval()->to_string());

		const auto tokens{ tokenizer::parse(input) };

		for (const auto& token : tokens)
		{
			std::print(std::cout, "{}[{}]\r\n", token.first, token.second);
		}
	}
};


int main(int argc, char* argv[])
{
	while (true)
	{
		try
		{
			std::print(std::cout, ">>>");
			std::string input{};
			std::getline(std::cin, input);
			parser::parse(input);
		}
		catch (const std::exception& exception)
		{
			std::print(std::cerr, "error: {}\r\n", exception.what());
		}
	}
	return 0;
}
