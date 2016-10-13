#ifndef PRG_H
#define PRG_H

#include <QtMath>

#include "../icreature.h"
#include "../rng.h"

#include "www.h"

class prg : public QObject, public ICreature {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "DirectoriX.UIR.Creature" FILE "pereguda.json")
    Q_INTERFACES(ICreature)

  public:
    prg();
    ~prg();

    prg *create();
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
    void updateInfo(double *arr);
};

#endif // PRG_H
