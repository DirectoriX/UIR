#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>
#include <QtPlugin>
#include <QtMath>
//#include <cmath>
#include <QMessageBox>

#include "../icreature.h"
#include "../rng.h"

class Square : public QObject, public ICreature {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "DirectoriX.UIR.Creature" FILE "Demo-Square.json")
    Q_INTERFACES(ICreature)

  public:
    Square();
    ~Square();

    Square *create();
    void initdoubleparam(int number);
    void calculate();
    void prepare();

  private:
    static const qreal min = M_PI / 3, max = M_PI;
    qreal square, perimeter;
    static int paramcount1;
};

#endif // SQUARE_H
