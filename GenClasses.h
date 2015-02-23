#ifndef GENINTERFACES
#define GENINTERFACES

#include <QString>
#include <QVector>
#include <QVariant>
#include <QDateTime>

// Interface for genetic simulation algorithm
class ICreature {
  protected:
    qreal fitness;
    /// NOTE use this fitness with operator< to sort
  public:
    // Used to obtain ICreature name
    /// NOTE Thus must be changed in every inherited class!
    static QString name;

    virtual ~ICreature() {}

    // Create child ICreature from two parents
    virtual void inherit(ICreature *parent1, ICreature *parent2) = 0;

    // Set all parameters to random values
    // This operatopn equals to creating new ICreature object
    virtual void randomize(void) = 0;

    // Change some parameters with given chance
    virtual void mutate(qreal chance) = 0;

    // Get fitness value
    virtual qreal getfitness(void) = 0;
    /// TODO: operator< instead of getfitness
    /// virtual bool operator<(ICreature *other)=0;

    // Get short info about ICreature
    virtual QString getinfo(void) = 0;

    // Show full information
    // powered by plug-in
    virtual void showfullinfo() = 0;

    // Calculate fitness here
    // This function must present for some reasons - i.e. Creatures can "fight" with each other to obtain fitness value
    virtual void calculate(void) = 0;
};

class NeuroInput {
  public:
    QString filename;
    virtual bool load() = 0;
    virtual bool load(QString filename) = 0;
};

class NeuroResult {
  public:
    virtual bool save(QString filename) = 0;
};

// Interface for neural networks
class INeuroNet {
  public:
    virtual ~INeuroNet() {}

    virtual NeuroResult *analyze(NeuroInput *input) = 0;

    virtual void learn(NeuroInput *input, NeuroResult *result) = 0;

    virtual void save(QString filename) = 0;

    virtual bool load(QString filename) = 0;
};

class RNG {
  public:
    static int getint(int min, int max);
    static int getint(int min, int max, int excluded);
    static qreal getreal();
    static qreal getreal(qreal min, qreal max);
};

#define Creature_iid "DirectoriX.UIR.Creature"

Q_DECLARE_INTERFACE(ICreature, Creature_iid)

#define NeuroNet_iid "DirectoriX.UIR.NeuroNet"

Q_DECLARE_INTERFACE(INeuroNet, NeuroNet_iid)

#endif // GENINTERFACES

