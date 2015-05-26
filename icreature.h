#ifndef GENINTERFACES
#define GENINTERFACES

#include <QString>
#include <QVector>
#include <QStringList>
/// NOTE #include <QVariant> - for future development

class ICreature {
  public:
    int paramcount;
    QString name;
    QStringList paramnames;

    qreal fitness;
    QVector<double> doubleparams;

    virtual ~ICreature() {}

    // Create random new ICreature
    virtual ICreature *create(void) = 0;

    // Set parameter to random value
    virtual void initdoubleparam(int number) = 0;

    // Calculate fitness
    virtual void calculate(void) = 0;

    // For some preparation work, called ony once
    virtual void prepare(void) = 0;

    /// TODO more features
    ///static const bool threadable;
    ///QVector<QVariant> values_other;
};

#define Creature_iid "DirectoriX.UIR.Creature"

Q_DECLARE_INTERFACE(ICreature, Creature_iid)

#endif // GENINTERFACES

