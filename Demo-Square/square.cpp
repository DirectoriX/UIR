#include "square.h"

Square::Square()
{
  doubleparams.append(RNG::getreal(min, max));
  doubleparams.append(RNG::getreal(min, max));
}

Square::~Square()
{
  doubleparams.clear();
}

Square *Square::create()
{
  return new Square();
}

void Square::initdoubleparam(qint32 number)
{
  if (number > paramcount)
    { return; }

  doubleparams[number] = RNG::getreal(min, max);
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
  perimeter = 0;
  qreal  d = 2 * qSin(doubleparams[1] / 2);
  qreal a_i = doubleparams[0] + doubleparams[1] / 2 - M_PI_2;
  square = (d * qSin(a_i) + qSin(doubleparams[1])) / 2;
  perimeter = 3 + qSqrt(1 + d * d - 2 * d * qCos(a_i));
  fitness = square / perimeter;
}


void Square::prepare()
{
  paramcount = 2;
  name = "DemoSquare";
  paramnames.append("a");
  paramnames.append("b");
}
