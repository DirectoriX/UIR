#ifndef SQUARE_H
#define SQUARE_H

#include <QObject>
#include <QtPlugin>
#include <QtMath>
#include <QMessageBox>
#include <../GenClasses.h>


class Square : public QObject, public IPopulation {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "DirectoriX.UIR.Population" FILE "Demo-Square.json")
    Q_INTERFACES(IPopulation)

    class param: public Creature {
      public:
        double a, b;
        qreal square, perimeter;
    };

  private:
    static const qreal min = 60, max = 180;

    // IPopulation interface
  public:
    void inherit(int result, int parent1, int parent2);
    void fill(int count);
    void randomize(int number);
    void mutate(int number, qreal chance);
    void sort();
    QStringList getinfo(int count);
    void showfullinfo(int number);
    void calculate();
    void calculateAt(Creature *&c);
};

#endif // SQUARE_H
