#include <QtMath>

#include "cutter.h"
#include "info.h"
#include "descr.h"
#include "setup.h"
#include "../rng.h"

info *w = NULL;
QVector<detailType> types;
QVector<qint32> detailSet;
qint8 mode = 3;
bool hasfree = true;
qint32 typecount, detailCount;
qreal maxwidth, maxheight;

bool cmpRange(place a, place b)
{
  switch (mode)
    {
      case 0:
        return (a.y != b.y) ? a.y < b.y : a.x < b.x; // Строками

      case 1:
        return (a.x != b.x) ? a.x < b.x : a.y < b.y; // Столбцами

      case 2:
        return (a.x + a.y != b.x + b.y) ? a.x + a.y < b.x + b.y : ((a.y != b.y) ? a.y < b.y : a.x < b.x); // Диагональю

      case 3:
      {
        qreal ar, br;
        ar = qSqrt(qPow(a.x, 2) + qPow(a.y, 2));
        br = qSqrt(qPow(b.x, 2) + qPow(b.y, 2));
        return (ar != br) ? ar < br : ((a.x != b.x) ? a.x < b.x : a.y < b.y); // Сектором
      }

      case 4:
        return (qMin(a.x, a.y) != qMin(b.x, b.y)) ? qMin(a.x, a.y) < qMin(b.x, b.y) : ((a.y != b.y) ? a.y < b.y : a.x < b.x); // Строками и столбцами

      case 5:
        return (qMax(a.x, a.y) != qMax(b.x, b.y)) ? qMax(a.x, a.y) < qMax(b.x, b.y) : ((a.y != b.y) ? a.y < b.y : a.x < b.x); // Квадратом

      case 6:
        return (a.x * a.y != b.x * b.y) ? a.x * a.y < b.x * b.y : ((a.y != b.y) ? a.y < b.y : a.x < b.x); // Обрезком

      case 7:
      case 8:
        return false;
    }
}

cutter::cutter()
{
  if (!w)
    { return; }

  details = QVector<qint32>(detailSet);
  quint32 m = details.count();
  quint32 i;

  while (m)
    {
      i = qFloor(RNG::getreal() * m--);
      qSwap(details[m], details[i]);
    }

  for (i = 0; i < detailCount; i++)
    {
      details[i] *= (RNG::getreal() > 0.5) ? 1 : -1;
    }

  places.append(place{ 0, 0, maxwidth, maxheight });
  totalwidth = totalheight = square = freex = freey = 0;
}

cutter::~cutter()
{
  disconnect(this);
}

quint32 cutter::paramCount() {return 0;}

QString cutter::name() {return "Раскрой";}

bool cutter::threadable() {return true;}

bool cutter::isSimpleMutation() {return false;}

void cutter::mutate(bool onlyOnce, qreal probability)
{
  if (onlyOnce)
    {
      if (RNG::getreal() < probability)
        {
          if (RNG::getreal() < 0.5)
            { details[RNG::getint(0, detailCount)] *= -1; }
          else
            {
              qint32 c1 = RNG::getint(0, detailCount);
              qint32 c2 = RNG::getint(0, detailCount, c1);
              qSwap(c1, c2);
            }
        }
    }
  else
    {
      for (quint32 i = 0; i < RNG::getreal() * probability * detailCount; i++)
        {
          if (RNG::getreal() < 0.5)
            { details[RNG::getint(0, detailCount)] *= -1; }
          else
            {
              qint32 c1 = RNG::getint(0, detailCount);
              qint32 c2 = RNG::getint(0, detailCount, c1);
              qSwap(c1, c2);
            }
        }
    }
}

bool cutter::isSimpleCrossover() {return false;}

void cutter::crossover(ICreature *p1, ICreature *p2)
{
  cutter *pT1, *pT2;
  pT1 = (cutter *)p1;
  pT2 = (cutter *)p2;
  details = QVector<qint32>(pT1->details);
  QVector<qint32> p2d = QVector<qint32>(pT2->details);
  qint32 i;

  for (i = 0; i < detailCount; i++)
    {
      details[i] = qAbs(details[i]);
      p2d[i] = qAbs(p2d[i]);
    }

  for (i = 0; i < detailCount / 2; i++)
    {
      p2d.removeOne(details[i]);
      details.removeLast();
    }

  if (detailCount % 2 == 1)
    {
      p2d.removeOne(details[detailCount / 2]);
    }

  details += p2d;
  p2d = QVector<qint32>(pT2->details);

  for (i = 0; i < detailCount; i++)
    {
      details[i] *= p2d[i] > 0 ? 1 : -1;
    }
}

ICreature *cutter::create()
{
  return new cutter();
}

void cutter::initDoubleParam(quint32 number) {}

void cutter::calculate()
{
  list.clear();
  places.clear();
  places.append(place{ 0, 0, maxwidth, maxheight });
  totalwidth = totalheight = square = freex = freey = 0;
  bool success = true;

  for (qint32 i = 0; i < detailCount; i++)
    { success &= addRect(details[i]); }

  qreal totalsquare = totalwidth * totalheight;
  qreal freesquare;

  if (hasfree)
    { freesquare = (totalwidth - freex) * (totalheight - freey); }
  else { freesquare = 0; }

  fitness = square / (totalsquare - freesquare);

  if (!success)
    { fitness -= 1; }

  places.clear();
}

void cutter::prepare()
{
  qRegisterMetaType<QVector<detailType>/**/>();
  qRegisterMetaType<QVector<detail>/**/>();
  w = new info();
  setup *s = new setup();
  s->exec();
  maxwidth = s->getWidth();
  maxheight = s->getHeight();
  types = QVector<detailType>(*s->getTypes());
  typecount = types.count();
  detailSet = QVector<qint32>();
  QVector<quint32> c = *s->getCount();

  for (qint32 i = 0, n = c.count(); i < n; i++)
    {
      for (quint32 j = 0; j < c[i]; j++)
        { detailSet.append(i + 1); }
    }

  mode = s->getMode();
  hasfree = (mode == 2 || mode == 3 || mode == 4 || mode == 6 || mode == 8);
  delete s;
  detailCount = detailSet.count();
  connect(this, SIGNAL(setFieldSize(qreal, qreal)), w, SLOT(setFieldSize(qreal, qreal)));
  connect(this, SIGNAL(setTypes(QVector<detailType> *)), w, SLOT(setTypes(QVector<detailType> *)));
  emit setFieldSize(maxwidth, maxheight);
  emit setTypes(&types);
}

void cutter::clean()
{
  delete w;
}

QDialog *cutter::getInfoWindow()
{
  return w;
}

void cutter::updateInfoWindow()
{
  connect(this, SIGNAL(setRects(QVector<detail> *, qreal, qreal, qreal, qreal)),
          w, SLOT(setRects(QVector<detail> *, qreal, qreal, qreal, qreal)));
  emit setRects(&list, totalwidth, totalheight, freex, freey);
}

bool cutter::addRect(qint32 type)
{
  qreal x, y, width, height;
  bool swap = type < 0;
  type = qAbs(type) - 1;
  width = types[type].width;
  height = types[type].height;

  if (swap) { qSwap(width, height); }

  qint32 i;

  for (i = 0; i < places.count(); i++)
    {
      place p = places[i];

      if (p.maxx >= width && p.maxy >= height)
        {
          x = p.x;
          y = p.y;
          break;
        }
    }

  if (i == places.count())
    {
      return false;
    }

  places.remove(i);

  if (x + width > totalwidth)
    { totalwidth = x + width; }

  if (y + height > totalheight)
    { totalheight = y + height; }

  if (hasfree)
    {
      if (x + width > y + height)
        { freey = qMax(freey, y + height); }
      else
        { freex = qMax(freex, x + width); }
    }
  else
    {
      freex = totalwidth;
      freey = totalheight;
    }

  list.append(detail{ x, y, width, height, type, swap });
  square += width * height;
  places.append(place{ 0, totalheight, maxwidth, maxheight - totalheight });
  places.append(place{ totalwidth, 0, maxwidth - totalwidth, maxheight });
  places.append(place{ x + width, y, maxwidth - x - width, maxheight - y });
  places.append(place{ x, y + height, maxwidth - x, maxheight - y - height });

  // Правим наибольшие доступные размеры и удаляем лишние точки
  //  for (i = 0; i < places.count() - 1; i++)
  //    {
  //      if (places[i].x <= x && places[i].y >= y && places[i].y < y + height)
  //        { places[i].maxx = qMin(places[i].maxx, x - places[i].x); }

  //      if (places[i].y <= y && places[i].x >= x && places[i].x < x + width)
  //        { places[i].maxy = qMin(places[i].maxy, y - places[i].y); }

  //      if (places[i].x >= x && places[i].x <= x + width &&
  //          places[i].y >= y && places[i].y <= y + height)
  //        {
  //          places[i].maxx = places[i].maxy = 0;
  //        }
  //    }

  for (i = places.count() - 1; i >= 0; i--)
    {
      for (qint32 j = list.count() - 1; j >= 0; j--)
        {
          detail d = list[j];

          if (places[i].x <= d.x && places[i].y >= d.y && places[i].y < d.y + d.height)
            { places[i].maxx = qMin(places[i].maxx, d.x - places[i].x); }

          if (places[i].y <= d.y && places[i].x >= d.x && places[i].x < d.x + d.width)
            { places[i].maxy = qMin(places[i].maxy, d.y - places[i].y); }

          if (places[i].x >= d.x && places[i].x < d.x + d.width &&
              places[i].y >= d.y && places[i].y < d.y + d.height)
            {
              places[i].maxx = places[i].maxy = 0;
            }
        }

      bool keep = false;

      for (qint32 j = 0; j < typecount; j++)
        {
          keep |= (places[i].maxx >= types[j].width && places[i].maxy >= types[j].height) ||
                  (places[i].maxx >= types[j].height && places[i].maxy >= types[j].width);
        }

      if (!keep)
        {
          places.remove(i);
          continue;
        }
    }

  //  for (i = places.count() - 1; i >= 0; i--)
  //    {
  //      bool keep = false;
  //      place p = places[i];
  //      for (qint32 j = 0; j < typecount; j++)
  //        {
  //          keep |= (p.maxx >= types[j].width && p.maxy >= types[j].height) ||
  //                  (p.maxx >= types[j].height && p.maxy >= types[j].width);
  //        }
  //      if (!keep)
  //        {
  //          places.remove(i);
  //          continue;
  //        }
  //    }
  //  places.append(place{ x + width, y, maxx - width, maxy });
  //  places.append(place{ x, y + height, maxx, maxy - height });
  std::sort(places.begin(), places.end(), cmpRange);
  return true;
}
