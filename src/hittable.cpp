
struct Hit {
    Point3 point;
    Vec3 normal;
    double t;
    bool valid;

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
    Hit result = {};

    Vec3 distance = center - ray.origin;
    double t_to_center = dot(distance, ray.direction);
    Vec3 closest_point = ray.at(t_to_center);
    double radius_squared = radius * radius;
    double p_to_c = (closest_point - center).length_squared();
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
    result.point = ray.at(potential);
    // This is neat!
    result.normal = (result.point - center) / radius;
    return result;
};


