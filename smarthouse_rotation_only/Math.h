#ifndef __Math__
#define __Math__

double saturate(double val){
  return max(0,min(val, 1));
}

double smoothStep(double edge0, double edge1, double x) {
    double t = saturate((x - edge0) / (edge1 - edge0));
    return t * t * (3.0 - 2.0 * t);
}

double interpolateCos(double t){
  return (cos(PI + saturate(t) * PI) +1) / 2;
}

double dRandom() {
  return random(0, 100000) / 100000. ;
}

#endif
