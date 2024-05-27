/**
 * \file pdf.hpp
 **/
#ifndef PDF_HPP
#define PDF_HPP

#include "onb.hpp"
#include "hittable_list.hpp"

class Pdf {
  public:
    virtual ~Pdf() {}

    virtual double Value(const glm::vec3& direction) const = 0;
    virtual glm::vec3 Generate() const = 0;
};

class SpherePdf : public Pdf {
  public:
    virtual ~SpherePdf() override {}

    virtual double Value(const glm::vec3& direction) const override;
    virtual glm::vec3 Generate() const override;
};

class CosinePdf : public Pdf {
  public:
    CosinePdf(const glm::vec3& k);
    virtual ~CosinePdf() override {}

    virtual double Value(const glm::vec3& direction) const override;
    virtual glm::vec3 Generate() const override;

  private:
    Onb ijk;
};

class HittablePdf : public Pdf {
  public:
    HittablePdf(const Ref<Hittable>& objects , const Point3& origin);
    virtual ~HittablePdf() override {}


    virtual double Value(const glm::vec3& direction) const override;
    virtual glm::vec3 Generate() const override;

  private:
    Ref<Hittable> objects;
    Point3 origin;
};

class MixturePdf : public Pdf {
  public:
    MixturePdf(Ref<Pdf> p0 , Ref<Pdf> p1);
    virtual ~MixturePdf() override {}

    virtual double Value(const glm::vec3& direction) const override;
    virtual glm::vec3 Generate() const override;

  private:
    Ref<Pdf> dists[2];
};

#endif // !PDF_HPP
