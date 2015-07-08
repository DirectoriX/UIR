#ifndef POPULATION_H
#define POPULATION_H

#include "../icreature.h"
#include "../rng.h"
#include <QVector>
#include <QtConcurrent/QtConcurrent>
#include <QCloseEvent>

// Main class for genetic algorithm
class Population {
  public:
    // Create new ICreature with random parameters
    static void create(void);

    // Create k new ICreatures with random parameters
    static void create(qint32 k);

    // Create child ICreature from two parents
    static void crossover(qint32 parent1, qint32 parent2);

    // Change some parameters with given probability
    static void mutate(qint32 number, qreal probability);

    // Select best creatures
    static void select(void) ;

    // Show information about creature
    static QStringList *showinfo(qint32 number);

    // Get short info for all ICreatures
    static QStringList *getstrings(void);

    // Delete not needed ICreatures
    static void shrink(qint32 newcount);

    // Delete ALL ICreatures
    static void clear(void);

    // Set new root ICreature
    static void setroot(ICreature *newroot);

    // Get ICreatures count
    static qint32 count(void);

    // Get ICreature name
    static QString getName(void);
};

#endif // POPULATION_H
