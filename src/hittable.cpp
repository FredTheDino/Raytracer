#include <iostream>
#include "hittable.h"

struct Sphere: public Hittable {
    Point3 center;
    double radius;
    Material *material;

    Sphere(const Vec3 c, double r, Material *material): center(c), radius(r), material(material) {}

    Hit hit(const Ray &ray, double t_min, double t_max) override;
};

Hit Sphere::hit(const Ray &ray, double t_min, double t_max) {
    // TODO(ed): Work through the math
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
    result.material = material;
    return result;
#else
    Vec3 oc = ray.origin - center;
    double a = ray.direction.length_squared();
    double half_b = dot(oc, ray.direction);
    double c = oc.length_squared() - radius * radius;
    double discriminant = half_b * half_b - a * c;
    if (discriminant > 0) {
        double root = sqrt(discriminant);
        double temp = (-half_b - root)/a;
        if (temp < t_max && temp > t_min) {
            Hit result;
            result.valid = true;
            result.t = temp;
            result.point = ray.at(result.t);
            Vec3 outward_normal = (result.point - center) / radius;
            result.normal_from_intersection(ray, outward_normal);
            result.material = material;
            return result;
        }
        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            Hit result;
            result.valid = true;
            result.t = temp;
            result.point = ray.at(result.t);
            Vec3 outward_normal = (result.point - center) / radius;
            result.normal_from_intersection(ray, outward_normal);
            result.material = material;
            return result;
        }
    }
    return {};
#endif
};
