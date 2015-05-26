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
    // Base ICreature
   // static ICreature *root;

    // Create new ICreature with random parameters
    static void create(void);

    // Create k new ICreatures with random parameters
    static void create(int k);

    // Create child ICreature from two parents
    static void inherit(int parent1, int parent2);

    // Change some parameters with given probability
    static void mutate(int number, double probability);

    // Sort creatures
    static void sort(void) ;

    // Show information about creature
    static QStringList *showinfo(int number) ;

    // Save population into file
    static bool save(QString filename);

    // Load population from file
    static bool load(QString filename);

    // Get short info for all ICreatures
    static QStringList *getstrings(void);

    // Delete not needed ICreatures
    static void shrink(int newcount);

    // Delete ALL ICreatures
    static void clear(void);

    // Set new root ICreature
    static void setroot(ICreature *newroot);

    // Get ICreatures count
    static int count(void);

  private:
    // Actual sets of some parameters and their efficiency
  //  static QVector<ICreature *> creatures;
};

#endif // POPULATION_H
