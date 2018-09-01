#ifndef __Filter__
#define __Filter__
#define ARM_FLOAT
enum OSCILLATOR_TYPE {
  LOWPASS_BESSEL,
  //HIGHPASS_BESSEL,
  LOWPASS_BUTTERWORTH,
  //HIGHPASS_BUTTERWORTH,
};

// implements a driven harmonic oscillator, which can be used as a filter
// mass is normalized to unity (does not appear explicity in the equations),
// and the driving force is writtin in units of spring constant, so a static force
// of X will cause the oscillator come to rest at X
//
// The oscillator parameters are
//   W0 (undampened resonant frequency) ... the user sets this in Hz (as F0)
//   Q  (quality factor)
//
// In addition, the ocillator can be configured as a (two-pole) lowpass or high filter,
// since these circuits are directly analogous to harmonic oscillators.

// note that the two-pole lowpass Bessel, the rise time (for a step) and
// and impulse response width are approx tau/2.0, or 1/(2*w0):
//  a tau = 1 is w0 (and wc) of TWO_PI
//  this has a rise time / impulse response of about 0.4
// (i.e., the intergration time is about 0.4 * tau)

class FilterTwoPole {

  private:
    //OSCILLATOR_TYPE FT;
    float X;        // position
    float Vprev;    // previously computed velocity
    float Vavg;     // average of the last two calculated velocities
    float Fprev;    // previous driving force (not frequency!)

    float Q;        // quality factor, must be > 0
    float W0;       // undamped resonance frequency

    bool IsHighpass;  // false for normal output, true will make a lowpass into a highpass

    long LastTimeUS;  // last time measured
    
  public:
  
    FilterTwoPole( float frequency0 = 1, float qualityFactor = 1, float xInit = 0) {
      X = xInit;              // start it some arbitrary position
      Vprev = 0;              // initially stopped
      IsHighpass = false;     // by default, a normal oscillator

      setQ( qualityFactor );
      setFrequency0( frequency0 );

      LastTimeUS = micros();
    }

    void setQ( float qualityFactor ) {
      // zero will result in divide by zero, upper value keeps it stable
      qualityFactor = constrain( qualityFactor, 1e-3, 1e3 );

      Q = qualityFactor;
    }

    void setFrequency0( float f ) {
      W0 = TWO_PI * abs(f);
    }

    void setAsFilter( OSCILLATOR_TYPE ft, float frequency3db, float initialValue = 0 ) {
      // if this is a highpass filter, set to invert the transfer function on the output
      //if( ft == HIGHPASS_BESSEL || ft == HIGHPASS_BUTTERWORTH ) {
      //  IsHighpass = true;
      //}
      //else {
      IsHighpass = false;
      //}

      X = initialValue;

      if ( ft == LOWPASS_BESSEL ) {
        setFrequency0( frequency3db * 1.28 );
        setQ( 0.5774 );
      }
      //else if( ft == HIGHPASS_BESSEL ) {
      //  setFrequency0( frequency3db * 1.28 );
      //  setQ( 0.5774 );
      //}

      else if ( ft == LOWPASS_BUTTERWORTH ) {
        // set as butterworth
        setFrequency0( frequency3db );
        setQ( 0.7071 );
      }
      //else if( ft == HIGHPASS_BUTTERWORTH ) {
      // set as butterworth
      //  setFrequency0( frequency3db );
      //  setQ( 0.7071 );
      //}


    }


    float input( float drive = 0 ) {
      Fprev = drive;                      // needed when using filter as a highpass

      long now = micros();                      // get current time
      float dt = 1e-6 * float(now - LastTimeUS); // find dt
      LastTimeUS = now;                         // save the last time

      // constrain the dt
      // if input has not been called frequently enough
      // the velocity and position can fly off to extremly high values
      // ... constraining the dt effectively "pauses" the motion during delays in updating
      // note this will result in an incorrect answer, but if dt is too large
      // the answer will be incorrect, regardless.
      dt = constrain( dt, 0, 1.0 / W0 );

      float A = sq(W0) * drive - W0 / Q * Vprev - sq(W0) * X; // *** compute acceleration
      float V = Vprev + A * dt;                       // step velocity
      Vavg = .5 * (V + Vprev);
      X += Vavg * dt;                                 // step position, using average V to reduce error
      // (trapezoidal integration)

      Vprev = V;                                      // save the last V

      // normally, this returns output
      // use it here to figure out how to return highpass

      //return Q/W0*Vavg;
      return output();
    }


    float output() {
      //  if( IsHighpass )
      //    return Fprev-X-Q/W0*Vavg;   // this is almost correct ...
      //  else
      return X;          // the filtered value (position of oscillator)
    }

    // as a measure for the energy of the oscillator, returns the maxium amplitude
    float getMaxAmp() {
      // first, calculate the energy
      // E = 0.5*w0*x² + 0.5*v²/w0

      float E = 0.5 * W0 * sq(X) + 0.5 * sq(Vprev) / W0;

      // calculate use this to calculate max amplitude
      // E = 0.5*w0*x²
      // x = sqrt(2*E/w0)
#ifdef ARM_FLOAT
      return sqrtf(2.0 * E / W0);
#else
      return sqrt(2.0 * E / W0);
#endif
    }

    void print();

    void test();

};
#endif
