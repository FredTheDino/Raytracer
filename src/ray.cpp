
struct Ray {
    Point3 origin;
    Vec3 direction;
    bool absorbed;

    static Ray absorb() { return Ray(); }

    Ray(): absorbed(true) {}

    Ray(Point3 origin, Vec3 direction): origin(origin), direction(direction), absorbed(false) {
        normalize();
    }

    void normalize() {
        direction = direction.normalized();
    }

    Point3 at(double t) const {
        return origin + t * direction;
    }
};
