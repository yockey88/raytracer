/**
 * \file bvh.hpp
 **/
#ifndef BVH_HPP
#define BVH_HPP

#include "defines.hpp"
#include "aabb.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"

class BvhNode : public Hittable {
  public:
    BvhNode(HittableList list) 
      : BvhNode(list.objects , 0 , list.objects.size()) {}

    BvhNode(std::vector<Ref<Hittable>>& objects , size_t start , size_t end);

    virtual bool Hit(const Ray& r , Interval rayt , HitRecord& rec) const override;

    virtual Aabb BoundingBox() const override;

    static bool BoxCompare(const Ref<Hittable> a , const Ref<Hittable> b , int axis_index);
    static bool BoxXCompare(const Ref<Hittable> a , const Ref<Hittable> b);
    static bool BoxYCompare(const Ref<Hittable> a , const Ref<Hittable> b);
    static bool BoxZCompare(const Ref<Hittable> a , const Ref<Hittable> b);

  private:
    Ref<Hittable> left;
    Ref<Hittable> right;
    Aabb bbox;
};

#endif // !BVH_HPP
