/**
 * \file interval.hpp
 **/
#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include "defines.hpp"

class Interval {
  public:
    double min , max;

    Interval() 
      : min(-infinity) , max(infinity) {}

    Interval(double min , double max) 
      : min(min) , max(max) {}

    Interval(const Interval& a , const Interval& b);

    double Size() const;

    bool ClosedContains(double x) const;

    bool OpenContains(double x) const;

    double Clamp(double x) const;

    Interval Expand(double delta) const;

    static const Interval empty , universe;
};

Interval operator+(const Interval& ival , double displacement);
Interval operator+(double displacement , const Interval& ival);

#endif // !INTERVAL_HPP

