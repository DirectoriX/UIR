#include "square.h"

void Square::inherit(ICreature *parent1, ICreature *parent2)
{
  this->a = (RNG::getreal() < 0.5) ? (((Square *)parent1)->a) : (((Square *)parent2)->a);
  this->b = (RNG::getreal() < 0.5) ? (((Square *)parent1)->b) : (((Square *)parent2)->b);
}

void Square::randomize()
{
  this->a = RNG::getreal(min, max);
  this->b = RNG::getreal(min, max);
}

void Square::mutate(qreal chance)
{
  if (RNG::getreal() < chance)
    { this->a = RNG::getreal(min, max); }

  if (RNG::getreal() < chance)
    { this->b = RNG::getreal(min, max); }
}

qreal Square::getfitness()
{
  return this->fitness;
}

QString Square::getinfo()
{
  return QString::number(fitness, 'G', 5);
}


void Square::calculate()
{
  /// Some blueprint to understand math
  ///
  /// *----------*
  ///  \        /|
  ///   \     d/ |
  ///   1\    /  |
  ///     \  /  1|
  ///      \/    |
  ///       *----*
  ///       a  1  b
  ///
  this->perimeter = 0;
  qreal  d, alpha = qDegreesToRadians(this->a), beta = qDegreesToRadians(this->b);
  d = 2 * qSin(beta / 2);
  qreal a_i = alpha + beta / 2 - qDegreesToRadians(90.0);
  this->square = (d * qSin(a_i) + qSin(beta)) / 2;
  this->perimeter = 3 + qSqrt(1 + d * d - 2 * d * qCos(a_i));
  this->fitness = this->square / this->perimeter;
}


void Square::showfullinfo()
{
}
