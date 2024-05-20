/**
 * \file hittable.hpp
 **/
#ifndef HITTABLE_HPP
#define HITTABLE_HPP

#include "defines.hpp"
#include "ray.hpp"
#include "interval.hpp"
#include "aabb.hpp"

class Material;

class HitRecord {
  public:
    Point3 point;
    glm::vec3 normal;
    Ref<Material> mat;
    double t;
    double u;
    double v;
    bool front_face;

    void SetFaceNormal(const Ray& r , const glm::vec3& outward_normal);
};

class Hittable {
  public:
    virtual ~Hittable() = default;
    
    virtual bool Hit(const Ray& r , Interval rayt , HitRecord& rec) const = 0;

    virtual Aabb BoundingBox() const = 0;
};

class Translate : public Hittable {
  public:
    Translate(Ref<Hittable> object , const glm::vec3& offset);

    virtual ~Translate() override {}

    virtual bool Hit(const Ray& r , Interval rayt , HitRecord& rec) const override;

    virtual Aabb BoundingBox() const override;

  private:
    Ref<Hittable> object;
    glm::vec3 offset;
    Aabb bbox;
};

class RotateY : public Hittable {
  public:
    RotateY(Ref<Hittable> object , double angle);

    virtual ~RotateY() override {}

    virtual bool Hit(const Ray& r , Interval rayt , HitRecord& rec) const override;

    virtual Aabb BoundingBox() const override;

  private:
    Ref<Hittable> object;
    double angle;

    double cos_theta;
    double sin_theta;

    Aabb bbox;
};

#endif // !HIT_TABLE_HPP
