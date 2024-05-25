/**
 * \file camera.cpp
 **/
#include "camera.hpp"

#include <chrono>
#include <iostream>
#include <fstream>
#include <latch>

#include "defines.hpp"
#include "material.hpp"

inline double LinearToGamma(double linear_component) {
  if (linear_component > 0) {
    return glm::sqrt(linear_component);
  }

  return 0;
}

inline void WriteColor(std::ostream& stream , const Color& color) {
  auto r = color.x;
  auto g = color.y;
  auto b = color.z;

  r = LinearToGamma(r);
  g = LinearToGamma(g);
  b = LinearToGamma(b);

  static const Interval intensity(0.000 , 0.999);
  uint32_t ir = uint32_t(256 * intensity.Clamp(r));
  uint32_t ig = uint32_t(256 * intensity.Clamp(g));
  uint32_t ib = uint32_t(256 * intensity.Clamp(b));

  stream << ir << ' ' << ig << ' ' << ib << '\n';
}

void Camera::Render(const Hittable& world) {
  Initialize();
  
  std::cout << "generating pixel data...\n";

  std::chrono::time_point<std::chrono::steady_clock> before , after;
  before = std::chrono::steady_clock::now();

  std::latch latch(img_width * img_height);

  auto pixel_func = [this , &world , &latch](size_t i , size_t j) {
    Color pixel_col(0 , 0 , 0);
    for (auto s_j = 0; s_j < sqrt_spp; ++s_j) {
      for (auto s_i = 0; s_i < sqrt_spp; ++s_i) {
        Ray r = GetRay(i , j , s_i , s_j);
        pixel_col += RayColor(r , max_depth , world);
      }
    }

    size_t idx = i + j * img_width;

    {
      std::unique_lock lck(pixel_mtx);
      final_pixels[idx] = pixel_col;
    }

    latch.count_down();
  };

  std::cout << "spawning workers...\n";
  for (size_t j = 0; j < img_height; ++j) {
    for (size_t i = 0; i < img_width; ++i) {
      workers.emplace_back(pixel_func , i , j);
    }
  }
  std::cout << "...workers spawned\n\n";

  std::cout << "waiting for workers...\n";
  latch.wait();
  std::cout << "...tracing complete\n\n";

  for (auto& worker : workers) {
    worker.join();
  }
  workers.clear();

  after = std::chrono::steady_clock::now();
  
  std::cout << "...pixel data generated\n\n";

  std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
  std::cout << "Ray-Cast time : [" << duration.count() << "]ms\n\n";

  WriteToFile();
}

void Camera::Initialize() {
  img_height = int32_t(img_width / aspect_ratio);
  img_height = (img_height < 1) ? 
    1 : img_height;

  pixel_samples_scale = 1.0 / samples_per_pixel;

  sqrt_spp = glm::sqrt(samples_per_pixel);
  recip_sqrt_spp = 1.0 / sqrt_spp;

  center = camera_loc;

  auto h = glm::tan(DegreesToRadians(vfov) / 2);
  
  auto viewport_h = 2 * h * focus_dist;
  auto viewport_w = viewport_h * (double(img_width) / img_height);

  w = UnitVector(camera_loc - target);
  u = UnitVector(glm::cross(vup , w));
  v = glm::cross(w , u);

  /// calculate the vectors across viewport edges
  auto viewport_u = viewport_w * u;
  auto viewport_v = viewport_h * -v;

  /// calculate horizontal and vertical delta vectors from pixel to pixel
  pixel_del_u = viewport_u / img_width;
  pixel_del_v = viewport_v / img_height;

  /// position of upper left pixel
  auto viewport_ul = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
  pixel00_loc = viewport_ul + 0.5 * (pixel_del_u + pixel_del_v);

  /// calculate camera defocus disk basis
  auto defocus_radius = focus_dist * glm::tan(DegreesToRadians(defocus_angle / 2));
  defocus_disk_u = u * defocus_radius;
  defocus_disk_v = v * defocus_radius;

  final_pixels.resize(img_width * img_height);
}

Ray Camera::GetRay(uint32_t i , uint32_t j , uint32_t s_i , uint32_t s_j) const {
  auto offset = SampleSquareStratified(s_i , s_j);
  auto pixel_sample = pixel00_loc + 
                      ((i + offset.x) * pixel_del_u) +
                      ((j + offset.y) * pixel_del_v);

  auto ray_origin = (defocus_angle <= 0) ?
    center : DefocusDiskSample();
  auto ray_direction = pixel_sample - ray_origin;

  auto ray_time = RandomDouble();

  return Ray(ray_origin , ray_direction , ray_time);
}
    
glm::vec3 Camera::SampleSquare() const {
  return glm::vec3(RandomDouble() - 0.5 , RandomDouble() - 0.5 , 0);
}
    
glm::vec3 Camera::SampleSquareStratified(uint32_t s_i , uint32_t s_j) const {
  auto px = ((s_i + RandomDouble()) * recip_sqrt_spp) - 0.5;
  auto py = ((s_j + RandomDouble()) * recip_sqrt_spp) - 0.5;

  return glm::vec3(px , py , 0);
}
    
glm::vec3 Camera::SampleDisk(double radius) const {
  return radius * RandomVec3InUnitDisk();
}

Point3 Camera::DefocusDiskSample() const {
  auto p = RandomVec3InUnitDisk();
  return center + (p.x * defocus_disk_u) + (p.y * defocus_disk_v);
}
    
Color Camera::RayColor(const Ray& r , int32_t depth , const Hittable& world) const {
  if (depth <= 0) {
    return Color(0 , 0 , 0);
  }

  HitRecord rec;

  if (!world.Hit(r , Interval(0.001 , infinity) , rec)) {
    return background;
  }

  Ray scattered;
  Color attenuation;
  Color color_from_emission = rec.mat == nullptr ? 
    Color(0.0 , 0.0 , 0.0) : rec.mat->Emitted(rec.u , rec.v , rec.point);

  if (rec.mat != nullptr && !rec.mat->Scatter(r , rec , attenuation , scattered)) {
    return color_from_emission;
  }

  Color color_from_scatter = attenuation * RayColor(scattered , depth - 1 , world);

  return color_from_emission + color_from_scatter;
}

void Camera::WriteToFile() {
  std::cout << "writing to image...\n";
  
  std::chrono::time_point<std::chrono::steady_clock> before = std::chrono::steady_clock::now();

  std::ofstream img("images/" + img_file);
  if (!img.is_open()) {
    std::cout << "file not open" << std::endl;
    return;
  }

  img << "P3\n" << img_width << ' ' << img_height << "\n255\n";

  for (auto& pixel : final_pixels) {
    WriteColor(img , pixel_samples_scale * pixel);
  }

  std::chrono::time_point<std::chrono::steady_clock> after = std::chrono::steady_clock::now();

  std::cout << "...image complete" << std::endl;
  
  std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
  std::cout << "Image-Write time : [" << duration.count() << "]ms\n\n";
}
