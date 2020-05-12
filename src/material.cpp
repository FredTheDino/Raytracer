#include "hittable.h"

struct Material {
    virtual std::pair<Color, Ray> scatter(const Ray &ray, const Hit &hit) const = 0;
};

struct Lambertian: public Material {
    Color albedo;

    Lambertian(Color albedo): albedo(albedo) {}

    virtual std::pair<Color, Ray> scatter(const Ray &ray, const Hit &hit) const {
        Vec3 scatter_direction = random_in_hemisphere(hit.normal);
        Ray scatter = {hit.point, scatter_direction};
        return std::make_pair(albedo, scatter);
    }
};

struct Metal: public Material {
    Color albedo;
    double fuzz;

    Metal(Color albedo, double f): albedo(albedo), fuzz(clamp(f, 0, 1)) {}

    virtual std::pair<Color, Ray> scatter(const Ray &ray, const Hit &hit) const {
        Vec3 reflection = reflect(ray.direction, hit.normal);
        Vec3 fuzzed = fuzz * random_unit();
        Ray scatter = Ray(hit.point, reflection + fuzzed);
        scatter.absorbed = dot(scatter.direction, hit.normal) < 0;
        return std::make_pair(albedo, scatter);
    }
};

struct Dielectric: public Material {
    double index_of_refraction;

    Dielectric(double ior): index_of_refraction(ior) {};

    virtual std::pair<Color, Ray> scatter(const Ray &ray, const Hit &hit) const {
        Vec3 reflection = reflect(ray.direction, hit.normal);
        double n = hit.front_face ? 1.0 / index_of_refraction : index_of_refraction;
        Ray scatter = Ray(hit.point, refract(ray.direction, hit.normal, n));
        scatter.absorbed = false;
        return std::make_pair(C(1.0, 1.0, 1.0), scatter);
    }
};
