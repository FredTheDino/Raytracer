#include <iostream>

struct Hit {
    Point3 point;
    Vec3 normal;
    double t;
    bool front_face;
    bool valid;

    void normal_from_intersection(const Ray &ray, const Vec3 &outward) {
        front_face = dot(ray.direction, outward) < 0;
        normal = front_face ? outward : -outward;
    }

    operator bool() { return valid; }
};

struct Hittable {
    virtual Hit hit(const Ray &ray, double t_min, double t_max) = 0;
};

struct Sphere: public Hittable {
    Point3 center;
    double radius;

    Sphere(const Vec3 c, double r): center(c), radius(r) {};

    Hit hit(const Ray &ray, double t_min, double t_max) override;
};

Hit Sphere::hit(const Ray &ray, double t_min, double t_max) {
#if 1
    Hit result = {};

    Vec3 distance = center - ray.origin;
    double t_to_center = dot(distance, ray.direction);
    Vec3 closest_point = ray.at(t_to_center);
    double p_to_c = (closest_point - center).length_squared();
    double radius_squared = radius * radius;
    double discriminant = radius_squared - p_to_c;

    if (discriminant < 0) return result; // Missed the sphere

    double delta = sqrt(discriminant);
    double potential = t_to_center - delta;

    if (potential < t_min || t_max < potential) {
        potential = t_to_center + delta;

        if (potential < t_min || t_max < potential)
            return result;
    }

    result.valid = true;
    result.t = potential;
    result.point = ray.at(result.t);
    // This is neat!
    Vec3 outward = (result.point - center) / radius;
    result.normal_from_intersection(ray, outward);
    if (!result.front_face) {
        std::cout << "WRONG!" << std::endl;
    }
    return result;
#else
    Vec3 oc = ray.origin - center;
    auto a = ray.direction.length_squared();
    auto half_b = dot(oc, ray.direction);
    auto c = oc.length_squared() - radius*radius;
    auto discriminant = half_b*half_b - a*c;

    Hit result = {};
    if (discriminant > 0) {
        auto root = std::sqrt(discriminant);
        auto temp = (-half_b - root)/a;
        if (temp > t_max && temp < t_min) {
            temp = (-half_b + root) / a;
        }
        result.valid = temp < t_max && temp > t_min;
        result.t = temp;
        result.point = ray.at(result.t);
        Vec3 outward = (result.point - center) / radius;
        result.normal_from_intersection(ray, outward);
    }
    return result;
#endif
};



