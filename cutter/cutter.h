#ifndef CUTTER_H
#define CUTTER_H

#include "../icreature.h"
#include "descr.h"

class cutter : public QObject, public ICreature {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "DirectoriX.UIR.Creature" FILE "cutter.json")
    Q_INTERFACES(ICreature)

  public:
    cutter();
    ~cutter();

    quint32 paramCount();
    QString name();
    bool threadable();
    bool isSimpleMutation();
    void mutate(bool onlyOnce, qreal probability);
    bool isSimpleCrossover();
    void crossover(ICreature *p1, ICreature *p2);
    ICreature *create();
    void initDoubleParam(quint32 number);
    void calculate();
    void prepare();
    void clean();
    QDialog *getInfoWindow();
    void updateInfoWindow();

    QVector<qint32> details;

  signals:
    void setFieldSize(qreal width, qreal height);
    void setRects(QVector<detail> *details, qreal w, qreal h, qreal freex, qreal freey);

  private:
    bool addRect(qint32 type);

    QVector<place> places;
    QVector<detail> list;
    qreal totalwidth, totalheight, freex, freey;
};

#endif // CUTTER_H
