#include <cstdlib>
#include <random>
#include <cstdint>

thread_local struct XORWOWState {
    uint32_t a, b, c, d;
    uint32_t counter;
} random_state;

void random_seed(uint32_t seed) {
    // Generated random numbers from random.org.
    //
    // This makes it deterministic, which you might
    // not want.
    random_state.a = 398973565 ^ seed;
    random_state.b = 141307319 ^ seed;
    random_state.c =  15804145 ^ seed;
    random_state.d = 296129167 ^ seed;
    random_state.counter = 100;
}

uint32_t xorwow() {
    // Algorithm "xorwow" from p. 5 of Marsaglia, "Xorshift RNGs"
    // Stolen straight from Wikipedia.
    uint32_t t = random_state.d;

    uint32_t s = random_state.a;
    random_state.d = random_state.c;
    random_state.c = random_state.b;
    random_state.b = s;

    t ^= t >> 2;
    t ^= t << 1;
    t ^= s ^ (s << 4);
    random_state.a = t;

    random_state.counter += 362437;
    return t + random_state.counter;
}

double random_real() {
    return double(xorwow()) / 0xFFFFFFFF;
}

double random_real(double min, double max) {
    return min + (max - min) * random_real();
}

Vec3 random_in_sphere() {
    Vec3 vec;
    do {
        vec = {random_real(-1, 1), random_real(-1, 1), random_real(-1, 1)};
    } while (vec.length_squared() <= 1.0);
    return vec;
}

Vec3 random_in_hemisphere(Vec3 normal) {
    Vec3 vec = random_in_sphere();
    if (dot(normal, vec) < 0)
        return -vec;
    return vec;
}

Vec3 random_unit_disk() {
    double a = random_real(0, 2 * M_PI);
    return { cos(a), sin(a), 0 };
}

Vec3 random_unit() {
    double a = random_real(0, 2 * M_PI);
    double z = random_real(-1, 1);
    double r = sqrt(1 - z * z);
    return { r * cos(a) , r * sin(a), z };
}


double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (max < x) return max;
    return x;
}

void write(std::ostream &out, Color pixel) {
    // assert(0 <= pixel.r && pixel.r <= 1.0);
    // assert(0 <= pixel.g && pixel.g <= 1.0);
    // assert(0 <= pixel.b && pixel.b <= 1.0);

    auto cc = [](double x) { return clamp(std::sqrt(x), 0, 0.9999); };

    int ir = static_cast<int>(cc(pixel.r) * 256);
    int ig = static_cast<int>(cc(pixel.g) * 256);
    int ib = static_cast<int>(cc(pixel.b) * 256);

    out << ir << " " << ig <<  " " << ib << std::endl;
}

