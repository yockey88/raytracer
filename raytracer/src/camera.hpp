/**
 * \file camera.hpp
 **/
#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <string>
#include <vector>
#include <mutex>
#include <thread>

#include "defines.hpp"
#include "ray.hpp"
#include "hittable.hpp"

class Camera {
  public:
    double aspect_ratio = 1.0;
    double img_width = 100;

    double defocus_angle = 0;
    double focus_dist = 10;

    /// vfov = vertical field of view
    double vfov = 90;

    Point3 camera_loc = Point3(0 , 0 , 0);
    Point3 target = Point3(0 , 0 , -1);

    /// relative up
    glm::vec3 vup = glm::vec3(0 , 1 , 0);

    Color background;

    uint32_t samples_per_pixel = 10;
    int32_t max_depth = 10;

    std::string img_file = "image.ppm";

    void Render(const Hittable& world); 

  private:
    uint32_t sqrt_spp = 0;

    double img_height;
    double pixel_samples_scale;
    double recip_sqrt_spp;

    Point3 center;
    Point3 pixel00_loc;

    glm::vec3 pixel_del_u;
    glm::vec3 pixel_del_v;

    /// camera space basis (i , j , k)
    glm::vec3 u , v , w;

    glm::vec3 defocus_disk_u;
    glm::vec3 defocus_disk_v;

    std::mutex pixel_mtx;
    std::vector<Color> final_pixels{};
    std::vector<std::thread> workers{};

    void Initialize();

    Ray GetRay(uint32_t i , uint32_t j , uint32_t s_i , uint32_t s_j) const;

    glm::vec3 SampleSquare() const;
    
    glm::vec3 SampleSquareStratified(uint32_t s_i , uint32_t s_j) const;

    glm::vec3 SampleDisk(double radius) const;

    Point3 DefocusDiskSample() const;

    Color RayColor(const Ray& r , int32_t depth , const Hittable& world) const;

    void WriteToFile();
};

#endif // !CAMERA_HPP
