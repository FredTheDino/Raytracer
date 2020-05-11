
struct Ray {
    Point3 origin;
    Vec3 direction;
    bool absorbed;

    static Ray absorb() { return { {}, {}, true }; };

    Point3 at(double t) const {
        return origin + t * direction;
    }
};
