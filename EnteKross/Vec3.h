#pragma once

#include <cmath>
#include <cassert>

template<typename T>
class _Vec3
{
public:
	_Vec3() = default;
	_Vec3(T x, T y, T z) noexcept : x(x), y(y), z(z) {}
	_Vec3(const _Vec3& src) noexcept : x(src.x), y(src.y), z(src.z) {}
	_Vec3& operator =(const _Vec3& src) noexcept
	{
		x = src.x;
		y = src.y;
		z = src.z;
		return *this;
	}
public:
	_Vec3 operator +(const _Vec3& rhs) const noexcept
	{
		return _Vec3{ x + rhs.x, y + rhs.y, z + rhs.z };
	}
	_Vec3 operator -(const _Vec3& rhs) const noexcept
	{
		return _Vec3{ x - rhs.x, y - rhs.y, z - rhs.z };
	}
	_Vec3 operator *(const T& rhs) const noexcept
	{
		return _Vec3{ x * rhs, y * rhs, z * rhs };
	}
	_Vec3& operator +=(const _Vec3& rhs) noexcept
	{
		return *this = *this + rhs;
	}
	_Vec3& operator -=(const _Vec3& rhs) noexcept
	{
		return *this = *this - rhs;
	}
	_Vec3& operator *=(const T& rhs) noexcept
	{
		return *this = *this * rhs;
	}
	_Vec3 operator -() const { return { -x, -y, -z }; }
	template<typename S>
	explicit operator _Vec3<S>() const
	{
		return { S(x), S(y), S(z) };
	}
public:
	T GetLengthSqr() const noexcept
	{
		return x * x + y * y + z * z;
	}
	T GetLength() const noexcept
	{
		std::sqrt(GetLengthSqr());
	}
	_Vec3 GetNormalized() const noexcept(!IS_DEBUG)
	{
		T length = GetLength();
		assert(length != 0);
		return _Vec3{ x,y,z } * (1.0f / length);
	}
	void Normalize() noexcept(!IS_DEBUG)
	{
		*this = GetNormalized();
	}
public:
	static _Vec3 Dot(const _Vec3& lhs, const _Vec3& rhs) noexcept
	{
		return _Vec3
		{
			lhs.y * rhs.z - lhs.z * rhs.y,
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x
		};
	}
	static _Vec3 Zero() noexcept
	{
		return _Vec3{ T(0), T(0), T(0) };
	}
public:
	T x;
	T y;
	T z;
};


template<typename T>
_Vec3<T> operator *(const T& scalar, _Vec3<T> rhs)
{
	return rhs *= scalar;
}

typedef _Vec3<float> Vec3;
typedef _Vec3<int> Vei3;