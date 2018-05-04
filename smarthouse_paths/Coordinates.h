#ifndef __Cartesian__
#define __Cartesian__

/**
 * Contains structs and converstions methods for polar and cartesian coordinates
 */

struct Cartesian{
  double x;
  double y;
};

struct Polar{
  double a;
  double r;
};

Cartesian toCartesian(Polar thePolar){
  return (Cartesian){
    cos( thePolar.a ) * thePolar.r,
    sin( thePolar.a ) * thePolar.r
  };
}
  
Polar toPolar(Cartesian theCartesian){
  return (Polar){
    atan2(theCartesian.y, theCartesian.x),
    sqrt(theCartesian.x * theCartesian.x + theCartesian.y * theCartesian.y)
  };
}

#endif
