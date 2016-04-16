#ifndef SQUARE_H
#define SQUARE_H

#include <QtMath>

#include "../icreature.h"
#include "../rng.h"

#include "infowindow.h"

class Square : public QObject, public ICreature {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "DirectoriX.UIR.Creature" FILE "Demo-Square.json")
    Q_INTERFACES(ICreature)

  public:
    Square();
    ~Square();

    Square *create();
    void initDoubleParam(quint32 number);
    void calculate();
    void prepare();
    void clean();
    quint32 paramCount();
    QString name();
    bool threadable();
    QDialog *getInfoWindow();
    void updateInfoWindow();
    bool isSimpleMutation();
    void mutate(bool onlyOnce, qreal probability);
    bool isSimpleCrossover();
    void crossover(ICreature *p1, ICreature *p2);

  signals:
    void updateInfo(qreal a, qreal b);

  private:
    qreal square, perimeter;

};

#endif // SQUARE_H
