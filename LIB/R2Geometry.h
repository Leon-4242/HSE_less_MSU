#ifndef GEOMETRY
#define GEOMETRY

#include <iostream>
#include <cmath>
#include <vector>
#include "EXCEPT.h"

#define EPS 1e-12

namespace Geometry {

    template <typename F>
    bool operator==(const F&, const F&);

    using namespace EXCEPT;
    template <typename F>
    class Vector {
        std::vector<F> data;
        size_t num;
        public:

        Vector(const size_t n) {
            if (n == 0) throw Except("not understand");
            num = n;
            data(num, F());
        }

        Vector(const std::vector<F>& value): num(value.length()), data(value) {}

        Vector(std::vector<F>&& value) {
            num = value.length();
            data(value);
            value.clear();
        }

        Vector(const Vector<F>& v) {
            num = v.num;
            data = v.data;
        }

        Vector(Vector<F>&& v) {
            num = v.num; v.num = 0;
            data(v.data); v.data.clear();
        }

        Vector& operator= (const Vector<F>& v) {
            num = v.num;
            data.assign(v.data.begin(), v.data.end());
            return *this;
        }

        Vector& operator= (Vector<F>&& v) {
            num = v.num; v.num = 0;
            data.swap(v.data); v.data.clear();
            return *this;
        }

        ~Vector(void) {}

        Vector operator+ (void) const {
            return Vector<F>(data);
        }

        Vector operator- (void) const {
            auto newdata(data);
            for (auto curr : newdata) curr = F()-curr;
            return Vector<F>(newdata);
        }

        Vector operator+ (const Vector<F>& v) const {
            if (num != v.num) throw Except("not same dimensions");
            auto newdata(data);
            for (size_t i = 0; i < num; ++i) newdata[i] += v.data[i];
            return Vector<F>(newdata);
        }

        Vector& operator+= (const Vector<F>& v) {
            *this = *this + v;
            return *this;
        }

        Vector operator- (const Vector<F>& v) const {
            if (num != v.num) throw Except("not same dimensions");
            auto newdata(data);
            for (size_t i = 0; i < num; ++i) newdata[i] -= v.data[i];
            return Vector<F>(newdata);
        }

        Vector& operator-= (const Vector<F>& v) {
            *this = *this - v;
            return *this;
        }

        Vector operator* (const F& lymbda) const {
            auto newdata(data);
            for (auto curr : newdata) curr *= lymbda;
            return vector<F>(newdata);
        }

        bool operator== (const Vector<F>& v) const {
            if (num != v.num) return false;
            for (size_t i = 0; i < num; ++i) 
                if (data[i] == v.data[i])
                else return false;

        }
    };

    template <typename F>
    Vector<F> operator* (const F& lymbda, const Vector<F>& v) {
        return v*lymbda;
    }
}

#endif
