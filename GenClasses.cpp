#include "GenClasses.h"

int RNG::getint(int min, int max)
{
  return (int)RNG::getreal() * (max - min) + min;
}

int RNG::getint(int min, int max, int excluded)
{
  if (excluded > min && excluded < max)
    {
      int res = RNG::getint(min, max - 1);
      return (res <= excluded) ? res : res + 1;
    }
  else
    { return RNG::getint(min, max); }
}

qreal RNG::getreal(qreal min, qreal max)
{
  return RNG::getreal() * (max - min) + min;
}

double RNG::getreal()
{
  return qrand() * 1.0 / RAND_MAX;
}
