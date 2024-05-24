#include <chrono>
#include <iostream>

#include "constant_medium.hpp"
#include "defines.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "sphere.hpp"
#include "quad.hpp"
#include "camera.hpp"
#include "material.hpp"
#include "bvh.hpp"
#include "texture.hpp"

void BouncingSpheres(const std::string& file_name) {
  HittableList world;

  auto checker = NewRef<CheckerTexture>(0.32 , Color(0.2 , 0.3 , 0.1) , Color(0.9 , 0.9 , 0.9));
  auto ground_mat = NewRef<Lambertian>(checker);
  world.Add(NewRef<Sphere>(Point3(0 , -1000 , 0) , 1000 , ground_mat));

  for (auto a = -11; a < 11; ++a) {
    for (auto b = -11; b < 11; ++b) {
      auto choose_mat = RandomDouble();

      Point3 center(a + 0.9 * RandomDouble() , 0.2 , b + 0.9 * RandomDouble());

      if ((center - Point3(4 , 0.2 , 0)).length() > 0.9) {
        Ref<Material> sphere_mat;

        if (choose_mat < 0.8) {
          /// diffuse
          auto albedo = RandomVec3() * RandomVec3();
          sphere_mat = NewRef<Lambertian>(albedo);
          auto center2 = center + glm::vec3(0 , RandomDouble(0 , 0.5) , 0);
          world.Add(NewRef<Sphere>(center , center2 , 0.2 , sphere_mat));
        } else if (choose_mat < 0.95) {
          /// metal 
          auto albedo = RandomVec3(0.5 , 1);
          auto fuzz = RandomDouble(0 , 0.5);
          sphere_mat = NewRef<Metal>(albedo , fuzz);
          world.Add(NewRef<Sphere>(center , 0.2 , sphere_mat));
        } else {
          /// glass 
          sphere_mat = NewRef<Dielectric>(1.5);
          world.Add(NewRef<Sphere>(center , 0.2 , sphere_mat));
        }
      }
    }
  }

  auto mat1 = NewRef<Dielectric>(1.5);
  world.Add(NewRef<Sphere>(Point3(0 , 1 , 0) , 1.0 , mat1));

  auto mat2 = NewRef<Lambertian>(Color(0.4 , 0.2 , 0.1));
  world.Add(NewRef<Sphere>(Point3(-4 , 1 , 0) , 1.0 , mat2));

  auto mat3 = NewRef<Metal>(Color(0.7 , 0.6 , 0.5) , 0.0);
  world.Add(NewRef<Sphere>(Point3(4 , 1 , 0) , 1.0 , mat3));

  world = HittableList(NewRef<BvhNode>(world));

  Camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.img_width = 1200;
  cam.samples_per_pixel = 500;
  cam.max_depth = 50;
  cam.background = Color(0.70 , 0.80 , 1.00);

  cam.vfov = 20;
  cam.camera_loc = Point3(13 , 2 , 3);
  cam.target = Point3(0 , 0 , 0);
  cam.vup = glm::vec3(0 , 1 , 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;

  cam.img_file = file_name;

  /// Rendering
  cam.Render(world);
}

void CheckeredSpheres(const std::string& file_name) {
  HittableList world;
  
  auto checker = NewRef<CheckerTexture>(0.32 , Color(0.2 , 0.3 , 0.1) , Color(0.9 , 0.9 , 0.9));
  auto checker_mat = NewRef<Lambertian>(checker);

  world.Add(NewRef<Sphere>(Point3(0 , 10 , 0) , 10 , checker_mat));
  world.Add(NewRef<Sphere>(Point3(0 , -10 , 0) , 10 , checker_mat));

  Camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.img_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = Color(0.70 , 0.80 , 1.00);

  cam.vfov = 20;
  cam.camera_loc = Point3(12 , 2 , 3);
  cam.target = Point3(0 , 0 , 0);
  cam.vup = glm::vec3(0 , 1 , 0);

  cam.defocus_angle = 0;

  cam.img_file = file_name;

  cam.Render(world);
}

void Earth(const std::string& file_name) {
  auto earth_tex = NewRef<ImageTexture>("images/earthmap.jpg");
  auto earth_mat = NewRef<Lambertian>(earth_tex);
  auto globe = NewRef<Sphere>(Point3(0 , 0 , 0) , 2 , earth_mat);

  Camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.img_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = Color(0.70 , 0.80 , 1.00);

  cam.vfov = 20;
  cam.camera_loc = Point3(0 , 0 , 12);
  cam.target = Point3(0 , 0 , 0);
  cam.vup = glm::vec3(0 , 1 , 0);

  cam.defocus_angle = 0;

  cam.img_file = file_name;

  cam.Render(HittableList(globe));
}

void PerlinSpheres(const std::string& file_name , double scale) {
  HittableList world;

  auto pertext = NewRef<NoiseTexture>(scale);
  world.Add(NewRef<Sphere>(Point3(0 , -1000 , 0) , 1000 , NewRef<Lambertian>(pertext)));
  world.Add(NewRef<Sphere>(Point3(0 , 2 , 0) , 2 , NewRef<Lambertian>(pertext)));

  Camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.img_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = Color(0.70 , 0.80 , 1.00);

  cam.vfov = 20;
  cam.camera_loc = Point3(13 , 2 , 3);
  cam.target = Point3(0 , 0 , 0);
  cam.vup = glm::vec3(0 , 1 , 0);

  cam.defocus_angle = 0;

  cam.img_file = file_name;

  cam.Render(world);
}

void Quads(const std::string& file_name) {
  HittableList world;

  auto left_red = NewRef<Lambertian>(Color(1.0 , 0.2 , 0.2));
  auto back_green = NewRef<Lambertian>(Color(0.2 , 1.0 , 0.2));
  auto right_blue = NewRef<Lambertian>(Color(0.2 , 0.2 , 1.0));
  auto upper_orange = NewRef<Lambertian>(Color(1.0 , 0.5 , 0.0));
  auto lower_teal = NewRef<Lambertian>(Color(0.2 , 0.8 , 0.8));

  world.Add(NewRef<Quad>(Point3(-3 , -2 , 5) , glm::vec3(0 , 0 , -4) , glm::vec3(0 , 4 , 0) , left_red));
  world.Add(NewRef<Quad>(Point3(-2 , -2 , 0) , glm::vec3(4 , 0 , 0) , glm::vec3(0 , 4 , 0) , back_green));
  world.Add(NewRef<Quad>(Point3(3 , -2 , 1) , glm::vec3(0 , 0 , 4) , glm::vec3(0 , 4 , 0) , right_blue));
  world.Add(NewRef<Quad>(Point3(-2 , 3 , 1) , glm::vec3(4 , 0 , 0) , glm::vec3(0 , 0 , 4) , upper_orange));
  world.Add(NewRef<Quad>(Point3(-2 , -3 , 5) , glm::vec3(4 , 0 , 0) , glm::vec3(0 , 0 , -4) , lower_teal));

  Camera cam;

  cam.aspect_ratio = 1.0;
  cam.img_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = Color(0.70 , 0.80 , 1.00);

  cam.vfov = 80;
  cam.camera_loc = Point3(0 , 0 , 9);
  cam.target = Point3(0 , 0 , 0);
  cam.vup = glm::vec3(0 , 1 , 0);

  cam.defocus_angle = 0;

  cam.img_file = file_name;

  cam.Render(world);
}

void SimpleLight(const std::string& file_name , double scale) {
  HittableList world;

  auto pertext = NewRef<NoiseTexture>(scale);
  world.Add(NewRef<Sphere>(Point3(0 , -1000 , 0) , 1000 , NewRef<Lambertian>(pertext)));
  world.Add(NewRef<Sphere>(Point3(0 , 2 , 0) , 2 , NewRef<Lambertian>(pertext)));

  auto difflight = NewRef<DiffuseLight>(Color(4 , 4 , 4));
  world.Add(NewRef<Sphere>(Point3(0 , 7 , 0) , 2 , difflight));
  world.Add(NewRef<Quad>(Point3(3 , 1 , -2) , glm::vec3(2 , 0 , 0) , glm::vec3(0 , 2 , 0) , difflight));

  Camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.img_width = 1200;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background = Color(0.0 , 0.0 , 0.0);

  cam.vfov = 20;
  cam.camera_loc = Point3(26 , 3 , 6);
  cam.target = Point3(0 , 2 , 0);
  cam.vup = glm::vec3(0 , 1 , 0);

  cam.defocus_angle = 0;

  cam.img_file = file_name;

  cam.Render(world);
}

void CornellBox(const std::string& file_name) {
  HittableList world;

  auto red = NewRef<Lambertian>(Color(0.65 , 0.05 , 0.05));
  auto white = NewRef<Lambertian>(Color(0.73 , 0.73 , 0.73));
  auto green = NewRef<Lambertian>(Color(0.12 , 0.45 , 0.15));
  auto light = NewRef<DiffuseLight>(Color(15 , 15 , 15));

  world.Add(NewRef<Quad>(Point3(555 , 0 , 0) , glm::vec3(0 , 555 , 0) , glm::vec3(0 , 0 , 555) , green));
  world.Add(NewRef<Quad>(Point3(0 , 0 , 0) , glm::vec3(0 , 555 , 0) , glm::vec3(0 , 0 , 555) , red));
  world.Add(NewRef<Quad>(Point3(343 , 554 , 332) , glm::vec3(-130 , 0 , 0) , glm::vec3(0 , 0 , -105) , light));
  world.Add(NewRef<Quad>(Point3(0 , 0 , 0) , glm::vec3(555 , 0 , 0) , glm::vec3(0 , 0 , 555) , white));
  world.Add(NewRef<Quad>(Point3(555 , 555 , 555) , glm::vec3(-555 , 0 , 0) , glm::vec3(0 , 0 , -555) , white));
  world.Add(NewRef<Quad>(Point3(0 , 0 , 555) , glm::vec3(555 , 0 , 0) , glm::vec3(0 , 555 , 0) , white));

  Ref<Hittable> box1 = CreateBox(Point3(0 , 0 , 0) , Point3(165 , 330 , 165) , white);
  box1 = NewRef<RotateY>(box1 , 15);
  box1 = NewRef<Translate>(box1 , glm::vec3(265 , 0 , 295));
  world.Add(box1);

  Ref<Hittable> box2 = CreateBox(Point3(0 , 0 , 0) , Point3(165 , 165 , 165) , white);
  box2 = NewRef<RotateY>(box2 , -18);
  box2 = NewRef<Translate>(box2 , glm::vec3(130 , 0 , 65));
  world.Add(box2);

  Camera cam;

  cam.aspect_ratio = 1.0;
  cam.img_width = 600;
  cam.samples_per_pixel = 200;
  cam.max_depth = 50;
  cam.background = Color(0 , 0 , 0);

  cam.vfov = 40;
  cam.camera_loc = Point3(278 , 278 , -800);
  cam.target = Point3(278 , 278 , 0);
  cam.vup = glm::vec3(0 , 1 , 0);

  cam.defocus_angle = 0;

  cam.img_file = file_name;

  cam.Render(world);
}

void CornellSmoke(const std::string& file_name) {
  HittableList world;

  auto red = NewRef<Lambertian>(Color(0.65 , 0.05 , 0.05));
  auto white = NewRef<Lambertian>(Color(0.73 , 0.73 , 0.73));
  auto green = NewRef<Lambertian>(Color(0.12 , 0.45 , 0.15));
  auto light = NewRef<DiffuseLight>(Color(7 , 7 , 7));

  world.Add(NewRef<Quad>(Point3(555 , 0 , 0) , glm::vec3(0 , 555 , 0) , glm::vec3(0 , 0 , 555) , green));
  world.Add(NewRef<Quad>(Point3(0 , 0 , 0) , glm::vec3(0 , 555 , 0) , glm::vec3(0 , 0 , 555) , red));
  world.Add(NewRef<Quad>(Point3(113 , 554 , 127) , glm::vec3(330 , 0 , 0) , glm::vec3(0 , 0 , 305) , light));
  world.Add(NewRef<Quad>(Point3(0 , 0 , 0) , glm::vec3(555 , 0 , 0) , glm::vec3(0 , 0 , 555) , white));
  world.Add(NewRef<Quad>(Point3(555 , 555 , 555) , glm::vec3(-555 , 0 , 0) , glm::vec3(0 , 0 , -555) , white));
  world.Add(NewRef<Quad>(Point3(0 , 0 , 555) , glm::vec3(555 , 0 , 0) , glm::vec3(0 , 555 , 0) , white));

  Ref<Hittable> box1 = CreateBox(Point3(0 , 0 , 0) , Point3(165 , 330 , 165) , white);
  box1 = NewRef<RotateY>(box1 , 15);
  box1 = NewRef<Translate>(box1 , glm::vec3(265 , 0 , 295));

  Ref<Hittable> box2 = CreateBox(Point3(0 , 0 , 0) , Point3(165 , 165 , 165) , white);
  box2 = NewRef<RotateY>(box2 , -18);
  box2 = NewRef<Translate>(box2 , glm::vec3(130 , 0 , 65));

  world.Add(NewRef<ConstantMedium>(box1 , 0.01 , Color(0 , 0 , 0)));
  world.Add(NewRef<ConstantMedium>(box2 , 0.01 , Color(1 , 1 , 1)));

  Camera cam;

  cam.aspect_ratio = 1.0;
  cam.img_width = 600;
  cam.samples_per_pixel = 500;
  cam.max_depth = 50;
  cam.background = Color(0 , 0 , 0);

  cam.vfov = 40;
  cam.camera_loc = Point3(278 , 278 , -800);
  cam.target = Point3(278 , 278 , 0);
  cam.vup = glm::vec3(0 , 1 , 0);

  cam.defocus_angle = 0;

  cam.img_file = file_name;

  cam.Render(world);
}

void FinalScene(const std::string& file_name , int img_width , int samples_per_pixel , int max_depth) {
  HittableList boxes1;

  auto ground = NewRef<Lambertian>(Color(0.48 , 0.83 , 0.53));

  int32_t boxes_per_side = 20;
  for (auto i = 0; i < boxes_per_side; ++i) {
    for (auto j = 0; j < boxes_per_side; ++j) {
      auto w = 100.0;
      auto x0 = -1000.0 + i*w;
      auto z0 = -1000.0 + j*w;
      auto y0 = 0.0;
      auto x1 = x0 + w;
      auto y1 = RandomDouble(1,101);
      auto z1 = z0 + w;

      boxes1.Add(CreateBox(Point3(x0 , y0 , z0) , Point3(x1 , y1 , z1) , ground));
    }
  }

  HittableList world;

  world.Add(NewRef<BvhNode>(boxes1));

  auto light = NewRef<DiffuseLight>(Color(7 , 7 , 7));
  world.Add(NewRef<Quad>(Point3(123 , 554 , 147) , glm::vec3(300 , 0 , 0) , glm::vec3(0 , 0 , 265) , light));

  auto center1 = Point3(400, 400, 200);
  auto center2 = center1 + glm::vec3(30,0,0);
  auto sphere_material = NewRef<Lambertian>(Color(0.7, 0.3, 0.1));
  world.Add(NewRef<Sphere>(center1, center2, 50, sphere_material));

  world.Add(NewRef<Sphere>(Point3(260 , 150 , 45) , 50 , NewRef<Dielectric>(1.5)));
  world.Add(NewRef<Sphere>(Point3(0 , 150 , 145) , 50 , NewRef<Metal>(Color(0.8 , 0.8 , 0.9) , 1.0)));

  auto boundary = NewRef<Sphere>(Point3(360 , 150 , 145) , 70 , NewRef<Dielectric>(1.5));
  world.Add(boundary);
  world.Add(NewRef<ConstantMedium>(boundary , 0.2 , Color(0.2 , 0.4 , 0.9)));
  
  boundary = NewRef<Sphere>(Point3(0 , 0 , 0) , 1000 , NewRef<Dielectric>(1.5));
  world.Add(boundary);

  auto emat = NewRef<Lambertian>(NewRef<ImageTexture>("images/earthmap.jpg"));
  world.Add(NewRef<Sphere>(Point3(400 , 200 , 400) , 100 , emat));

  auto pertext = NewRef<NoiseTexture>(0.2);
  world.Add(NewRef<Sphere>(Point3(220 , 280 , 300) , 80 , NewRef<Lambertian>(pertext)));

  HittableList boxes2;
  auto white = NewRef<Lambertian>(Color(0.73 , 0.73 , 0.73));
  int32_t ns = 1000;
  for (int32_t i = 0; i < ns; ++i) {
    boxes2.Add(NewRef<Sphere>(RandomVec3(0 , 165) , 10 , white));
  }

  world.Add(NewRef<Translate>(NewRef<RotateY>(NewRef<BvhNode>(boxes2) , 15) , glm::vec3(-100 , 270 , 395)));

  Camera cam;

  cam.aspect_ratio = 1.0;
  cam.img_width = img_width;
  cam.samples_per_pixel = samples_per_pixel;
  cam.max_depth = max_depth;
  cam.background = Color(0 , 0 , 0);

  cam.vfov = 40;
  cam.camera_loc = Point3(478 , 278 , -600);
  cam.target = Point3(278 , 278 , 0);
  cam.vup = glm::vec3(0 , 1 , 0);

  cam.defocus_angle = 0;

  cam.Render(world);
}

void MonteCarlo() {
  int32_t sample = 0;
  int32_t stratified_sample = 0;

  int32_t sqrt_n = 10000;

  auto before = std::chrono::system_clock::now();
  for (auto i = 0u; i < sqrt_n; ++i) {
    for (auto j = 0u; j < sqrt_n; ++j) {
      auto x = RandomDouble(-1 , 1);
      auto y = RandomDouble(-1 , 1);

      if (x * x + y * y < 1) {
        sample++;
      }

      x = 2 * ((i + RandomDouble()) / sqrt_n) - 1;
      y = 2 * ((j + RandomDouble()) / sqrt_n) - 1;

      if (x * x + y * y < 1) {
        stratified_sample++;
      }
    }
  }
  auto after = std::chrono::system_clock::now();
  auto dur = after - before;

  std::cout << "Monte Carlo time : " << dur.count() << "\n\n";
  std::cout << std::fixed << std::setprecision(12);

  std::cout << "Pi (no stratification) = " << (4.0 * sample) / (sqrt_n * sqrt_n) << "\n";
  std::cout << "Pi (stratification) = " << (4.0 * stratified_sample) / (sqrt_n * sqrt_n) << "\n";
}

int main() {
  uint32_t example = 9;

  switch (example) {
    case 0: BouncingSpheres("bouncing_spheres.ppm"); break;
    case 1: CheckeredSpheres("checkered_spheres1.ppm"); break;
    case 2: Earth("earth1.ppm"); break;
    case 3: PerlinSpheres("perlin_spheres4.ppm" , 4); break;
    case 4: Quads("quads1.ppm"); break;
    case 5: SimpleLight("simple_light3.ppm" , 4); break;
    case 6: CornellBox("cornell_box2.ppm"); break;
    case 7: CornellSmoke("cornell_smoke.ppm"); break;
    case 8: FinalScene("the_next_week.ppm" , 800 , 1000 , 50); break;
    case 9: MonteCarlo(); break;
    default:
      std::cout << "INVALID EXAMPLE" << std::endl;
  }

  return 0;
}
