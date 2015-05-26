#include "rng.h"

int RNG::getint(int min, int max)
{
  return (int)(RNG::getreal() * (max - min) + min);
}

int RNG::getint(int min, int max, int excluded)
{
  if (excluded > min && excluded < max)
    {
      int res = RNG::getint(min, max - 1);
      return (res >= excluded) ? res + 1 : res;
    }
  else
    { return RNG::getint(min, max); }
}

qreal RNG::getreal(qreal min, qreal max)
{
  return RNG::getreal() * (max - min) + min;
}

qreal RNG::getreal()
{
  return qrand() * 1.0 / RAND_MAX;
}