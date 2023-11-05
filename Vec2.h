#pragma once

#include <utility>

template <class T1, class T2=T1>
struct Vec2
{
	using x_type = T1;
	using y_type = T2;
	T1 x;
	T2 y;

	Vec2(const Vec2 &v) = default;
	Vec2(Vec2 &&v) = default;

	constexpr Vec2(const T1 &x, const T2 &y) : x(x), y(y) {}

	template <class U1 = T1, class U2 = T2>
	constexpr Vec2(U1 &&x, U2 &&y) :
		x(std::forward<U1>(x)),
		y(std::forward<U2>(y)) {
	}

	template <class U1, class U2>
	constexpr Vec2& operator=(const Vec2<U1, U2> &v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	template <class U1, class U2>
	constexpr Vec2& operator=(Vec2<U1, U2> &&v) {
		x = std::forward<U1>(v.x);
		y = std::forward<U2>(v.y);
		return *this;
	}

	template <class U1, class U2>
	constexpr auto operator+=(const Vec2<U1, U2> &o)
	{
		x += o.x;
		y += o.y;
		return *this;
	}

	template <class U1, class U2>
	constexpr auto operator*=(const Vec2<U1, U2> &o)
	{
		x *= o.x;
		y *= o.y;
		return *this;
	}
};

template <class T1, class T2>
Vec2<T1, T2> makeVec2(T1 x, T2 y)
{
	return {x, y};
}