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
        Color attenuation = albedo;
        return std::make_pair(albedo, scatter);
    }
};

