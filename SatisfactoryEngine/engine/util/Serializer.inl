#pragma once



namespace stf
{
constexpr Serializer::Serializer(std::ostream* out, std::istream* in) noexcept
	: out(out), in(in)
{}


constexpr Serializer::Serializer(std::istream* in, std::ostream* out) noexcept
	: Serializer(out, in)
{}


constexpr Serializer::Serializer(std::ostream* out) noexcept
	: Serializer(out, nullptr)
{}


constexpr Serializer::Serializer(std::istream* in) noexcept
	: Serializer(nullptr, in)
{}


constexpr Serializer::Serializer(std::ostream& out, std::istream& in) noexcept
	: Serializer(&out, &in)
{}


constexpr Serializer::Serializer(std::istream& in, std::ostream& out) noexcept
	: Serializer(&in, &out)
{}


constexpr Serializer::Serializer(std::ostream& out) noexcept
	: Serializer(&out, nullptr)
{}


constexpr Serializer::Serializer(std::istream& in) noexcept
	: Serializer(nullptr, &in)
{}


template <TriviallyCopyable T>
PlainSerializer& PlainSerializer::operator<<(const T& value)
{
	out->write(reinterpret_cast<const char*>(&value), sizeof(T));

	return *this;
}


template <ConsecutiveAndTriviallyCopyableContainer T>
PlainSerializer& PlainSerializer::operator<<(const T& value)
{
	const size_t size = value.size();
	out->write(reinterpret_cast<const char*>(&size), sizeof(size_t));
	out->write(reinterpret_cast<const char*>(value.data()), sizeof(typename T::value_type) * size);

	return *this;
}


template <SerializeableContainer<PlainSerializer> T>
	requires (!ConsecutiveAndTriviallyCopyableContainer<T>)
PlainSerializer& PlainSerializer::operator<<(const T& value)
{
	const size_t size = value.size();
	out->write(reinterpret_cast<const char*>(&size), sizeof(size_t));

	for (const auto& item : value)
	{
		*this << item;
	}

	return *this;
}


template <typename T, typename S>
PlainSerializer& PlainSerializer::operator<<(const std::pair<T, S>& value)
{
	*this << value.first << value.second;

	return *this;
}


template <typename ... Types>
PlainSerializer& PlainSerializer::operator<<(const std::tuple<Types...>& value)
{
	serializeTuple(value, std::make_index_sequence<sizeof...(Types)>{});

	return *this;
}


template <TriviallyCopyable T>
PlainSerializer& PlainSerializer::operator>>(T& value)
{
	in->read(const_cast<char*>(reinterpret_cast<const volatile char*>(&value)), sizeof(T));

	return *this;
}


template <ConsecutiveAndTriviallyCopyableContainer T>
PlainSerializer& PlainSerializer::operator>>(T& value)
{
	size_t size;
	in->read(reinterpret_cast<char*>(&size), sizeof(size_t));

	if constexpr (IsResizeable<T>)
	{
		value.resize(size);
	}
	in->read(const_cast<char*>(reinterpret_cast<const volatile char*>(value.data())),
		sizeof(typename T::value_type) * size);

	return *this;
}


template <DeserializeableContainer<PlainSerializer> T>
	requires (!ConsecutiveAndTriviallyCopyableContainer<T>)
PlainSerializer& PlainSerializer::operator>>(T& value)
{
	size_t size;
	in->read(reinterpret_cast<char*>(&size), sizeof(size_t));

	if constexpr (IsReserveable<T>)
	{
		value.reserve(size);
	}

	using ValueType = typename T::value_type;
	if constexpr (IsResizeable<T>)
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


template <typename T, typename S>
PlainSerializer& PlainSerializer::operator>>(std::pair<T, S>& value)
{
	*this >> value.first >> value.second;

	return *this;
}


template <typename ... Types>
PlainSerializer& PlainSerializer::operator>>(std::tuple<Types...>& value)
{
	deserializeTuple(value, std::make_index_sequence<sizeof...(Types)>{});

	return *this;
}


template <typename Tuple, std::size_t... I>
void PlainSerializer::serializeTuple(const Tuple& tuple, [[maybe_unused]] std::index_sequence<I...>)
{
	(*this << ... << std::get<I>(tuple));
}


template <typename Tuple, std::size_t... I>
void PlainSerializer::deserializeTuple(Tuple& tuple, [[maybe_unused]] std::index_sequence<I...>)
{
	(*this >> ... >> std::get<I>(tuple));
}
}
