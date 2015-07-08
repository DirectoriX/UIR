#include "rng.h"

qint32 RNG::getint(qint32 min, qint32 max)
{
  return (int)(RNG::getreal() * (max - min) + min);
}

qint32 RNG::getint(qint32 min, qint32 max, qint32 excluded)
{
  if (excluded > min && excluded < max)
    {
      qint32 res = RNG::getint(min, max - 1);
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
