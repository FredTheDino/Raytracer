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

struct Camera {
    Point3 origin;
    Point3 lower_left;
    Vec3 horizontal;
    Vec3 vertical;

    Camera(double vfov, double aspect_ratio) {
        origin = V(0, 0, 0);

        double half_height = std::tan(vfov / 2);
        double half_width = aspect_ratio * half_height;

        lower_left = V(-half_width, -half_height, -1);

        horizontal = V(2 * half_width, 0, 0);
        vertical = V(0, 2 * half_height, 0);
    }

    Ray get_ray(double u, double v) const {
        return Ray(origin, lower_left + u * horizontal + v * vertical - origin);
    }
};

