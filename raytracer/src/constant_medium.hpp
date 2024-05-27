/**
 * \file constant_medium.hpp
 **/
#ifndef CONSTANT_MEDIUM_HPP
#define CONSTANT_MEDIUM_HPP

#include "defines.hpp"
#include "hittable.hpp"
#include "texture.hpp"
#include "material.hpp"

class ConstantMedium : public Hittable {
  public:
    ConstantMedium(Ref<Hittable> boundary , double density , Ref<Texture> tex)
      : boundary(boundary) , neg_inv_density(-1.0 / density) , 
        phase_function(NewRef<Isotropic>(tex)) {}
    ConstantMedium(Ref<Hittable> boundary , double density , const Color& albedo)
      : boundary(boundary) , neg_inv_density(-1.0 / density) ,
        phase_function(NewRef<Isotropic>(albedo)) {}

    virtual bool Hit(const Ray& r , Interval rayt , HitRecord& rec) const override;

    virtual Aabb BoundingBox() const override;
    
    virtual double PdfValue(const Point3& origin , const glm::vec3& direction) const override;

    virtual glm::vec3 Random(const Point3& origin) const override;

  private:
    Ref<Hittable> boundary;
    double neg_inv_density;
    Ref<Material> phase_function;
};

#endif // !CONSTANT_MEDIUM_HPP
