#pragma once

#include <string>
#include <numeric>
#include <algorithm>

namespace lang
{
	namespace analyzer
	{
		template<typename container, typename element>
		class basic
		{
		public:
			using token = std::pair<size_t, container>;
			using callback = bool(const container&, size_t&);
			using callback_noexcept = bool(const container&, size_t&) noexcept;
		public:
			template<callback... expression>
			static constexpr token match(const container& input) noexcept
			{
				const auto begin{ std::begin(input) };
				size_t counter{ 0 };
				for (const auto element : { expression... })
				{
					size_t index{ 0 };
					if (element(input, index))
					{
						return { counter, { begin, begin + index } };
					}
					counter++;
				}
				return { ~0ull, { begin, begin } };
			}
		public:
			template<element min, element max>
			static constexpr bool range(const container& input, size_t& index) noexcept
			{
				const auto current{ peek(input, index) };
				if ((current >= min) && (current <= max))
				{
					index++;
					return true;
				}
				return false;
			}

			template<auto... values>
			static constexpr bool all(const container& input, size_t& index) noexcept
			{
				return ((match_primary<values>(input, index)) && ...);
			}

			template<auto... values>
			static constexpr bool any(const container& input, size_t& index) noexcept
			{
				return ((match_primary<values>(input, index)) || ...);
			}

			template<callback expression, size_t min = 1ull, size_t max = ~0ull>
			static constexpr bool repeat(const container& input, size_t& index) noexcept
			{
				size_t temp{ index }, counter{};
				while ((counter != max) && match_primary<expression>(input, temp))
				{
					counter++;
				}
				if (counter >= min)
				{
					if (counter != 0)
					{
						index = temp;
					}
					return true;
				}
				return false;
			}
		private:
			static constexpr element peek(const container& input, size_t index) noexcept
			{
				auto iterator{ std::begin(input) };
				std::advance(iterator, std::min<size_t>(index, input.size()));
				return (iterator == std::end(input)) ? element{} : (*iterator);
			}

			template<auto value>
			static constexpr bool match_primary(const container& input, size_t& index) noexcept
			{
				constexpr auto is_callback{ std::is_same<decltype(value), callback*>::value };
				constexpr auto is_callback_noexcept{ std::is_same<decltype(value), callback_noexcept*>::value };
				constexpr auto is_element{ std::is_same<decltype(value), element>::value };
				static_assert(is_callback || is_callback_noexcept || is_element, "invalid arguments");

				if constexpr (is_callback || is_callback_noexcept)
				{
					return value(input, index);
				}
				else if constexpr (is_element)
				{
					if (peek(input, index) == value)
					{
						index++;
						return true;
					}
					return false;
				}
			}
		};

		using string_view = basic<std::string_view, std::string_view::value_type>;

		template<typename type>
		using list = basic<std::list<type>, std::list<type>::value_type>;
	}
}
