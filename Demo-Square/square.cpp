#include "square.h"

void Square::inherit(int result, int parent1, int parent2)
{
  if (RNG::getreal() < 0.5)
    {
      ((param *)creatures[result])->a = ((param *)creatures[parent1])->a;
      ((param *)creatures[result])->b = ((param *)creatures[parent2])->b;
    }
  else
    {
      ((param *)creatures[result])->a = ((param *)creatures[parent2])->a;
      ((param *)creatures[result])->b = ((param *)creatures[parent1])->b;
    }
}

void Square::fill(int count)
{
  for (int i = creatures.count() - 1; i < count; i++)
    {
      creatures.append(new param());
    }
}

void Square::randomize(int number)
{
  ((param *)creatures[number])->a = RNG::getreal(min, max);
  ((param *)creatures[number])->b = RNG::getreal(min, max);
}

void Square::mutate(int number, qreal chance)
{
  if (RNG::getreal() < chance)
    { ((param *)creatures[number])->a = RNG::getreal(min, max); }

  if (RNG::getreal() < chance)
    { ((param *)creatures[number])->b = RNG::getreal(min, max); }
}

void Square::sort()
{
  std::sort(creatures.begin(), creatures.end());
}

QStringList Square::getinfo()
{
  QStringList result;

  for (int i = 0; i < creatures.count(); i++)
    {
      result.append(QString::number(creatures[i]->fitness, 'g', 5));
    }

  return result;
}

void Square::showfullinfo(int number)
{
}

void Square::calculate()
{
  for (int i = 0; i < creatures.count(); i++)
    { calculateAt(creatures[i]); }
}

void Square::calculateAt(Creature *c)
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
  param *cre = (param *)c;
  cre->perimeter = 0;
  qreal  d, alpha = qDegreesToRadians(cre->a), beta = qDegreesToRadians(cre->b);
  d = 2 * qSin(beta / 2);
  qreal a_i = alpha + beta / 2 - qDegreesToRadians(90.0);
  cre->square = (d * qSin(a_i) + qSin(beta)) / 2;
  cre->perimeter = 3 + qSqrt(1 + d * d - 2 * d * qCos(a_i));
  cre->fitness = cre->square / cre->perimeter;
}
