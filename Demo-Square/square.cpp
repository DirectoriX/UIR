#include "square.h"

InfoWindow *iw;
const qreal min = M_PI / 3, max = M_PI - 0.00001;

Square::Square()
{
  realParams = new qreal[2];
  realParams[0] = RNG::getreal(min, max);
  realParams[1] = RNG::getreal(min, max);
}

Square::~Square()
{
  delete realParams;
  disconnect(this);
}

Square *Square::create()
{
  return new Square();
}

void Square::initDoubleParam(quint32 number)
{
  if (number >= paramCount())
    { return; }

  realParams[number] = RNG::getreal(min, max);
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
  qreal d = 2 * qSin(realParams[1] / 2);
  qreal a_i = realParams[0] + realParams[1] / 2 - M_PI_2;
  square = (d * qSin(a_i) + qSin(realParams[1])) / 2;
  perimeter = 3 + qSqrt(1 + d * d - 2 * d * qCos(a_i));
  fitness = square / perimeter;
}

void Square::prepare()
{
  iw = new InfoWindow();
}

void Square::clean()
{
  delete iw;
}

quint32 Square::paramCount()
{
  return 2;
}

QString Square::name()
{
  return "DemoSquare";
}

bool Square::threadable()
{
  return true;
}

QDialog *Square::getInfoWindow()
{
  return iw;
}

void Square::updateInfoWindow()
{
  connect(this, SIGNAL(updateInfo(qreal, qreal)), iw, SLOT(updateL(qreal, qreal)));
  emit updateInfo(realParams[0], realParams[1]);
}

bool Square::isSimpleMutation()
{
  return true;
}

void Square::mutate(bool onlyOnce, qreal probability)
{
  return;
}

bool Square::isSimpleCrossover()
{
  return true;
}

void Square::crossover(ICreature *p1, ICreature *p2)
{
  return;
}
