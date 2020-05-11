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
    return out << "{" << v.x << ' ' << v.y << ' ' << v.z << "}";
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

Vec3 cross(const Vec3 &a, const Vec3 &b) {
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

Vec3 Vec3::operator -() const { return {-x, -y, -z}; }
Vec3 Vec3::normalized() const { return *this / length(); }
double Vec3::length() const { return std::sqrt(length_squared()); }
double Vec3::length_squared() const { return x * x + y * y + z * z; }

using Point3 = Vec3;
using Color = Vec3;

Color C(double r, double g, double b) {
    return {r, g, b};
}

Vec3 V(double x, double y, double z) {
    return {x, y, z};
}

