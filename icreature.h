#ifndef GENINTERFACES
#define GENINTERFACES

#include <QString>
#include <QVector>
#include <QStringList>
#include <QtPlugin>
#include <QDialog>

class ICreature {
  public:
    // Some information methods
    virtual quint32 paramCount() = 0;
    virtual QString name() = 0;
    virtual bool threadable() = 0;

    // Custom mutation mechanism
    virtual bool isSimpleMutation() = 0;
    virtual void mutate(bool onlyOnce, qreal probability) = 0;

    // Custom crossover mechanism
    virtual bool isSimpleCrossover() = 0;
    virtual void crossover(ICreature *p1, ICreature *p2) = 0;

    qreal fitness;
    qreal *realParams;

    virtual ~ICreature() {}

    // Create random new ICreature
    virtual ICreature *create() = 0;

    // Set parameter to random value
    virtual void initDoubleParam(quint32 number) = 0;

    // Calculate fitness
    virtual void calculate() = 0;

    // For some preparation work, called only once
    virtual void prepare() = 0;

    // For some cleaning work, called only once
    virtual void clean() = 0;

    // Construct information window
    virtual QDialog *getInfoWindow() = 0;

    // Update shown information
    virtual void updateInfoWindow() = 0;
};

#define Creature_iid "DirectoriX.UIR.Creature"

Q_DECLARE_INTERFACE(ICreature, Creature_iid)

#endif // GENINTERFACES

