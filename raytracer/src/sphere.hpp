/**
 * \file sphere.hpp
 **/
#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "defines.hpp"
#include "hittable.hpp"

class Sphere : public Hittable {
  public:
    Sphere(const Point3& center , double radius , Ref<Material> material);
    
    Sphere(const Point3& center1 , const Point3& center2 , double radius , Ref<Material> material);

    virtual bool Hit(const Ray& r , Interval rayt , HitRecord& rec) const override;

    virtual Aabb BoundingBox() const override;
    
    virtual double PdfValue(const Point3& origin , const glm::vec3& direction) const override;

    virtual glm::vec3 Random(const Point3& origin) const override;

  private:
    Point3 center1;
    double radius;
    Ref<Material> material;

    bool is_moving;
    glm::vec3 center_vec;

    Aabb bbox;

    Point3 SphereCenter(double time) const;

    static void GetSphereUV(const Point3& p , double& u , double& v);
};

#endif // !SPHERE_HPP
