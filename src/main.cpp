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
#include <thread>

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

const double aspect_ratio = 16.0 / 9.0;
const int width = 384 * 2;
const int height = width / aspect_ratio;
const int num_samples = 200;
const int num_threads = 4;

volatile int finished;

void raytrace(int t, const Camera &camera, const HittableList &world, Color **buffer, int start, int end) {
    random_seed(0xCAFEBABE + t * t);

    for (int y = start; y < end; ++y) {
        if (t == 0)
            std::cerr << std::setprecision(3) << double(finished) / height << "\r";
        for (int x = width - 1; x >= 0; --x) {
            Color color = {};
            for (int sample = 0; sample < num_samples; sample++) {
                double u = (x + random_real()) / double(width - 1);
                double v = (y + random_real()) / double(height - 1);
                color += ray_color(camera.get_ray(u, v), world, 50);
            }
            buffer[y][x] = color;
        }
        finished++;
    }
}

int main() {

    Color **buffer = new Color *[height];
    for (int i = 0; i < height; i++) {
        buffer[i] = new Color[width];
    }

    Lambertian red_mat = {C(0.7, 0.2, 0.1)};
    Lambertian blue_mat = {C(0.4, 0.4, 0.4)};
    Metal metal = {C(0.8, 0.6, 0.2), 0.5};
    Dielectric glass = {1.5};
    Sphere small = {V(0, 0, -1), 0.5, (Material *) &red_mat};
    Sphere large = {V(0, -100.5, -1), 100, (Material *) &blue_mat};
    Sphere metal_one = {V(-1, 0, -1), 0.5, (Material *) &metal};
    Sphere glass_one = {V(1, 0, -1), -0.5, (Material *) &glass};
    HittableList world = {&small, &large, &metal_one, &glass_one};

    Vec3 from = V(-2, 2, 1);
    Vec3 target = V(0, 0, -1);

    Camera camera(from, target, V(0, 1, 0), M_PI * 0.3, aspect_ratio, 0.01, (from - target).length());

    int start = 0;
    std::thread threads[num_threads];
    std::cerr << "Starting" << std::endl;
    for (int t = 0; t < num_threads; t++) {
        int work_for_thread = (height / num_threads) + ((height % num_threads) > t);
        int end = start + work_for_thread;
        std::cerr << start << ", " << end << std::endl;
        threads[t] = std::thread(raytrace, t, camera, world, buffer, start, end);
        start = end;
    }

    for (int t = 0; t < num_threads; t++) {
        threads[t].join();
    }

    std::ofstream output("render.ppm");
    output << "P3" << std::endl << width << " " << height << std::endl;
    output << "255" << std::endl;
    for (int y = height - 1; y >= 0; --y) {
        for (int x = width - 1; x >= 0; --x) {
            write(output, buffer[y][x] / double(num_samples));
        }
    }
    output.close();
}
