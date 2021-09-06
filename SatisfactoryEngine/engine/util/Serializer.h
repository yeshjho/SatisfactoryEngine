#pragma once
#include <iostream>



namespace stf
{
inline namespace detail
{
template<typename T>
concept IsReserveable = requires(T t) { t.reserve(0); };

template<typename T>
concept IsResizeable = requires(T t) { t.resize(0); };

template<typename T>
concept IsEmplaceable = requires(T t, typename T::value_type&& value) { t.emplace(value); };

template<typename T>
concept TriviallyCopyable = std::is_trivially_copyable_v<T>;

template<typename T>
concept Container = requires(T t, typename T::value_type value)
{
	requires std::ranges::range<T>;
	t.size();
};

template<typename T>
concept ConsecutiveAndTriviallyCopyableContainer = requires(T t)
{
	requires Container<T>;
	requires TriviallyCopyable<typename T::value_type>;
	t.data();
};

template<typename T, typename Serializer>
concept SerializeableContainer = requires(T t, const typename T::value_type& value, Serializer serializer)
{
	requires Container<T>;
	serializer << value;
};

template<typename T, typename Serializer>
concept DeserializeableContainer = requires(T t, typename T::value_type& value)
{
	requires Container<T>;
	std::declval<Serializer>() >> value;
	requires IsResizeable<T> || 
		(std::is_default_constructible_v<typename T::value_type> && IsEmplaceable<T>);
};
}



struct Serializer
{
	constexpr Serializer(std::ostream* out, std::istream* in) noexcept;
	constexpr Serializer(std::istream* in, std::ostream* out) noexcept;
	constexpr Serializer(std::ostream* out) noexcept;
	constexpr Serializer(std::istream* in) noexcept;
	constexpr Serializer(std::ostream& out, std::istream& in) noexcept;
	constexpr Serializer(std::istream& in, std::ostream& out) noexcept;
	constexpr Serializer(std::ostream& out) noexcept;
	constexpr Serializer(std::istream& in) noexcept;



	std::ostream* out = nullptr;
	std::istream* in = nullptr;
};

// TODO: report error if nullptr
struct [[nodiscard]] PlainSerializer final : Serializer
{
	using Serializer::Serializer;


	// Converts to char*
	template<TriviallyCopyable T>
	PlainSerializer& operator<<(const T& value);

	// Converts value.data() to char* 
	template<ConsecutiveAndTriviallyCopyableContainer T>
	PlainSerializer& operator<<(const T& value);

	// Iterates through the items and call operator <<
	template<SerializeableContainer<PlainSerializer> T>
		requires (!ConsecutiveAndTriviallyCopyableContainer<T>)
	PlainSerializer& operator<<(const T& value);
		
	template<typename T, typename S>
	PlainSerializer& operator<<(const std::pair<T, S>& value);

	template<typename... Types>
	PlainSerializer& operator<<(const std::tuple<Types...>& value);


	// Converts to char*
	template<TriviallyCopyable T>
	PlainSerializer& operator>>(T& value);

	// Converts value.data() to char* 
	template<ConsecutiveAndTriviallyCopyableContainer T>
	PlainSerializer& operator>>(T& value);

	// Iterates through the items and call operator >>
	template<DeserializeableContainer<PlainSerializer> T>
		requires (!ConsecutiveAndTriviallyCopyableContainer<T>)
	PlainSerializer& operator>>(T& value);

	template<typename T, typename S>
	PlainSerializer& operator>>(std::pair<T, S>& value);

	template<typename... Types>
	PlainSerializer& operator>>(std::tuple<Types...>& value);


private:
	template<typename Tuple, std::size_t... I>
	void serializeTuple(const Tuple& tuple, std::index_sequence<I...>);

	template<typename Tuple, std::size_t... I>
	void deserializeTuple(Tuple& tuple, std::index_sequence<I...>);
};
}

#include "Serializer.inl"
