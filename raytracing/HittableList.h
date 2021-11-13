#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"

#include <memory>
#include <vector>

class HittableList: public Hittable {
    public:
        std::vector<std::shared_ptr<Hittable>> objects;
        
        HittableList(){}
        HittableList(std::shared_ptr<Hittable> obj) {
            objects.push_back(obj);
        }
        
        void clear() {
            objects.clear();
        }
        
        void add(std::shared_ptr<Hittable> obj) {
            objects.push_back(obj);
        }
        
        virtual bool hit(
            const Ray& r, 
            double t_min, 
            double t_max, 
            HitRecord& rec
        ) const override;
};

bool HittableList::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
    HitRecord temp_rec;
    bool hit_anything = false;
    auto curr_closest = t_max;
    
    for(const auto& obj : objects) {
        if(obj->hit(r, t_min, curr_closest, temp_rec)) {
            curr_closest = temp_rec.t;
            rec = temp_rec; // reassigning rec!
            hit_anything = true;
        }
    }
    
    return hit_anything;
}

#endif