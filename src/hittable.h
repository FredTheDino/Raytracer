#pragma once

struct Material;

struct Hit {
    Point3 point;
    Vec3 normal;
    Material *material;
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

