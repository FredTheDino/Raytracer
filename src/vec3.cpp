#pragma once

#include <cmath>

struct Vec3 {
    union {
        struct {
            double x, y, z;
        };
        struct {
            double r, g, b;
        };
        double _[3];
    };

    Vec3 &operator +=(const Vec3 &v);
    Vec3 &operator -=(const Vec3 &v);

    Vec3 operator -() const;
    double length_squared() const;
    double length() const;
    Vec3 normalized() const;
};


inline std::ostream& operator<<(std::ostream &out, const Vec3 &v) {
    return out << "{" << v.x << ' ' << v.y << ' ' << v.z << "}" << "|" << v.length_squared() << "|";
}

inline Vec3 operator +(const Vec3 &a, const Vec3 &b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline Vec3 operator +=(const Vec3 &a, const Vec3 &b) {
    return { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline Vec3 operator -(const Vec3 &a, const Vec3 &b) {
    return { a.x - b.x, a.y - b.y, a.z - b.z };
}

inline Vec3 operator *(const Vec3 &v, const double s) {
    return { v.x * s, v.y * s, v.z * s };
}

inline Vec3 operator *( const double s, const Vec3 &v) {
    return v * s;
}

inline Vec3 operator /(const Vec3 &v, const double s) {
    return v * (1 / s);
}

Vec3 &Vec3::operator +=(const Vec3 &v) {
    *this = *this + v;
    return *this;
}

Vec3 &Vec3::operator -=(const Vec3 &v) {
    *this = *this + v;
    return *this;
}

Vec3 hadmard(const Vec3 &a, const Vec3 &b) {
    return { a.x * b.x, a.y * b.y, a.z * b.z };
}

double dot(const Vec3 &a, const Vec3 &b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 reflect(const Vec3 a, const Vec3 normal) {
    return a - 2 * dot(a, normal) * normal;
}

Vec3 refract(const Vec3 &a, const Vec3 &normal, double refraction_const) {
    assert(0.9 < a.length_squared() && a.length_squared() < 1.1);
    assert(0.9 < normal.length_squared() && normal.length_squared() < 1.1);
    double cos_theta = -dot(a, normal);
    Vec3 r_parallel = refraction_const * (a + cos_theta * normal);
    Vec3 r_perp = -sqrt(1.0 - r_parallel.length_squared()) * normal;
    return r_parallel + r_perp;
}

Vec3 cross(const Vec3 &a, const Vec3 &b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

Vec3 Vec3::operator -() const { return {-x, -y, -z}; }
Vec3 Vec3::normalized() const { return *this / length(); }
double Vec3::length() const { return std::sqrt(length_squared()); }
double Vec3::length_squared() const { return x * x + y * y + z * z; }

double schlick(double cosine, double index_of_refraction) {
    double r0 = (1 - index_of_refraction) / (1 + index_of_refraction);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow(1 - cosine, 5);
}


using Point3 = Vec3;
using Color = Vec3;

Color C(double r, double g, double b) {
    return {r, g, b};
}

Vec3 V(double x, double y, double z) {
    return {x, y, z};
}

