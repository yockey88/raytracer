/**
 * \file raytracing_ex.cpp
 **/
#include "raytracing.hpp"

#include <iostream>

int main() {
  uint32_t example = 6;

  switch (example) {
    case 0: BouncingSpheres("bouncing_spheres.ppm"); break;
    case 1: CheckeredSpheres("checkered_spheres1.ppm"); break;
    case 2: Earth("earth1.ppm"); break;
    case 3: PerlinSpheres("perlin_spheres4.ppm" , 4); break;
    case 4: Quads("quads1.ppm"); break;
    case 5: SimpleLight("simple_light3.ppm" , 4); break;
    case 6: CornellBox("cornell_box16.ppm"); break;
    case 7: CornellSmoke("cornell_smoke.ppm"); break;
    case 8: FinalScene("the_next_week.ppm" , 800 , 1000 , 50); break;
    case 9: Stratification("stratification.ppm"); break;
    default:
      std::cout << "INVALID EXAMPLE" << std::endl;
  }

  return 0;
}

