#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <cmath>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class t> struct vec2 {
	union {
		struct {t u, v;};
		struct {t x, y;};
		t raw[2];
	};
	vec2() : u(0), v(0) {}
	vec2(t _u, t _v) : u(_u),v(_v) {}
	inline vec2<t> operator +(const vec2<t> &V) const { return vec2<t>(u+V.u, v+V.v); }
	inline vec2<t> operator -(const vec2<t> &V) const { return vec2<t>(u-V.u, v-V.v); }
	inline vec2<t> operator *(float f)          const { return vec2<t>(u*f, v*f); }
	template <class > friend std::ostream& operator<<(std::ostream& s, vec2<t>& v);
};

template <class t> struct vec3 {
	union {
		struct {t x, y, z;};
		struct {t ivert, iuv, inorm;};
		t raw[3];
	};
	vec3() : x(0), y(0), z(0) {}
	vec3(t _x, t _y, t _z) : x(_x),y(_y),z(_z) {}
	inline vec3<t> operator ^(const vec3<t> &v) const { return vec3<t>(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	inline vec3<t> operator +(const vec3<t> &v) const { return vec3<t>(x+v.x, y+v.y, z+v.z); }
	inline vec3<t> operator -(const vec3<t> &v) const { return vec3<t>(x-v.x, y-v.y, z-v.z); }
	inline vec3<t> operator *(float f)          const { return vec3<t>(x*f, y*f, z*f); }
	inline t       operator *(const vec3<t> &v) const { return x*v.x + y*v.y + z*v.z; }
	float norm () const { return std::sqrt(x*x+y*y+z*z); }
	vec3<t> & normalize(t l=1) { *this = (*this)*(l/norm()); return *this; }
	template <class > friend std::ostream& operator<<(std::ostream& s, vec3<t>& v);
};

typedef vec2<float> vec2f;
typedef vec2<int>   vec2i;
typedef vec3<float> vec3f;
typedef vec3<int>   vec3i;

template <class t> std::ostream& operator<<(std::ostream& s, vec2<t>& v) {
	s << "(" << v.x << ", " << v.y << ")\n";
	return s;
}

template <class t> std::ostream& operator<<(std::ostream& s, vec3<t>& v) {
	s << "(" << v.x << ", " << v.y << ", " << v.z << ")\n";
	return s;
}

#endif //__GEOMETRY_H__
