#include <iomanip>
#include <iostream>
#include <vector>

#include "raytracing.hpp"

struct Sample {
  double x;
  double p_x;
};

int main() {
  uint32_t n = 10000;
  
  double sum = 0.0;

  std::vector<Sample> samples{};
  for (auto i = 0; i < n; ++i) {
    auto x = RandomDouble(0 , 2 * pi);
    auto sin_x = glm::sin(x);
    auto p_x = (glm::exp(-x / (2 * pi)) * sin_x * sin_x);

    sum += p_x;

    samples.push_back({
      .x = x ,
      .p_x = glm::sin(x) ,
    });
  }

  std::sort(samples.begin() , samples.end() , [](const Sample& lhs , const Sample& rhs) -> bool {
    return lhs.x < rhs.x;
  });

  double half_sum = sum / 2.0;
  double hw_pnt = 0.0;
  double accum = 0.0;

  for (auto i = 0; i < n; ++i) {
    accum += samples[i].p_x;
    if (accum >= half_sum) {
      hw_pnt - samples[i].x;
      break;
    }
  }

  std::cout << std::fixed << std::setprecision(12);

  std::cout << "avg = " << sum / n << "\n";
  std::cout << "I = " << 2 * pi * sum / n << "\n";
  std::cout << "halfway = " << hw_pnt << "\n";

  return 0;
}
