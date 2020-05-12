#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>

// TODO(ed): I belive the sphere intersection test is wrong...

#include "vec3.cpp"
#include "random.cpp"
#include "ray.cpp"
#include "material.cpp"
#include "hittable.cpp"

#include <vector>
#include <functional>

using HittableList = std::vector<Hittable *>;

// TODO(ed): This could be templated instead, what's the difference?
Hit cast_ray(const HittableList &world, const Ray &ray, double t_min, double t_max) {
    Hit hit = {};
    for (auto object : world) {
        if (Hit ray_hit = object->hit(ray, t_min, t_max)) {
            hit = ray_hit;
            t_max = hit.t;
        }
    }
    return hit;
}

Color ray_color(const Ray &r, const HittableList &world, int depth) {
    if (depth <= 0) return {};
    if (Hit hit = cast_ray(world, r, 0.001, 100.0)) {
        Ray scatter; Color attenuation;
        std::tie(attenuation, scatter) = hit.material->scatter(r, hit);
        if (scatter.absorbed) return {};
        scatter.normalize();
        return hadmard(attenuation, ray_color(scatter, world, depth - 1));
    }

    double t = 0.5 * (r.direction.y + 1.0);
    return (1.0 - t) * C(1, 1, 1) + t * C(0.5, 0.7, 1.0);
}

// TODO(ed): Camera class

int main() {
    const double aspect_ratio = 16.0 / 9.0;
    const int width = 384;
    const int height = width / aspect_ratio;
    const int num_samples = 50;

    std::ofstream output("render.ppm");
    output << "P3" << std::endl << width << " " << height << std::endl;
    output << "255" << std::endl;

    Lambertian red_mat = {C(0.7, 0.2, 0.1)};
    Lambertian blue_mat = {C(0.2, 0.6, 0.1)};
    Metal metal = {C(0.8, 0.6, 0.2), 0.5};
    Dielectric glass = {1.5};
    Sphere small = {V(0, 0, -1), 0.5, (Material *) &red_mat};
    Sphere large = {V(0, -100.5, -1), 100, (Material *) &blue_mat};
    Sphere metal_one = {V(-1, 0, -1), 0.5, (Material *) &metal};
    Sphere glass_one = {V(1, 0, -1), -0.5, (Material *) &glass};
    HittableList world = {&small, &large, &metal_one, &glass_one};

    Camera camera(M_PI * 0.4, aspect_ratio);
    for (int y = height - 1; y >= 0; --y) {
        std::cerr << "Progress: " << std::setprecision(3) << double(y) / height << "\r";
        for (int x = width - 1; x >= 0; --x) {
            Color color = {};
            for (int sample = 0; sample < num_samples; sample++) {
                double u = (x + random_real()) / double(width - 1);
                double v = (y + random_real()) / double(height - 1);
                color += ray_color(camera.get_ray(u, v), world, 50);
            }
            write(output, color / double(num_samples));
        }
    }
    output.close();
}
