#ifndef GENINTERFACES
#define GENINTERFACES

#include <QString>
#include <QVector>
#include <QStringList>
#include <QtPlugin>

class ICreature {
  public:
    qint32 paramcount;
    QString name;
    QStringList paramnames;

    qreal fitness;
    QVector<double> doubleparams;

    virtual ~ICreature() {}

    // Create random new ICreature
    virtual ICreature *create(void) = 0;

    // Set parameter to random value
    virtual void initdoubleparam(qint32 number) = 0;

    // Calculate fitness
    virtual void calculate(void) = 0;

    // For some preparation work, called ony once
    virtual void prepare(void) = 0;
};

#define Creature_iid "DirectoriX.UIR.Creature"

Q_DECLARE_INTERFACE(ICreature, Creature_iid)

#endif // GENINTERFACES

