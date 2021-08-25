#pragma once
#include <iostream>



namespace stf
{
inline namespace detail
{
template<typename T, typename = void>
struct IsReservable : std::false_type {};

template<typename T>
struct IsReservable<T, std::void_t<decltype(std::declval<T>().reserve(0))>> : std::true_type {};


template<typename T, typename = void>
struct IsResizable : std::false_type {};

template<typename T>
struct IsResizable<T, std::void_t<decltype(std::declval<T>().resize(0))>> : std::true_type {};


template<typename T, typename = void>
struct IsEmplacable : std::false_type {};

template<typename T>
struct IsEmplacable<T, std::void_t<decltype(std::declval<T>().emplace(std::declval<typename T::value_type&&>()))>> : std::true_type {};



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
concept SerializeableContainer = requires(T t, const typename T::value_type& value)
{
	requires Container<T>;
	std::declval<Serializer>() << value;
};

template<typename T, typename Serializer>
concept DeserializeableContainer = requires(T t, typename T::value_type& value)
{
	requires Container<T>;
	std::declval<Serializer>() >> value;
	requires IsResizable<T>::value || 
		(std::is_default_constructible_v<typename T::value_type> && IsEmplacable<T>::value);
};
}

struct Serializer
{};

// TODO: report error if nullptr
struct PlainSerializer final : Serializer
{
	PlainSerializer(std::ostream* out, std::istream* in)
		: out(out), in(in)
	{}

	PlainSerializer(std::istream* in, std::ostream* out)
		: PlainSerializer(out, in)
	{}

	PlainSerializer(std::ostream* out)
		: PlainSerializer(out, nullptr)
	{}

	PlainSerializer(std::istream* in)
		: PlainSerializer(nullptr, in)
	{}

	PlainSerializer(std::ostream& out, std::istream& in)
		: PlainSerializer(&out, &in)
	{}

	PlainSerializer(std::istream& in, std::ostream& out)
		: PlainSerializer(&in, &out)
	{}

	PlainSerializer(std::ostream& out)
		: PlainSerializer(&out, nullptr)
	{}

	PlainSerializer(std::istream& in)
		: PlainSerializer(nullptr, &in)
	{}


	template<TriviallyCopyable T>
	PlainSerializer& operator<<(const T& value)
	{
		out->write(reinterpret_cast<const char*>(&value), sizeof(T));

		return *this;
	}

	template<ConsecutiveAndTriviallyCopyableContainer T>
	PlainSerializer& operator<<(const T& value)
	{
		const size_t size = value.size();
		out->write(reinterpret_cast<const char*>(&size), sizeof(size_t));
		out->write(reinterpret_cast<const char*>(value.data()), sizeof(typename T::value_type) * size);

		return *this;
	}

	template<SerializeableContainer<PlainSerializer> T>
		requires (!ConsecutiveAndTriviallyCopyableContainer<T>)
	PlainSerializer& operator<<(const T& value)
	{
		const size_t size = value.size();
		out->write(reinterpret_cast<const char*>(&size), sizeof(size_t));

		for (const auto& item : value)
		{
			*this << item;
		}

		return *this;
	}

	template<typename T, typename S>
	PlainSerializer& operator<<(const std::pair<T, S>& value)
	{
		*this << value.first << value.second;

		return *this;
	}

	template<typename... Types>
	PlainSerializer& operator<<(const std::tuple<Types...>& value)
	{
		serializeTuple(value, std::make_index_sequence<sizeof...(Types)>{});

		return *this;
	}


	template<TriviallyCopyable T>
	PlainSerializer& operator>>(T& value)
	{
		in->read(const_cast<char*>(reinterpret_cast<const char*>(&value)), sizeof(T));

		return *this;
	}

	template<ConsecutiveAndTriviallyCopyableContainer T>
	PlainSerializer& operator>>(T& value)
	{
		size_t size;
		in->read(reinterpret_cast<char*>(&size), sizeof(size_t));

		if constexpr (IsResizable<T>::value)
		{
			value.resize(size);
		}
		in->read(const_cast<char*>(reinterpret_cast<const char*>(value.data())), 
			sizeof(typename T::value_type) * size);

		return *this;
	}

	template<DeserializeableContainer<PlainSerializer> T>
		requires (!ConsecutiveAndTriviallyCopyableContainer<T>)
	PlainSerializer& operator>>(T& value)
	{
		size_t size;
		in->read(reinterpret_cast<char*>(&size), sizeof(size_t));

		if constexpr (IsReservable<T>::value)
		{
			value.reserve(size);
		}

		using ValueType = typename T::value_type;
		if constexpr (IsResizable<T>::value)
		{
			value.resize(size);
			for (ValueType& item : value)
			{
				*this >> item;
			}
		}
		else
		{
			for (size_t i = 0; i < size; i++)
			{
				ValueType item;
				*this >> item;
				value.emplace(std::move(item));
			}
		}

		return *this;
	}

	template<typename T, typename S>
	PlainSerializer& operator>>(std::pair<T, S>& value)
	{
		*this >> value.first >> value.second;

		return *this;
	}

	template<typename... Types>
	PlainSerializer& operator>>(std::tuple<Types...>& value)
	{
		deserializeTuple(value, std::make_index_sequence<sizeof...(Types)>{});

		return *this;
	}



	std::ostream* out = nullptr;
	std::istream* in = nullptr;


private:
	template<typename Tuple, std::size_t... I>
	void serializeTuple(const Tuple& tuple, [[maybe_unused]] std::index_sequence<I...>)
	{
		(*this << ... << std::get<I>(tuple));
	}

	template<typename Tuple, std::size_t... I>
	void deserializeTuple(Tuple& tuple, [[maybe_unused]] std::index_sequence<I...>)
	{
		(*this >> ... >> std::get<I>(tuple));
	}
};
}
