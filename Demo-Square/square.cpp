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

  ((param *)creatures[result])->fitness = 0;
}

void Square::fill(int count)
{
  for (int i = creatures.count(); i < count; i++)
    {
      creatures.append(new param());
    }

  for (int i = creatures.count(); i > count; i--)
    {
      delete creatures[i - 1];
      creatures.removeAt(i - 1);
    }
}

void Square::randomize(int number)
{
  ((param *)creatures[number])->a = RNG::getreal(min, max);
  ((param *)creatures[number])->b = RNG::getreal(min, max);
  ((param *)creatures[number])->fitness = 0;
}

void Square::mutate(int number, qreal chance)
{
  if (RNG::getreal() < chance)
    {
      ((param *)creatures[number])->a = RNG::getreal(min, max);
      ((param *)creatures[number])->fitness = 0;
    }

  if (RNG::getreal() < chance)
    {
      ((param *)creatures[number])->b = RNG::getreal(min, max);
      ((param *)creatures[number])->fitness = 0;
    }
}

void Square::sort()
{
  std::sort(creatures.begin(), creatures.end());
}

QStringList Square::getinfo(int count)
{
  QStringList result;

  if (count > creatures.count()) { count = creatures.count(); }

  for (int i = 0; i < count; i++)
    {
      result.append(QString::number(creatures[i]->fitness, 'g', 7).append("; a = ").append(QString::number(((param *)creatures[i])->a, 'g', 8)).append("; b = ").append(QString::number(((param *)creatures[i])->b, 'g', 8)));
    }

  return result;
}

void Square::showfullinfo(int number)
{
  QString text;
  param *p = (param *)(creatures[number]);
  text.append("a = ").append(QString::number(p->a, 'g', 8)).append("\nb = ").append(QString::number(p->b, 'g', 8));
  QMessageBox(QMessageBox::Information, "Параметры",  text, QMessageBox::Ok, NULL).exec();
}

void Square::calculate()
{
  for (int i = 0; i < creatures.count(); i++)
    { calculateAt(creatures[i]); }
}

void Square::calculateAt(Creature *&c)
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
