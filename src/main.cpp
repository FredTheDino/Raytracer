#include <iostream>
#include <fstream>
#include <cassert>

#include "vec3.cpp"
#include "ray.cpp"
#include "hittable.cpp"

void write(std::ostream &out, Color pixel) {
    assert(0 <= pixel.r && pixel.r <= 1.0);
    assert(0 <= pixel.g && pixel.g <= 1.0);
    assert(0 <= pixel.b && pixel.b <= 1.0);

    int ir = static_cast<int>(pixel.r * 255.999);
    int ig = static_cast<int>(pixel.g * 255.999);
    int ib = static_cast<int>(pixel.b * 255.999);

    out << ir << " " << ig <<  " " << ib << std::endl;
}

Color ray_color(const Ray &r) {
    {
        Sphere sphere = Sphere(V(0, 0, -1), 0.5);
        Hit hit = sphere.hit(r, 0, 100);
        if (hit) {
            std::cout << hit.normal << std::endl;
            return 0.5 * (hit.normal + V(1, 1, 1));
        }
    }
    Vec3 unit_dir = r.direction.normalized();
    double t = 0.5 * (unit_dir.y + 1.0);
    return (1.0 - t) * C(1, 1, 1) + t * C(0.5, 0.7, 1.0);
}

int main() {
    const double aspect_ratio = 16.0 / 9.0;
    const int width = 384;
    const int height = width / aspect_ratio;
    Color image[width][height];

    Point3 origo = {};
    Vec3 horizontal = {4, 0, 0};
    Vec3 vertical = {0, 2.25, 0};
    Vec3 lower_left = origo - horizontal / 2 - vertical / 2 - V(0, 0, 1);
    for (int x = width - 1; x >= 0; --x) {
        for (int y = 0; y < height; ++y) {
            auto u = double(x) / double(width - 1);
            auto v = double(y) / double(height - 1);
            Ray r = {origo, lower_left + u * horizontal + v * vertical};
            image[x][y] = ray_color(r);
        }
    }

    std::ofstream output("render.ppm");
    output << "P3" << std::endl << width << " " << height << std::endl;
    output << "255" << std::endl;
    for (int y = height - 1; y >= 0; --y) {
        for (int x = width - 1; x >= 0; --x) {
            write(output, image[x][y]);
        }
    }
    output.close();
}
