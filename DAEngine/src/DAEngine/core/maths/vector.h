#pragma once
#include "dastd.h"

namespace da::core::maths {
	namespace impl {
		template <typename T = float>
		T sqrt(T x) {
			T cur{ x }, prev{ 0 };
			while (cur != prev && cur != 0)
			{
				prev = cur;
				cur = (T)0.5f * (cur + x / cur);
			}
			return cur;
		}

		template <typename T = float>
		T abs(T x) {
			return x * (x > 0 ? 1 : -1);
		}

		template <typename T = float>
		T lerp(T a, T b, T amt) {
			return a + amt * (b - a);
		}
	}

	template <typename T>
	union TVector2;
	template <typename T>
	union TVector3;
	template <typename T>
	union TVector4;

	template <typename T>
	union TVector2 {
	public:
		inline TVector2() : x(0.f), y(0.f) {}
		inline TVector2(const T& v) : x(v), y(v) {};
		inline TVector2(const T& x, const T& y) : x(x), y(y) {};
		inline TVector2(const TVector2<T>& other) : x(other.x), y(other.y) {}
		inline TVector2(const TVector3<T>& other) : x(other.x), y(other.y) {}
		inline TVector2(const TVector4<T>& other) : x(other.x), y(other.y) {}
		inline TVector2(const T* ptr) : x(ptr[0]), y(ptr[1]) {};

		struct {
			T x;
			T y;
		};

		struct {
			T r;
			T g;
		};

		struct {
			T s;
			T t;
		};


		inline TVector2<T> operator +(const TVector2<T>& other) const {
			return TVector2<T>(x + other.x, y + other.y);
		}
		inline TVector2<T> operator +(const T& other) const {
			return TVector2<T>(x + other, y + other);
		}
		inline TVector2<T> operator -(const TVector2<T>& other) const {
			return TVector2<T>(x - other.x, y - other.y);
		}
		inline TVector2<T> operator -(const T& other) const {
			return TVector2<T>(x - other, y - other);
		}
		inline TVector2<T> operator -() const {
			return TVector2<T>(-x, -y);
		}
		inline TVector2<T> operator *(const TVector2<T>& other) const {
			return TVector2<T>(x * other.x, y * other.y);
		}
		inline TVector2<T> operator *(const T& other) const {
			return TVector2<T>(x * other, y * other);
		}
		inline TVector2<T> operator /(const TVector2<T>& other) const {
			return TVector2<T>(x / other.x, y / other.y);
		}
		inline TVector2<T> operator /(const T& other) const {
			return TVector2<T>(x / other, y / other);
		}
		inline bool operator ==(const TVector2<T>& other) const {
			return other.x == x && other.y == y;
		}
		inline bool operator !=(const TVector2<T>& other) const {
			return !(other == *this);
		}
		inline  TVector2<T>& operator +=(const T& v) {
			x += v;
			y += v;
			return (*this);
		}
		inline  TVector2<T>& operator +=(const TVector2<T>& other) {
			x += other.x;
			y += other.y;
			return (*this);
		}
		inline  TVector2<T>& operator -=(const T& v) {
			x -= v;
			y -= v;
			return (*this);
		}
		inline  TVector2<T>& operator -=(const TVector2<T>& other) {
			x -= other.x;
			y -= other.y;
			return (*this);
		}
		inline  TVector2<T>& operator *=(const TVector2<T>& other) {
			x *= other.x;
			y *= other.y;
			return (*this);
		}
		inline  TVector2<T>& operator *=(const T& v) {
			x *= v;
			y *= v;
			return (*this);
		}
		inline  TVector2<T>& operator /=(const T& v) {
			x /= v;
			y /= v;
			return (*this);
		}
		inline  TVector2<T>& operator /=(const TVector2<T>& other) {
			x /= other.x;
			y /= other.y;
			return (*this);
		}
		inline  TVector2<T>& operator ++() {
			x++;
			y++;
			return (*this);
		}
		inline  TVector2<T>& operator ++(int) {
			x++;
			y++;
			return (*this);
		}
		inline  TVector2<T>& operator --() {
			x--;
			y--;
			return (*this);
		}
		inline  TVector2<T>& operator --(int) {
			x--;
			y--;
			return (*this);
		}
		inline T length() const { return sqrt<T>(x * x + y * y); }
		inline T size() const { return length(); }
		inline T magnitude() const { return length(); }
		inline TVector2<T> normalize() const { return (*this) / length(); }
		static inline T dot(const TVector2<T>& a, const TVector2<T>& b) { return a.x * b.x + a.y * b.y; }
		static inline TVector2<T> project(const TVector2<T>& a, const TVector2<T>& b) { return (b * (TVector2<T>::dot(a, b) / TVector2<T>::dot(b, b))); }
		static inline TVector2<T> reject(const TVector2<T>& a, const TVector2<T>& b) { return (a - b * (TVector2<T>::dot(a, b) / TVector2<T>::dot(b, b))); }
		static inline TVector2<T> reflect(const TVector2<T>& v, const TVector2<T>& n) { return v - ((TVector2<T>::dot(v, n)) + (TVector2<T>::dot(v, n))) * n; }
		static inline T distance(const TVector2<T>& a, const TVector2<T>& b) { return impl::sqrt<T>(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y))); };
		static inline TVector2<T> lerp(const TVector2<T>& a, const TVector2<T>& b, T amt) { return TVector2<T>(maths::impl::lerp<T>(a.x, b.x, amt), maths::impl::lerp<T>(a.y, b.y, amt)); }
		inline const T* data() const { return &x; }
	};

	template <typename T>
	union TVector3 {
	public:
		inline TVector3() : x(0.f), y(0.f), z(0.f) {}
		inline TVector3(const T& v) : x(v), y(v), z(v) {};
		inline TVector3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
		inline TVector3(const TVector3<T>& other) : x(other.x), y(other.y), z(other.z) {}
		inline TVector3(const TVector4<T>& other) : x(other.x), y(other.y), z(other.z) {}
		inline TVector3(const T* ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2]) {};
		inline TVector3(const TVector2<T>& v, const T& z) : x(v.x), y(v.y), z(z) {};
		inline TVector3(const T& x, const TVector2<T>& v) : x(x), y(v.x), z(v.y) {};

		struct {
			T x;
			T y;
			T z;
		};

		struct {
			T r;
			T g;
			T b;
		};

		struct {
			T s;
			T t;
			T p;
		};



		inline TVector3<T> operator +(const TVector3<T>& other) const {
			return TVector3<T>(x + other.x, y + other.y, z + other.z);
		}
		inline TVector3<T> operator +(const TVector2<T>& other) const {
			return TVector3<T>(x + other.x, y + other.y, z);
		}
		inline TVector3<T> operator +(const T& other) const {
			return TVector3<T>(x + other, y + other, z + other);
		}
		inline TVector3<T> operator -(const TVector3<T>& other) const {
			return TVector3<T>(x - other.x, y - other.y, z - other.z);
		}
		inline TVector3<T> operator -(const TVector2<T>& other) const {
			return TVector3<T>(x - other.x, y - other.y, z);
		}
		inline TVector3<T> operator -(const T& other) const {
			return TVector3<T>(x - other, y - other, z - other);
		}
		inline TVector3<T> operator -() const {
			return TVector3<T>(-x, -y, -z);
		}
		inline TVector3<T> operator *(const TVector3<T>& other) const {
			return TVector3<T>(x * other.x, y * other.y, z * other.z);
		}
		inline TVector3<T> operator *(const TVector2<T>& other) const {
			return TVector3<T>(x * other.x, y * other.y, z);
		}
		inline TVector3<T> operator *(const T& other) const {
			return TVector3<T>(x * other, y * other, z * other);
		}
		inline TVector3<T> operator /(const TVector3<T>& other) const {
			return TVector3<T>(x / other.x, y / other.y, z / other.z);
		}
		inline TVector3<T> operator /(const TVector2<T>& other) const {
			return TVector3<T>(x / other.x, y / other.y, z);
		}
		inline TVector3<T> operator /(const T& other) const {
			return TVector3<T>(x / other, y / other, z / other);
		}
		inline bool operator ==(const TVector3<T>& other) const {
			return other.x == x && other.y == y && other.z == z;
		}
		inline bool operator !=(const TVector3<T>& other) const {
			return !(other == *this);
		}
		inline  TVector3<T>& operator +=(const T& v) {
			x += v;
			y += v;
			z += v;
			return (*this);
		}
		inline  TVector3<T>& operator +=(const TVector3<T>& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return (*this);
		}
		inline  TVector3<T>& operator +=(const TVector2<T>& other) {
			x += other.x;
			y += other.y;
			return (*this);
		}
		inline  TVector3<T>& operator -=(const T& v) {
			x -= v;
			y -= v;
			z -= v;
			return (*this);
		}
		inline  TVector3<T>& operator -=(const TVector3<T>& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return (*this);
		}
		inline  TVector3<T>& operator -=(const TVector2<T>& other) {
			x -= other.x;
			y -= other.y;
			return (*this);
		}
		inline  TVector3<T>& operator *=(const TVector3<T>& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return (*this);
		}
		inline  TVector3<T>& operator *=(const TVector2<T>& other) {
			x *= other.x;
			y *= other.y;
			return (*this);
		}
		inline TVector3<T>& operator *=(const T& v) {
			x *= v;
			y *= v;
			z *= v;
			return (*this);
		}
		inline  TVector3<T>& operator /=(const T& v) {
			x /= v;
			y /= v;
			z /= v;
			return (*this);
		}
		inline  TVector3<T>& operator /=(const TVector3<T>& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return (*this);
		}
		inline TVector3<T>& operator /=(const TVector2<T>& other) {
			x /= other.x;
			y /= other.y;
			return (*this);
		}
		inline  TVector3<T>& operator ++() {
			x++;
			y++;
			z++;
			return (*this);
		}
		inline  TVector3<T>& operator ++(int) {
			x++;
			y++;
			z++;
			return (*this);
		}
		inline  TVector3<T>& operator --() {
			x--;
			y--;
			z--;
			return (*this);
		}
		inline  TVector3<T>& operator --(int) {
			x--;
			y--;
			z--;
			return (*this);
		}
		inline T length() const { return sqrt<T>(x * x + y * y + z * z); }
		inline T size() const { return length(); }
		inline T magnitude() const { return length(); }
		inline TVector3<T> normalize() const { return (*this) / length(); }
		static inline T dot(const TVector3<T>& a, const TVector3<T>& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
		static inline TVector3<T> cross(const TVector3<T> a, const TVector3<T> b) { return { a.y * b.z - a.z * b.y,a.z * b.x - a.x * b.z,a.x * b.y - a.y * b.x }; };
		static inline TVector3<T> project(const TVector3<T>& a, const TVector3<T>& b) { return (b * (TVector3<T>::dot(a, b) / TVector3<T>::dot(b, b))); }
		static inline TVector3<T> reject(const TVector3<T>& a, const TVector3<T>& b) { return (a - b * (TVector3<T>::dot(a, b) / TVector3<T>::dot(b, b))); }
		static inline TVector3<T> reflect(const TVector3<T>& v, const TVector3<T>& n) { return v - ((TVector3<T>::dot(v, n)) + (TVector3<T>::dot(v, n))) * n; }
		static inline T distance(const TVector3<T>& a, const TVector3<T>& b) { return impl::sqrt<T>(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z))); };
		static inline TVector3<T> lerp(const TVector3<T>& a, const TVector3<T>& b, T amt) { return TVector3<T>(maths::impl::lerp<T>(a.x, b.x, amt), maths::impl::lerp<T>(a.y, b.y, amt), maths::impl::lerp<T>(a.z, b.z, amt)); }
		inline TVector2<T> xy() const { return { x,y }; }
		inline const T* data() const { return &x; }
	};

	template <typename T>
	union TVector4 {
	public:
		inline TVector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
		inline TVector4(const T& v) : x(v), y(v), z(v), w(v) {};
		inline TVector4(const TVector2<T>& v, const T& z, const T& w) : x(v.x), y(v.y), z(z), w(w) {};
		inline TVector4(const T& x, const T& y, const TVector2<T>& v) : x(x), y(y), z(v.x), w(v.y) {};
		inline TVector4(const T& x, const TVector2<T>& v, const T& w) : x(x), y(v.x), z(v.y), w(w) {};
		inline TVector4(const TVector3<T>& v, const T& w) : x(v.x), y(v.y), z(v.z), w(w) {};
		inline TVector4(const T& x, const TVector3<T>& v) : x(x), y(v.x), z(v.y), w(v.z) {};
		inline TVector4(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {};

		inline TVector4(const TVector4<T>& other) : x(other.x), y(other.y), z(other.z), w(other.w) {}
		inline TVector4(const T* ptr) : x(ptr[0]), y(ptr[1]), z(ptr[2]), w(ptr[3]) {};

		struct {
			T x;
			T y;
			T z;
			T w;
		};

		struct {
			T r;
			T g;
			T b;
			T a;
		};

		struct {
			T s;
			T t;
			T p;
			T q;
		};

		inline TVector4<T> operator +(const TVector4<T>& other) const {
			return TVector4<T>(x + other.x, y + other.y, z + other.z, w + other.w);
		}
		inline TVector4<T> operator +(const TVector2<T>& other) const {
			return TVector4<T>(x + other.x, y + other.y, z, w);
		}
		inline TVector4<T> operator +(const TVector3<T>& other) const {
			return TVector4<T>(x + other.x, y + other.y, other.z + z, w);
		}
		inline TVector4<T> operator +(const T& other) const {
			return TVector4<T>(x + other, y + other, z + other, w + other);
		}
		inline TVector4<T> operator -(const TVector4<T>& other) const {
			return TVector4<T>(x - other.x, y - other.y, z - other.z, w - other.w);
		}
		inline TVector4<T> operator -(const TVector2<T>& other) const {
			return TVector4<T>(x - other.x, y - other.y, z, w);
		}
		inline TVector4<T> operator -(const TVector3<T>& other) const {
			return TVector4<T>(x - other.x, y - other.y, z - other.z, w);
		}
		inline TVector4<T> operator -(const T& other) const {
			return TVector4<T>(x - other, y - other, z - other, w - other);
		}
		inline TVector4<T> operator -() const {
			return TVector4<T>(-x, -y, -z, -w);
		}
		inline TVector4<T> operator *(const TVector4<T>& other) const {
			return TVector4<T>(x * other.x, y * other.y, z * other.z, w * other.w);
		}
		inline TVector4<T> operator *(const TVector2<T>& other) const {
			return TVector4<T>(x * other.x, y * other.y, z, w);
		}
		inline TVector4<T> operator *(const TVector3<T>& other) const {
			return TVector4<T>(x * other.x, y * other.y, z * other.z, w);
		}
		inline TVector4<T> operator *(const T& other) const {
			return TVector4<T>(x * other, y * other, z * other, w * other);
		}
		inline TVector4<T> operator /(const TVector4<T>& other) const {
			return TVector4<T>(x / other.x, y / other.y, z / other.z, w / other.w);
		}
		inline TVector4<T> operator /(const TVector2<T>& other) const {
			return TVector4<T>(x / other.x, y / other.y, z, w);
		}
		inline TVector4<T> operator /(const TVector3<T>& other) const {
			return TVector4<T>(x / other.x, y / other.y, z / other.z, w);
		}
		inline TVector4<T> operator /(const T& other) const {
			return TVector4<T>(x / other, y / other, z / other, w / other);
		}
		inline bool operator ==(const TVector4<T>& other) const {
			return other.x == x && other.y == y && other.z == z && other.w == w;
		}
		inline bool operator !=(const TVector4<T>& other) const {
			return !(other == *this);
		}
		inline TVector4<T>& operator +=(const T& v) {
			x += v;
			y += v;
			z += v;
			w += v;
			return (*this);
		}
		inline TVector4<T>& operator +=(const TVector4<T>& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			w += other.w;
			return (*this);
		}
		inline TVector4<T>& operator +=(const TVector2<T>& other) {
			x += other.x;
			y += other.y;
			return (*this);
		}
		inline TVector4<T>& operator +=(const TVector3<T>& other) {
			x += other.x;
			y += other.y;
			z += other.z;
			return (*this);
		}
		inline TVector4<T>& operator -=(const T& v) {
			x -= v;
			y -= v;
			z -= v;
			w -= v;
			return (*this);
		}
		inline TVector4<T>& operator -=(const TVector4<T>& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			w -= other.w;
			return (*this);
		}
		inline TVector4<T>& operator -=(const TVector2<T>& other) {
			x -= other.x;
			y -= other.y;
			return (*this);
		}
		inline TVector4<T>& operator -=(const TVector3<T>& other) {
			x -= other.x;
			y -= other.y;
			z -= other.z;
			return (*this);
		}
		inline TVector4<T>& operator *=(const TVector4<T>& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			w *= other.w;
			return (*this);
		}
		inline TVector4<T>& operator *=(const TVector2<T>& other) {
			x *= other.x;
			y *= other.y;
			return (*this);
		}
		inline TVector4<T>& operator *=(const TVector3<T>& other) {
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return (*this);
		}
		inline TVector4<T>& operator *=(const T& v) {
			x *= v;
			y *= v;
			z *= v;
			w *= v;
			return (*this);
		}
		inline  TVector4<T>& operator /=(const T& v) {
			x /= v;
			y /= v;
			z /= v;
			w /= v;
			return (*this);
		}
		inline  TVector4<T>& operator /=(const TVector4<T>& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			w / other.w;
			return (*this);
		}
		inline TVector4<T>& operator /=(const TVector2<T>& other) {
			x /= other.x;
			y /= other.y;
			return (*this);
		}
		inline TVector4<T>& operator /=(const TVector3<T>& other) {
			x /= other.x;
			y /= other.y;
			z /= other.z;
			return (*this);
		}
		inline  TVector4<T>& operator ++() {
			x++;
			y++;
			z++;
			w++;
			return (*this);
		}
		inline  TVector4<T>& operator ++(int) {
			x++;
			y++;
			z++;
			w++;
			return (*this);
		}
		inline  TVector4<T>& operator --() {
			x--;
			y--;
			z--;
			w--;
			return (*this);
		}
		inline  TVector4<T>& operator --(int) {
			x--;
			y--;
			z--;
			w--;
			return (*this);
		}
		inline T length() const { return sqrt<T>(x * x + y * y + z * z + w * w); }
		inline T size() const { return length(); }
		inline T magnitude() const { return length(); }
		inline TVector4<T> normalize() const { return (*this) / length(); }
		static inline T dot(const TVector4<T>& a, const TVector4<T>& b) { return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w; }
		static inline TVector4<T> reflect(const TVector4<T>& v, const TVector4<T>& n) { return v - ((TVector4<T>::dot(v, n)) + (TVector4<T>::dot(v, n))) * n; }
		static inline T distance(const TVector4<T>& a, const TVector4<T>& b) { return impl::sqrt<T>(((a.x - b.x) * (a.x - b.x)) + ((a.y - b.y) * (a.y - b.y)) + ((a.z - b.z) * (a.z - b.z)) + ((a.w - b.w) * (a.w - b.w))); };
		static inline TVector4<T> lerp(const TVector4<T>& a, const TVector4<T>& b, T amt) { return TVector4<T>(maths::impl::lerp<T>(a.x, b.x, amt), maths::impl::lerp<T>(a.y, b.y, amt), maths::impl::lerp<T>(a.z, b.z, amt), maths::impl::lerp<T>(a.w, b.w, amt)); }
		static inline TVector4<T> project(const TVector4<T>& a, const TVector4<T>& b) { return (b * (TVector4<T>::dot(a, b) / TVector4<T>::dot(b, b))); }
		static inline TVector4<T> reject(const TVector4<T>& a, const TVector4<T>& b) { return (a - b * (TVector4<T>::dot(a, b) / TVector4<T>::dot(b, b))); }
		inline TVector2<T> xy() const { return { x,y }; }
		inline TVector3<T> xyz() const { return { x,y,z }; }
		inline const T* data() const { return &x; }
	};

	template<typename T = float>
	inline TVector4<T> operator *(const TVector4<T>& lhs, const T& rhs) {
		return TVector4<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs);
	}
	template<typename T = float>
	inline TVector4<T> operator *(const T& lhs, const TVector4<T>& rhs) {
		return TVector4<T>(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs, rhs.w * lhs);
	}
	template<typename T = float>
	inline TVector3<T> operator *(const TVector3<T>& lhs, const T& rhs) {
		return TVector3<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs);
	}
	template<typename T = float>
	inline TVector3<T> operator *(const T& lhs, const TVector3<T>& rhs) {
		return TVector3<T>(rhs.x * lhs, rhs.y * lhs, rhs.z * lhs);
	}

	template<typename T = float>
	inline TVector2<T> operator *(const TVector2<T>& lhs, const T& rhs) {
		return TVector2<T>(lhs.x * rhs, lhs.y * rhs);
	}
	template<typename T = float>
	inline TVector2<T> operator *(const T& lhs, const TVector2<T>& rhs) {
		return TVector2<T>(rhs.x * lhs, rhs.y * lhs);
	}

	template<typename T = float>
	inline TVector4<T> operator +(const TVector4<T>& lhs, const T& rhs) {
		return TVector4<T>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
	}
	template<typename T = float>
	inline TVector4<T> operator +(const T& lhs, const TVector4<T>& rhs) {
		return TVector4<T>(rhs.x + lhs, rhs.y + lhs, rhs.z + lhs, rhs.w + lhs);
	}
	template<typename T = float>
	inline TVector3<T> operator +(const TVector3<T>& lhs, const T& rhs) {
		return TVector3<T>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
	}
	template<typename T = float>
	inline TVector3<T> operator +(const T& lhs, const TVector3<T>& rhs) {
		return TVector3<T>(rhs.x + lhs, rhs.y + lhs, rhs.z + lhs);
	}

	template<typename T = float>
	inline TVector2<T> operator +(const TVector2<T>& lhs, const T& rhs) {
		return TVector2<T>(lhs.x + rhs, lhs.y + rhs);
	}
	template<typename T = float>
	inline TVector2<T> operator +(const T& lhs, const TVector2<T>& rhs) {
		return TVector2<T>(rhs.x + lhs, rhs.y + lhs);
	}
};

namespace da
{
	using Vector2f = da::core::maths::TVector2<float>;
	using Vector3f = da::core::maths::TVector3<float>;
	using Vector4f = da::core::maths::TVector4<float>;
	using Vector4u8 = da::core::maths::TVector4<uint8_t>;
};
