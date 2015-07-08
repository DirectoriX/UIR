#ifndef RNG_H
#define RNG_H

#include <QtGlobal>

class RNG {
  public:
    static qint32 getint(qint32 min, qint32 max);
    static qint32 getint(qint32 min, qint32 max, qint32 excluded);
    static qreal getreal();
    static qreal getreal(qreal min, qreal max);
};

#endif // RNG_H
