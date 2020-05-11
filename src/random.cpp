#include <cstdlib>

double random_real() {
    return rand() / (RAND_MAX + 1.0);
}

double random_real(double min, double max) {
    return min + (max - min) * random_real();
}

Vec3 random_in_sphere() {
    Vec3 vec;
    do {
        vec = {random_real(), random_real(), random_real()};
    } while (vec.length_squared() <= 1.0);
    return vec;
}

Vec3 random_in_hemisphere(Vec3 normal) {
    Vec3 vec;
    do {
        vec = {random_real(), random_real(), random_real()};
    } while (vec.length_squared() <= 1.0);
    if (dot(normal, vec) < 0)
        return -vec;
    return vec;
}

Vec3 random_unit() {
    double a = random_real(0, 2 * M_PI);
    double z = random_real(-1, 1);
    double r = sqrt(1 - r * r);
    return { r * cos(a) , r * sin(a), z };
}


double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (max < x) return max;
    return x;
}

void write(std::ostream &out, Color pixel) {
    assert(0 <= pixel.r && pixel.r <= 1.0);
    assert(0 <= pixel.g && pixel.g <= 1.0);
    assert(0 <= pixel.b && pixel.b <= 1.0);

    auto cc = [](double x) { return clamp(std::sqrt(x), 0, 0.9999); };

    int ir = static_cast<int>(cc(pixel.r) * 256);
    int ig = static_cast<int>(cc(pixel.g) * 256);
    int ib = static_cast<int>(cc(pixel.b) * 256);

    out << ir << " " << ig <<  " " << ib << std::endl;
}

