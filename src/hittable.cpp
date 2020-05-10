
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

    Vec3 oc = ray.origin - center;
    double a = ray.direction.length_squared();
    double half_b = dot(oc, ray.direction);
    double c = oc.length_squared() - radius * radius;
    double disciminant = half_b * half_b - a * c;
    result.valid = disciminant >= 0;
    if (!result) return result;
    double root = (-half_b - std::sqrt(disciminant)) / (a);
    double temp = (-half_b - root) / a;
    if (temp >= t_max || temp <= t_min) {
        temp = (-half_b + root) / a;
    }
    std::cout << temp << std::endl;
    result.t = temp;
    result.point = ray.at(result.t);
    result.normal = (result.point - center).normalized();
    if (result.normal.length() > 1.0) {
        std::cout << "LONGER!" << result.normal.length() << std::endl;
    }
    return result;

};


