/**
 * \file hittable_list.hpp
 **/
#ifndef HITTABLE_LIST_HPP
#define HITTABLE_LIST_HPP

#include <vector>

#include "defines.hpp"
#include "hittable.hpp"
#include "interval.hpp"
#include "aabb.hpp"

class HittableList : public Hittable {
  public:
    std::vector<Ref<Hittable>> objects{};

    HittableList() {}
    HittableList(Ref<Hittable> obj);

    virtual ~HittableList() override {}

    void Clear();

    void Add(Ref<Hittable> obj);

    virtual bool Hit(const Ray& r , Interval rayt , HitRecord& rec) const override;

    virtual Aabb BoundingBox() const override;

    virtual double PdfValue(const Point3& origin , const glm::vec3& direction) const override;

    virtual glm::vec3 Random(const Point3& origin) const override;

  private:
    Aabb bbox;
};

#endif // !HIT_TABLE_LIST_HPP
