/**
 * \file quad.hpp
 **/
#ifndef QUAD_HPP
#define QUAD_HPP

#include "defines.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include "material.hpp"

class Quad : public Hittable {
  public:
    Quad(const Point3& Q , const glm::vec3& u , const glm::vec3& v , Ref<Material> mat);

    virtual bool Hit(const Ray& r , Interval rayt , HitRecord& rec) const override;

    virtual Aabb BoundingBox() const override;
    
    virtual double PdfValue(const Point3& origin , const glm::vec3& direction) const override;

    virtual glm::vec3 Random(const Point3& origin) const override;

  private:
    double D;
    double area;
    Point3 Q;
    glm::vec3 u , v;
    glm::vec3 w;
    glm::vec3 normal;
    Ref<Material> material;
    Aabb bbox;

    void SetBoundingBox();

    bool IsInterior(double a , double b , HitRecord& rec) const;
};

Ref<HittableList> CreateBox(const Point3& a , const Point3& b , Ref<Material> mat);

#endif // !QUAD_HPP
