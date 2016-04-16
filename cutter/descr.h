#ifndef DESCR_H
#define DESCR_H

#include <QVector>

struct detailType
{
  qreal width, height;
};
Q_DECLARE_METATYPE(detailType);

struct detail
{
  qreal x, y, width, height;
  quint32 type;
  bool flipped;
};
Q_DECLARE_METATYPE(detail);

struct place
{
  qreal x, y, maxx, maxy;

};

#endif // DESCR_H
