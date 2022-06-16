#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include <limits>
#include <algorithm>

#ifndef HALF_CIRCLE
	#define HALF_CIRCLE 180.0f
#endif

namespace lm
{
	template <typename T> class Vec3
	{
		private:
			T x;
			T y;
			T z;

		public:
			static const T unitVal;
			static const Vec3<T> zero;
			static const Vec3<T> up;
			static const Vec3<T> down;
			static const Vec3<T> left;
			static const Vec3<T> right;
			static const Vec3<T> foward;
			static const Vec3<T> backward;

			Vec3()
			{
				this->x = 0;
				this->y = 0;
				this->z = 0;
			}

			Vec3(const T init)
			{
				this->x = init;
				this->y = init;
				this->z = init;
			}

			Vec3(const T x, const T y, const T z)
			{
				this->x = x;
				this->y = y;
				this->z = z;
			}

			Vec3(const Vec3<T>& vec3)
			{
				this->x = vec3.x;
				this->y = vec3.y;
				this->z = vec3.z;
			}

			Vec3(Vec3<T>&& vec3) noexcept
			{
				this->x = std::move(vec3.x);
				this->y = std::move(vec3.y);
				this->z = std::move(vec3.z);
			}

			Vec3& operator=(const Vec3<T>& vec3)
			{
				if (this == &vec3)
					return *this;

				this->x = vec3.x;
				this->y = vec3.y;
				this->z = vec3.z;

				return *this;
			}

			Vec3& operator=(Vec3<T>&& vec3) noexcept
			{
				if (this == &vec3)
					return *this;

				this->x = std::move(vec3.x);
				this->y = std::move(vec3.y);
				this->z = std::move(vec3.z);

				return *this;
			}
			
			T& X()
			{
				return this->x;
			}

			T& Y()
			{
				return this->y;
			}

			T& Z()
			{
				return this->z;
			}

			const T X() const
			{
				return this->x;
			}

			const T Y() const
			{
				return this->y;
			}

			const T Z() const
			{
				return this->z;
			}

			const T length() const
			{
				return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
			}

			const T length2() const
			{
				return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
			}

			const bool isUnit() const
			{
				return std::abs(this->length() - Vec3<T>::unitVal) <= std::numeric_limits<T>::epsilon() ||
					std::abs(this->length() - Vec3<T>::unitVal) <= std::numeric_limits<T>::epsilon() * std::max(std::abs(this->length()), std::abs(Vec3<T>::unitVal));
			}

			const T dotProduct(const Vec3<T>& vec3) const
			{
				return (this->x * vec3.x) + (this->y * vec3.y) + (this->z * vec3.z);
			}

			const Vec3<T> crossProduct(const Vec3<T>& vec3) const
			{
				return Vec3<T>( (this->y * vec3.z) - (this->z * vec3.y), 
								(this->z * vec3.x) - (this->x * vec3.z),
								(this->x * vec3.y) - (this->y * vec3.x));
			}

			const T radAngle(const Vec3<T>& vec3) const
			{
				const T product = this->dotProduct(vec3);

				const T lengthA2 = this->length2();
				const T lengthB2 = vec3.length2();
				const T length2Product = lengthA2 * lengthB2;

				const T length = sqrt(length2Product);

				return std::acos(product / length);
			}

			const T degAngle(const Vec3<T>& vec3) const
			{
				return radiansToDegrees(this->radAngle(vec3));
			}

			Vec3<T> project(const Vec3<T>& vec3) const
			{
				const T product = this->dotProduct(vec3);
				const T length = vec3.length2();
				const T division = product / length;

				return Vec3<T>(division * vec3.x, division * vec3.y, division * vec3.z);
			}

			Vec3<T> reflexion(const Vec3<T>& vec3) const
			{
				if (!vec3.isUnit())
				{
					Vec3<T> proj = this->project(vec3);
					proj *= 2;
					return Vec3<T>(this->x - proj.x, this->y - proj.y, this->z - proj.z);
				}

				const T product = this->dotProduct(vec3);
				const T doubledProduct = 2 * product;
				const Vec3<T> vecProduct = doubledProduct * vec3;

				return Vec3<T>(this->x - vecProduct.x, this->y - vecProduct.y, this->z - vecProduct.z);
			}

			Vec3<T> perpendicular(const Vec3<T>& vec3) const
			{
				const Vec3<T> proj = this->project(vec3);
				return Vec3<T>(this->x - proj.x, this->y - proj.y, this->z - proj.z);
			}

			Vec3<T> add(const Vec3<T>& vec3) const
			{
				return Vec3<T>(this->x + vec3.x, this->y + vec3.y, this->z + vec3.z);
			}

			Vec3<T>& add(const Vec3<T>& vec3)
			{
				this->x += vec3.x;
				this->y += vec3.y;
				this->z += vec3.z;
				return *this;
			}

			Vec3<T> scale(const T scale) const
			{
				return Vec3<T>(this->x * scale, this->y * scale, this->z * scale);
			}

			Vec3<T>& scale(const T scale)
			{
				this->x *= scale;
				this->y *= scale;
				this->z *= scale;
				return *this;
			}

			Vec3<T>& scale(const Vec3<T>& vec3)
			{
				this->x *= vec3.X();
				this->y *= vec3.Y();
				this->z *= vec3.Z();
				return *this;
			}

			Vec3<T> scaled(const Vec3<T>& vec3)
			{
				return lm::Vec3<T>(this->x * vec3.X(), this->y * vec3.Y(), this->z * vec3.Z());
			}

			Vec3<T> normalized() const
			{
				T length = this->length();
				return Vec3<T>(this->x / length, this->y / length, this->z / length);
			}

			void normalize()
			{
				T length = this->length();
				this->x /= length;
				this->y /= length;
				this->z /= length;
			}

			T& operator[](const int idx)
			{
				switch (idx)
				{
					case 0:		return x;
					case 1:		return y;
					case 2:		return z;
					
					case 'x':	return x;
					case 'y':	return y;
					case 'z':	return z;

					default:	return x;
				}
			}

			const T operator[](const int idx) const
			{
				switch (idx)
				{
				case 0:		return x;
				case 1:		return y;
				case 2:		return z;

				case 'x':	return x;
				case 'y':	return y;
				case 'z':	return z;

				default:	return x;
				}
			}

			T& operator[](const char* idx)
			{
				switch (idx[0])
				{
					case 'x' : return x;
					case 'y' : return y;
					case 'z' : return z;

					default: return x;
				}
			}

			const T operator[](const char* idx) const
			{
				switch (idx[0])
				{
				case 'x': return x;
				case 'y': return y;
				case 'z': return z;

				default: return x;
				}
			}

			const bool operator==(const Vec3<T>& vec3) const
			{
				if (this == &vec3)
					return true;

				return std::abs(this->length() - vec3.length()) <= std::numeric_limits<T>::epsilon() ||
					std::abs(this->length() - vec3.length()) <= std::numeric_limits<T>::epsilon() * std::max(std::abs(this->length()), std::abs(vec3.length()));
			}

			const bool operator!=(const Vec3<T>& vec3) const
			{
				return !(*this == vec3);
			}

			const bool operator<(const Vec3<T>& vec3) const
			{
				return this->length() < vec3.length();
			}

			const bool operator<=(const Vec3<T>& vec3) const
			{
				return this->length() < vec3.length() || (*this == vec3);
			}

			const bool operator>(const Vec3<T>& vec3) const
			{
				return !(*this < vec3);
			}

			const bool operator>=(const Vec3<T>& vec3) const
			{
				return !(*this <= vec3);
			}

			Vec3<T> operator+(const Vec3<T>& vec3) const
			{
				return Vec3(this->x + vec3.x, this->y + vec3.y, this->z + vec3.z);
			}

			void operator+=(const Vec3<T>& vec3)
			{
				this->x += vec3.x;
				this->y += vec3.y;
				this->z += vec3.z;
			}

			Vec3<T> operator-(const Vec3<T>& vec3) const
			{
				return Vec3(this->x - vec3.x, this->y - vec3.y, this->z - vec3.z);
			}

			void operator-=(const Vec3<T>& vec3)
			{
				this->x -= vec3.x;
				this->y -= vec3.y;
				this->z -= vec3.z;
			}

			Vec3<T> operator-() const
			{
				return Vec3(-this->x, -this->y, -this->z);
			}

			Vec3<T> operator*(const T value) const
			{
				return this->scale(value);
			}

			void operator*=(const T value)
			{
				*this = this->scale(value);
			}

			Vec3<T> operator*(const Vec3<T>& vec3) const
			{
				return this->crossProduct(vec3);
			}

			void operator*=(const Vec3<T>& vec3)
			{
				*this = this->crossProduct(vec3);
			}

			Vec3<T> operator/(const T value) const
			{
				return Vec3(this->x / value, this->y / value, this->z / value);
			}

			void operator/=(const T value)
			{
				this->x /= value;
				this->y /= value;
				this->z /= value;
			}

			void operator++(int)
			{
				T length = this->length();
				this->x *= (length + 1) / length;
				this->y *= (length + 1) / length;
				this->z *= (length + 1) / length;
			}

			void operator--(int)
			{
				T length = this->length();
				this->x *= (length - 1) / length;
				this->y *= (length - 1) / length;
				this->z *= (length - 1) / length;
			}

			const double static radiansToDegrees(const double rad)
			{
				return rad * (HALF_CIRCLE / M_PI);
			}

			const double static degreesToRadians(const double deg)
			{
				return deg * (M_PI / HALF_CIRCLE);
			}
	};

	template<class T> Vec3<T> operator*(const double value, const Vec3<T>& vec3)
	{
		return vec3 * T(value);
	}

	template<class T> Vec3<T> operator/(const double value, const Vec3<T>& vec3)
	{
		return Vec3<T>(T(value / vec3.X()), T(value / vec3.Y()), T(value / vec3.Z()));
	}

	template<class T> const T operator,(const Vec3<T>& vec1, const Vec3<T>& vec2)
	{
		return vec1.dotProduct(vec2);
	}

	template<class T> const Vec3<T> Vec3<T>::zero = Vec3();
	template<class T> const Vec3<T> Vec3<T>::up(0, 1, 0);
	template<class T> const Vec3<T> Vec3<T>::down(0, -1, 0);
	template<class T> const Vec3<T> Vec3<T>::left(-1, 0, 0);
	template<class T> const Vec3<T> Vec3<T>::right(1, 0, 0);
	template<class T> const Vec3<T> Vec3<T>::foward(0, 0, -1);
	template<class T> const Vec3<T> Vec3<T>::backward(0, 0, 1);
	template<class T> const T Vec3<T>::unitVal = 1;

	typedef Vec3<float> vec3;
}