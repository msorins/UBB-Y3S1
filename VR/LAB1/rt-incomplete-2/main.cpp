#include <cmath>
#include <iostream>
#include <string>
#include <math.h>

#include "Vector.hpp"
#include "Line.hpp"
#include "Geometry.hpp"
#include "Sphere.hpp"
#include "Image.hpp"
#include "Color.hpp"
#include "Intersection.hpp"
#include "Material.hpp"

#include "Scene.hpp"
#include "Light.hpp"

using namespace std;
using namespace rt;




float imageToViewPlane(int n, int imgSize, float viewPlaneSize) {
    float u = (float)n*viewPlaneSize / (float)imgSize;
    u -= viewPlaneSize / 2;
    return u;
}

const Intersection findFirstIntersection(const Line& ray,
    float minDist, float maxDist) {
    Intersection intersection;

    for (int i = 0; i < geometryCount; i++) {
        Intersection in = scene[i]->getIntersection(ray, minDist, maxDist);
        if (in.valid()) {
            if (!intersection.valid()) {
                intersection = in;
            }
            else if (in.t() < intersection.t()) {
                intersection = in;
            }
        }
    }

    return intersection;
}

int main() {
    Vector const viewPoint(0, 0, 0);
    Vector viewDirection(0, 0, 1);
    Vector viewUp(0, -1, 0);

    float frontPlaneDist = 0;
    float backPlaneDist = 1000;

    float viewPlaneDist = 512;
    float viewPlaneWidth = 1024;
    float viewPlaneHeight = 768;

    int imageWidth = 1024;
    int imageHeight = 768;

    Vector viewParallel = viewUp^viewDirection;

    viewDirection.normalize();
    viewUp.normalize();
    viewParallel.normalize();

    Image image(imageWidth, imageHeight);

	// ADD CODE HERE
	for(int x = 0; x < imageWidth; x++) {
	    for(int y = 0; y < imageHeight; y++) {

	        // Find v2 = view plane point
	        Vector v1 = viewPoint;
	        Vector v2 = viewPoint +
	                viewDirection * viewPlaneDist +
	                viewUp * imageToViewPlane(y, imageHeight, viewPlaneHeight) +
	                viewParallel * imageToViewPlane(x, imageWidth, viewPlaneWidth);

	       // Form the ray
	       Line ray = Line(v1, v2, false);

	       // Shot the ray and get the first intersaction
	       Intersection i = findFirstIntersection(ray, frontPlaneDist, backPlaneDist);

	       // If intersection is valid, render the color
	       Color cl;
	        if( i.valid() ) {
	            Color cl = i.geometry()->material().ambient();
	            for(auto light: lights) {
	                cl *= light->ambient();
	            }

	            for(auto light: lights) {
                    Vector N = i.geometry()->normal( i.vec() ); N.normalize(); N *= -1; // Normal to the surface at the intersection point
                    Vector T = Vector( i.vec() - light->position() ); T.normalize(); // Vector from the intersection point to the light

                    Vector E = Vector(i.vec() - viewPoint); E.normalize(); // Vector from the intersection point to the camera (0, 0, 0)
                    Vector R  = N*(N*T)*2 - T; R.normalize();   // Reflection vector

                    if(N * T > 0) {
                        cl += i.geometry()->material().diffuse() * light->diffuse() * ( N * T );
                    }

                    if(E * R > 0) {
                        cl += i.geometry()->material().specular() * light->specular() * pow(E*R, i.geometry()->material().shininess() );
                    }
	            }

	            image.setPixel(x, y, cl);
	        } else {
                image.setPixel(x, y, Color(0, 0, 0));
	        }
	    }
	}

    image.store("scene.png");

    for (int i = 0; i < geometryCount; i++) {
        delete scene[i];
    }

    return 0;
}
