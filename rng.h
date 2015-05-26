#ifndef RNG_H
#define RNG_H

#include <QtGlobal>

class RNG {
  public:
    static int getint(int min, int max);
    static int getint(int min, int max, int excluded);
    static qreal getreal();
    static qreal getreal(qreal min, qreal max);
};

#endif // RNG_H
