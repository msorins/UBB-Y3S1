#include "Sphere.hpp"

using namespace rt;
using namespace std;
Intersection Sphere::getIntersection(const Line& line, float minDist, float maxDist) {
    Vector v1 = line.dx();
    Vector v2 = line.vec(maxDist);
    Vector v3 = this->_center;
    float r = this->_radius;

    float a = v2.x() * v2.x()  - 2 * v2.x() * v1.x() + v1.x() * v1.x() +
              v2.y() * v2.y()  - 2 * v2.y() * v1.y() + v1.y() * v1.y() +
              v2.z() * v2.z()  - 2 * v2.z() * v1.z() + v1.z() * v1.z();

    float b = 2;
    b *= (  v2.x()*v1.x() - v2.x()*v3.x() - v1.x()*v1.x() + v1.x() * v3.x() +
            v2.y()*v1.y() - v2.y()*v3.y() - v1.y()*v1.y() + v1.y() * v3.y() +
            v2.z()*v1.z() - v2.z()*v3.z() - v1.z()*v1.z() + v1.z() * v3.z()
            );


    float c = v3.x() * v3.x() + v3.y()*v3.y() + v3.z()*v3.z() +
              v1.x() * v1.x() + v1.y()*v1.y() + v1.z()*v1.z() -
              2* (v3.x() * v1.x() + v3.y()*v1.y() + v3.z()*v1.z()) -
              r * r;

    float delta = b*b - 4 * a * c;
    if(delta < 0) { // no intersection
        Intersection in;
        return in;
    } else if(delta == 0) {
        int x0 = -b / (2 * a);
        Vector pc = Vector(v1.x() + x0 * (v2.x() - v1.x()),
                          v1.y() + x0 * (v2.y() - v1.y()),
                          v1.z() + x0 * (v2.z() - v1.z())
                         );

        return Intersection(true, this, new Line(v1, pc, false), pc.length() );

    } else {
        float x0 = ( - b + sqrt(delta)) / ( 2 * a ) ;
        float x1 = ( - b - sqrt(delta)) / ( 2 * a ) ;


        Vector pcA = Vector(v1.x() + x0 * (v2.x() - v1.x()),
                           v1.y()  + x0 * (v2.y() - v1.y()),
                           v1.z()  + x0 * (v2.z() - v1.z())
        );

        Vector pcB = Vector(v1.x() + x1 * (v2.x() - v1.x()),
                            v1.y() + x1 * (v2.y() - v1.y()),
                            v1.z() + x1 * (v2.z() - v1.z())
        );

        Line *lineA = new Line(v1, pcA, false);
        Line *lineB = new Line(v1, pcB, false);
        float dstA = sqrt(pcA.x() * pcA.x() + pcA.y() * pcA.y());
        float dstB = sqrt(pcB.x() * pcB.x() + pcB.y() * pcB.y());

        if(dstA < dstB) {
            return Intersection(true, this, lineA, dstA );
        } else {
            return Intersection(true, this, lineB, dstB );
        }
    }
}


const Vector Sphere::normal(const Vector& vec) const {
    Vector n = vec - _center;
    n.normalize();
    return n;
}
