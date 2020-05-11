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
};
