#ifndef __Animations__
#define __Animations__


#include "Constants.h"
#include "Coordinates.h"


typedef Cartesian (*animFuncPtr)(double);

double hauntedY = 0.66;

Cartesian hauntedHouseHorizontal(double theProgress){
  double myAngle = asin(hauntedY);
  double myX = cos(myAngle);

  return (Cartesian){
    myX * cos(theProgress * TWO_PI),
    hauntedY
  };
}

double hauntedX = 0.56;

Cartesian hauntedHouseVertical(double theProgress){
  double myAngle = asin(hauntedX);
  double myX = cos(myAngle);

  return (Cartesian){
    myX * (cos(theProgress * TWO_PI) * 0.5 + 0.5),
    hauntedX
  };
}

double pendularRadius = 0.9;

Cartesian pendular(double theProgress){
  return toCartesian((Polar){
    cos(theProgress * TWO_PI) * MAX_ANGLE,
    pendularRadius
  });
}

double linearAngle = HALF_PI / 2;

Cartesian linear(double theProgress){
  return toCartesian((Polar){
      linearAngle,
      cos(theProgress * TWO_PI) * 0.5 + 0.5
   });
}

#endif
