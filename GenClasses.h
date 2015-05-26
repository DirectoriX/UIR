#ifndef GENINTERFACES
#define GENINTERFACES

#include <QString>
#include <QVector>
#include <QVariant>
#include <QDateTime>

class Creature {
  public:
    qreal fitness;
};

// Interface for genetic simulation algorithm
class IPopulation {
  public:
    // Used to obtain Population name
    /// NOTE Thus must be changed in every inherited class!
    static QString name;
    QVector<Creature*> creatures;

    virtual ~IPopulation() {}

    // Create child ICreature from two parents
    virtual void inherit(int result, int parent1, int parent2) = 0;

    // Make creatures up to count
    virtual void fill(int count) = 0;

    // Set all parameters to random values
    virtual void randomize(int number) = 0;

    // Change some parameters with given chance
    virtual void mutate(int number, qreal chance) = 0;

    // Sort creatures
    virtual void sort(void) = 0;

    // Get short info about population
    virtual QStringList getinfo(int count) = 0;

    // Show full information
    // powered by plug-in
    virtual void showfullinfo(int number) = 0;

    // Calculate fitness here
    // This function must present for some reasons - i.e. Creatures can "fight" with each other to obtain fitness value
    virtual void calculate(void) = 0;

    // Calculate fitness here
    // This function must present for some reasons - i.e. Creatures can "fight" with each other to obtain fitness value
    virtual void calculateAt(Creature *&c) = 0;
};

class RNG {
  public:
    static int getint(int min, int max);
    static int getint(int min, int max, int excluded);
    static qreal getreal();
    static qreal getreal(qreal min, qreal max);
};

#define Population_iid "DirectoriX.UIR.Population"

Q_DECLARE_INTERFACE(IPopulation, Population_iid)

#endif // GENINTERFACES

