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

    double lens_radius;
    Vec3 w, u, v;

    Camera(Point3 lookfrom, Point3 lookat, Vec3 vup, double vfov, double aspect_ratio, double aperture, double focus_dist) {
        origin = lookfrom;
        lens_radius = aperture / 2;

        double half_height = std::tan(vfov / 2);
        double half_width = aspect_ratio * half_height;

        w = (lookfrom - lookat).normalized();
        u = cross(vup, w).normalized();
        v = cross(w, u);

        lower_left = origin
                - half_width * focus_dist * u
                - half_height * focus_dist * v
                - focus_dist * w;

        horizontal = 2 * focus_dist * half_width * u;
        vertical = 2 * focus_dist * half_height * v;
    }

    Ray get_ray(double s, double t) const {
        Vec3 source = lens_radius * random_unit_disk();
        Vec3 offset = u * source.x + v * source.y;
        return Ray(origin + offset, lower_left + s * horizontal + t * vertical - offset - origin);
    }
};

