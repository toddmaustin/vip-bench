

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

//#include "../config.h"
#include "mathlib.h"

VIP_ENCDOUBLE
myfloor(VIP_ENCDOUBLE x)
{
  VIP_ENCLONG i = (VIP_ENCLONG)x;
#ifdef VIP_DO_MODE
  VIP_ENCDOUBLE _retval = 0.0;
  VIP_ENCBOOL _pred = (i > x);
  _retval = VIP_CMOV(_pred, (VIP_ENCDOUBLE)(i-1), (VIP_ENCDOUBLE)i);
 
  return _retval;
#else /* !VIP_DO_MODE */
  return i - (i > x);
#endif /* VIP_DO_MODE */
}

VIP_ENCDOUBLE
myfabs(VIP_ENCDOUBLE x)
{
#ifdef VIP_DO_MODE
  VIP_ENCDOUBLE _retval = 0.0;
  VIP_ENCBOOL _pred = (x < 0.0);
  _retval = VIP_CMOV(_pred, -x, x);

  return _retval;
#else /* !VIP_DO_MODE */
  if (x < 0.0)
    return -x;
  else
    return x;
#endif /* VIP_DO_MODE */
}

VIP_ENCDOUBLE
mypow(VIP_ENCDOUBLE x, unsigned exp)
{
#ifdef VIP_DO_MODE
  VIP_ENCDOUBLE _retval = 0.0;

  VIP_ENCBOOL _pred1 = (exp == 0);
  _retval = VIP_CMOV(_pred1, (VIP_ENCDOUBLE)1.0L, _retval);

  _retval = VIP_CMOV(!_pred1, x, _retval);
  for (unsigned i=1; i < exp; i++)
    _retval = VIP_CMOV(!_pred1, _retval * x, _retval);

  return _retval;  
#else /* !VIP_DO_MODE */
  if (exp == 0)
    return 1.0L;

  double retval = x;
  for (unsigned i=1; i < exp; i++)
    retval *= x;
  return retval;
#endif /* VIP_DO_MODE */
}


VIP_ENCDOUBLE
invsqrt(VIP_ENCDOUBLE num)
{
  VIP_ENCDOUBLE y = num;
  VIP_ENCDOUBLE x2 = y * 0.5;
  VIP_ENCLONG i = *(VIP_ENCLONG *) &y;

  // The magic number is for doubles is from https://cs.uwaterloo.ca/~m32rober/rsqrt.pdf
  i = (VIP_ENCLONG)0x5fe6eb50c7b537a9 - (i >> 1);
  y = *(VIP_ENCDOUBLE *) &i;
  y = y * ((VIP_ENCDOUBLE)1.5 - (x2 * y * y));   // 1st iteration
  y  = y * ((VIP_ENCDOUBLE)1.5 - ( x2 * y * y ) );   // 2nd iteration, this can be removed
  return y;
}

VIP_ENCDOUBLE
mysqrt(VIP_ENCDOUBLE num)
{
  return (VIP_ENCDOUBLE)1.0 / invsqrt(num);
}

void
sincos(VIP_ENCDOUBLE *psin, VIP_ENCDOUBLE *pcos, VIP_ENCDOUBLE x)
{
  VIP_ENCDOUBLE sin = *psin, cos = *pcos;

#ifdef VIP_DO_MODE
  //always wrap input angle to -PI..PI
  VIP_ENCBOOL _pred1 = (x < -3.14159265);
  VIP_ENCBOOL _pred2 = (x >  3.14159265);
  x = VIP_CMOV(_pred1, x + 6.28318531, x);
  x = VIP_CMOV(!_pred1 && _pred2, x - 6.28318531, x);

  //compute sine
  VIP_ENCBOOL _pred3 = (x < 0);

  sin = VIP_CMOV(_pred3, (VIP_ENCDOUBLE)1.27323954 * x + (VIP_ENCDOUBLE).405284735 * x * x, x);
  VIP_ENCBOOL _pred4 = (sin < 0);
  sin = VIP_CMOV(_pred3 && _pred4, (VIP_ENCDOUBLE)0.225 * (sin *-sin - sin) + sin, sin);
  sin = VIP_CMOV(_pred3 && !_pred4, (VIP_ENCDOUBLE)0.225 * (sin * sin - sin) + sin, sin);

  sin = VIP_CMOV(!_pred3, (VIP_ENCDOUBLE)1.27323954 * x - (VIP_ENCDOUBLE)0.405284735 * x * x, sin);
  VIP_ENCBOOL _pred5 = (sin < 0);
  sin = VIP_CMOV(!_pred3 && _pred5, (VIP_ENCDOUBLE)0.225 * (sin *-sin - sin) + sin, sin);
  sin = VIP_CMOV(!_pred3 && !_pred5, (VIP_ENCDOUBLE)0.225 * (sin * sin - sin) + sin, sin);

  //compute cosine: sin(x + PI/2) = cos(x)
  x += 1.57079632;

  VIP_ENCBOOL _pred6 = (x >  3.14159265);
  x = VIP_CMOV(_pred6, x - 6.28318531, x);

  VIP_ENCBOOL _pred7 = (x < 0);

  cos = VIP_CMOV(_pred7, (VIP_ENCDOUBLE)1.27323954 * x + (VIP_ENCDOUBLE)0.405284735 * x * x, cos);
  VIP_ENCBOOL _pred8 = (cos < 0);
  cos = VIP_CMOV(_pred7 && _pred8, (VIP_ENCDOUBLE)0.225 * (cos *-cos - cos) + cos, cos);
  cos = VIP_CMOV(_pred7 && !_pred8, (VIP_ENCDOUBLE)0.225 * (cos * cos - cos) + cos, cos);

  cos = VIP_CMOV(!_pred7, (VIP_ENCDOUBLE)1.27323954 * x - (VIP_ENCDOUBLE)0.405284735 * x * x, cos);
  VIP_ENCBOOL _pred9 = (cos < 0);
  cos = VIP_CMOV(!_pred7 && _pred9, (VIP_ENCDOUBLE)0.225 * (cos *-cos - cos) + cos, cos);
  cos = VIP_CMOV(!_pred7 && !_pred9, (VIP_ENCDOUBLE)0.225 * (cos * cos - cos) + cos, cos);
#else /* !VIP_DO_MODE */
  //always wrap input angle to -PI..PI
  if (x < -3.14159265) /* _pred1: ... */
    x += 6.28318531;
  else if (x >  3.14159265) /* _pred2: ... */
    x -= 6.28318531;

  //compute sine
  if (x < 0) /* _pred3: ... */
  {
    sin = 1.27323954 * x + .405284735 * x * x;

    if (sin < 0) /* _pred4: ... */
      sin = .225 * (sin *-sin - sin) + sin;
    else
      sin = .225 * (sin * sin - sin) + sin;
  }
  else
  {
    sin = 1.27323954 * x - 0.405284735 * x * x;

    if (sin < 0) /* _pred5: ... */
      sin = .225 * (sin *-sin - sin) + sin;
    else
     sin = .225 * (sin * sin - sin) + sin;
  }

  //compute cosine: sin(x + PI/2) = cos(x)
  x += 1.57079632;
  if (x >  3.14159265) /* _pred6: ... */
    x -= 6.28318531;

  if (x < 0) /* _pred7: ... */
  {
    cos = 1.27323954 * x + 0.405284735 * x * x;

    if (cos < 0) /* _pred8: ... */
      cos = .225 * (cos *-cos - cos) + cos;
    else
      cos = .225 * (cos * cos - cos) + cos;
  }
  else
  {
    cos = 1.27323954 * x - 0.405284735 * x * x;

    if (cos < 0) /* _pred9: ... */
      cos = .225 * (cos *-cos - cos) + cos;
    else
      cos = .225 * (cos * cos - cos) + cos;
  }
#endif /* VIP_DO_MODE */

  *psin = sin;
  *pcos = cos;
}

VIP_ENCDOUBLE
mysin(VIP_ENCDOUBLE x)
{
  VIP_ENCDOUBLE sin, cos;

  sincos(&sin, &cos, x);
  return sin;
}

VIP_ENCDOUBLE
mycos(VIP_ENCDOUBLE x)
{
  VIP_ENCDOUBLE sin, cos;

  sincos(&sin, &cos, x);
  return cos;
}

#ifdef TEST
int
main(void)
{
  fprintf(stderr, "mysqrt(64.0) = %lf (should be 8.0)\n", mysqrt(64.0));
  fprintf(stderr, "mysqrt(4.0) = %lf (should be 2.0) \n", mysqrt(4.0));
  fprintf(stderr, "mysqrt(34232.6) = %lf (should be 185.0205394)\n", mysqrt(34232.6));
  fprintf(stderr, "\n");
  fprintf(stderr, "mysin(0.0) = %lf (should be 0.0)\n", mysin(0.0));
  fprintf(stderr, "mysin(pi/6) = %lf (should be 0.5)\n", mysin(0.52359877559829887307710723054658383L));
  fprintf(stderr, "mysin(-pi/6) = %lf (should be 0.5)\n", mysin(-0.52359877559829887307710723054658383L));
  fprintf(stderr, "mysin(pi/2) = %lf (should be 1.0)\n", mysin(1.5707963267948966192313216916397514L));
  fprintf(stderr, "mysin(-pi/2) = %lf (should be -1.0)\n", mysin(-1.5707963267948966192313216916397514L));
  fprintf(stderr, "mysin(0.7) = %lf (should be 0.64421768723769)\n", mysin(0.7L));
  fprintf(stderr, "\n");
  fprintf(stderr, "mycos(0.0) = %lf (should be 1.0)\n", mycos(0.0));
  fprintf(stderr, "mycos(2 * pi/6) = %lf (should be 0.5)\n", mycos(2.0L * 0.52359877559829887307710723054658383L));
  fprintf(stderr, "mycos(4 * pi/6) = %lf (should be -0.5)\n", mycos(4.0L * 0.52359877559829887307710723054658383L));
  fprintf(stderr, "mycos(pi/2) = %lf (should be 0.0)\n", mycos(1.5707963267948966192313216916397514L));
  fprintf(stderr, "mycos(0.7) = %lf (should be 0.764842187284)\n", mycos(0.7L));
  fprintf(stderr, "\n");
  fprintf(stderr, "mypow(2.0, 0) = %lf (should be 1.0)\n", mypow(2.0, 0));
  fprintf(stderr, "mypow(2.0, 1) = %lf (should be 2.0)\n", mypow(2.0, 1));
  fprintf(stderr, "mypow(2.0, 10) = %lf (should be 1024.0)\n", mypow(2.0, 10));
  fprintf(stderr, "\n");
  fprintf(stderr, "myfloor(7.0) = %lf (should be 7.0)\n", myfloor(7.0L));
  fprintf(stderr, "myfloor(7.1) = %lf (should be 7.0)\n", myfloor(7.1L));
  fprintf(stderr, "myfloor(7.9) = %lf (should be 7.0)\n", myfloor(7.9L));
  fprintf(stderr, "myfloor(-7.0) = %lf (should be -7.0)\n", myfloor(-7.0L));
  fprintf(stderr, "myfloor(-7.1) = %lf (should be -8.0)\n", myfloor(-7.1L));
  fprintf(stderr, "myfloor(-7.9) = %lf (should be -8.0)\n", myfloor(-7.9L));

  return 0;
}
#endif /* TEST */
