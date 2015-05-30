#include "population.h"

QVector<ICreature *> creatures;
ICreature *root;

int minsize;

int ic;

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

void Population::create(int k)
{
  for (ic = 0; ic < k; ic++)
    { create(); }
}

void Population::inherit(int parent1, int parent2)
{
  ICreature *last, * p1, *p2;
  creatures.append(last = root->create());
  p1 = creatures[parent1];
  p2 = creatures[parent2];

  for (ic = 0; ic < root->paramcount; ic++)
    { last->doubleparams[ic] = (RNG::getreal() < 0.5) ? p1->doubleparams[ic] : p2->doubleparams[ic]; }
}

void Population::mutate(int number, double probability)
{
  ICreature *cr = creatures[number];

  for (ic = 0; ic < root->paramcount; ic++)
    if (RNG::getreal() < probability)
      { cr->initdoubleparam(ic); }
}

void Population::sort()
{
  for (int i = minsize; i < creatures.count(); i++)    { creatures[i]->calculate(); }

  //  QtConcurrent::blockingMap(creatures, call);
  std::sort(creatures.begin(), creatures.end(), cmp);
}

QStringList *Population::showinfo(int number)
{
  ICreature *cr = creatures[number];
  QStringList *result = new QStringList(root->paramnames);

  for (ic = 0; ic < root->paramcount; ic++)
    {
      (*result)[ic] += " " + QString::number(cr->doubleparams[ic]);
    }

  return result;
}

bool Population::save(QString filename)
{
  QFile file(filename);

  if (!file.open(QIODevice::WriteOnly))
    { return false; }

  QDataStream out(&file);
  out << root->name;
  out << creatures.count();
  ICreature *cr;

  for (int i = 0; i < creatures.count(); i++)
    {
      cr = creatures[i];

      for (ic = 0; ic < root->paramcount; ic++)
        {
          out << cr->doubleparams[ic];
        }

      out << cr->fitness;
    }

  file.flush();
  file.close();
  return true;
}

bool Population::load(QString filename)
{
  QFile file(filename);

  if (!file.open(QIODevice::ReadOnly))
    { return false; }

  QDataStream in(&file);
  QString name;
  int count;
  in >> name >> count;

  if (!(name == root->name))
    { return false; }

  clear();
  ICreature *cr;

  for (ic = 0; ic < count; ic++)
    {
      creatures.append(cr = root->create());

      for (int i = 0; i < root->paramcount; i++)
        {
          in >> cr->doubleparams[i];
        }

      in >> cr->fitness;
    }

  return true;
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

void Population::shrink(int newcount)
{
  int count = creatures.count();

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

int Population::count()
{
  return creatures.count();
}
