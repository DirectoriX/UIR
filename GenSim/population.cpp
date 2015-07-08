#include "population.h"

QVector<ICreature *> creatures;
ICreature *root;

qint32 minsize;

qint32 ic;

void call(ICreature *&c)
{
  c->calculate();
}

bool cmp(ICreature *i, ICreature *j) { return (i->fitness > j->fitness); }

void calc(ICreature *cr)
{
  cr->calculate();
}

void Population::create()
{
  creatures.append(root->create());
  creatures.last()->fitness = 0;
}

void Population::create(qint32 k)
{
  for (ic = 0; ic < k; ic++)
    { create(); }
}

void Population::crossover(qint32 parent1, qint32 parent2)
{
  ICreature *last, * p1, *p2;
  creatures.append(last = root->create());
  p1 = creatures[parent1];
  p2 = creatures[parent2];

  for (ic = 0; ic < root->paramcount; ic++)
    { last->doubleparams[ic] = (RNG::getreal() < 0.5) ? p1->doubleparams[ic] : p2->doubleparams[ic]; }
}

void Population::mutate(qint32 number, qreal probability)
{
  ICreature *cr = creatures[number];

  for (ic = 0; ic < root->paramcount; ic++)
    if (RNG::getreal() < probability)
      { cr->initdoubleparam(ic); }
}

void Population::select()
{
  for (qint32 i = minsize; i < creatures.count(); i++)    { creatures[i]->calculate(); }

  std::sort(creatures.begin(), creatures.end(), cmp);
}

QStringList *Population::showinfo(qint32 number)
{
  ICreature *cr = creatures[number];
  QStringList *result = new QStringList(root->paramnames);

  for (ic = 0; ic < root->paramcount; ic++)
    {
      (*result)[ic] += " " + QString::number(cr->doubleparams[ic]);
    }

  return result;
}

QStringList *Population::getstrings()
{
  QStringList *result = new QStringList();

  for (ic = 0; ic < creatures.count(); ic++)
    {
      result->append(QString::number(creatures[ic]->fitness, 'g', 8)) ;
    }

  return result;
}

void Population::shrink(qint32 newcount)
{
  qint32 count = creatures.count();

  if (count <= newcount)
    { return; }

  for (ic = count - 1; ic >= newcount; ic--)
    {
      delete creatures[ic];
      creatures.removeLast();
    }

  minsize = newcount;
}

void Population::clear()
{
  for (ic = 0; ic < creatures.count(); ic++)
    { delete creatures[ic];}

  creatures.clear();
}

void Population::setroot(ICreature *newroot)
{
  root = newroot;
  root->prepare();
}

qint32 Population::count()
{
  return creatures.count();
}

QString Population::getName()
{
  return root->name;
}
