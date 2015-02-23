#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>
#include <QtPlugin>
#include <QtMath>
#include <../GenClasses.h>

class Square : public QObject, public ICreature {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "DirectoriX.UIR.ICreature" FILE "Demo-Square.json")
    Q_INTERFACES(ICreature)

    // ICreature interface
  public:
    void inherit(ICreature *parent1, ICreature *parent2);
    void randomize();
    void mutate(qreal chance);
    qreal getfitness();
    QString getinfo();
    void calculate();
    void showfullinfo();

  public:
    qreal a, b;
    //static QString name = ;
  private:
    qreal square, perimeter;
    static const qreal min = 60, max = 180;
};

#endif // SQUARE_H
