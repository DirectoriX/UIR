#include "rng.h"

// Целое число в интервале
qint32 RNG::getint(qint32 min, qint32 max)
{
  return (int)(RNG::getreal() * (max - min - 1) + min);
}

// Целое число в интервале, кроме указанного
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

// Дробное число в интервале
qreal RNG::getreal(qreal min, qreal max)
{
  return RNG::getreal() * (max - min) + min;
}

// Дробное число от 0 до 1
qreal RNG::getreal()
{
  return qrand() * 1.0 / RAND_MAX;
}
