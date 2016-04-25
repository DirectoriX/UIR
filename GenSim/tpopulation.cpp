#include "tpopulation.h"

ICreature *root;
quint32 paramCount, stopGen, stopTime;
bool simpleMutation, simpleCrossover, increase, stopG, stopT, stopF;
qreal stopFt;
QDialog *wnd;

const qreal maxDifference = 0.0015;

///FIXME Commentaries!

bool cmpi(ICreature *i, ICreature *j) { return (i->fitness > j->fitness); }

bool cmpd(ICreature *i, ICreature *j) { return (i->fitness < j->fitness); }

void calc(ICreature *&c)
{
  if (qIsNaN(c->fitness))
    { c->calculate(); }
}

TPopulation::TPopulation(QObject *parent): QThread(parent)
{
  QThreadPool::globalInstance()->reserveThread();
  qsrand(QTime::currentTime().msec());
  abort = false;
  reconfigure = stop = stopRequested = true;
  elapsedTime = generation = 0;
  this->w = wnd;
  root = NULL;
  QThreadPool::globalInstance()->setMaxThreadCount(QThread::idealThreadCount() - 1);
}

TPopulation::~TPopulation()
{
  mutex.lock();
  abort = true;
  values.clear();
  clear();
  condition.wakeOne();
  mutex.unlock();
  wait();
  QThreadPool::globalInstance()->releaseThread();
}

void TPopulation::updateSettings(const settings &s, bool isReset, bool resetTime)
{
  this->s = s;
  mutex.lock();
  reconfigure = true;

  if (isReset)
    {
      generation = 0;
      clear();
      divider = 1;

      if (resetTime)
        { elapsedTime = 0; }
    }

  mutex.unlock();
}

void TPopulation::requestUpdate()
{
  values.clear();
  mutex.lock();

  for (i = 0; i < creatures.count(); i++)
    {
      values.append(creatures[i]->fitness) ;
    }

  mutex.unlock();

  if (!creatures.empty() && w->isVisible())
    { creatures.first()->updateInfoWindow(); }

  emit updateView(generation, elapsedTime, values);
}

void TPopulation::requestStop()
{
  stop = true;
  stopRequested = true;
}

void TPopulation::requestContinue()
{
  t.start();
  condition.wakeOne();
}

void TPopulation::requestAbort()
{
  abort = true;

  if (stop)
    { condition.wakeOne(); }
}

void TPopulation::requestFullInfo(quint32 index)
{
  creatures[index]->updateInfoWindow();
}

void TPopulation::setStopConditions(bool sgen, quint32 gen, bool stime, const QTime &time, bool sf, qreal f)
{
  stopG = sgen;
  stopT = stime;
  stopF = sf;
  stopGen = gen;
  stopTime = time.hour() * 60 * 60 * 1000;
  stopTime += time.minute() * 60 * 1000;
  stopTime += time.second() * 1000;
  stopFt = f;
}

QString TPopulation::setRoot(ICreature *newRoot)
{
  clear();
  root = newRoot;
  root->prepare();
  paramCount = root->paramCount();
  wnd = w = root->getInfoWindow();
  simpleCrossover = root->isSimpleCrossover();
  simpleMutation = root->isSimpleMutation();
  increase = true;
  return root->name();
}

qreal TPopulation::difference(qint32 c1, qint32 c2)
{
  qreal result = 0;

  for (quint32 i = 0; i < paramCount; i++)
    {
      result += qPow((creatures[c1]->realParams[i] - creatures[c2]->realParams[i]), 2);
    }

  return result;
}

void TPopulation::crossover(quint32 parent1, quint32 parent2)
{
  ICreature *last, *p1, *p2;
  creatures.append(last = root->create());
  p1 = creatures[parent1];
  p2 = creatures[parent2];

  for (quint32 i = 0; i < paramCount; i++)
    { last->realParams[i] = (RNG::getreal() < 0.5) ? p1->realParams[i] : p2->realParams[i]; }

  last->fitness = NAN;
}

void TPopulation::clear()
{
  foreach (ICreature *c, creatures)
    {
      delete c;
    }

  creatures.clear();
}

void TPopulation::run()
{
  quint32 populationMax, populationMin;
  qreal probabilityMutation;
  bool selectFromNew, mutateOnce, createNew, clear, threadable;
  populationMax = 50;
  populationMin = 10;
  probabilityMutation = 0.1;
  selectFromNew = false;
  mutateOnce = false;
  createNew = true;
  clear = true;
  threadable = true;
  t.start();

  forever
    {
      mutex.lock();

      if (stop)
        {
          emit stopped(stopRequested);
          condition.wait(&mutex);
          stop = false;
        }

      if (abort)
        {
          mutex.unlock();
          return;
        }

      if (reconfigure)
        {
          populationMax = s.populationMax;
          populationMin = s.populationMin;
          probabilityMutation = s.probabilityMutation;
          selectFromNew = s.selectFromNew;
          mutateOnce = s.mutateOnce;
          createNew = s.createNew;
          clear = s.clear;
          threadable = s.threadable && root->threadable();
          reconfigure = false;
          increase = s.increase;
        }

      if (clear && ((generation % 10) == 1))
        {
          quint32 c = creatures.count();
          //if ((creatures[0]->fitness / creatures[c / 2]->fitness) > 0.99995 &&
          //    (creatures[c / 2]->fitness / creatures[0]->fitness) > 0.99995)
          {
            for (ui = 0; ui < c; ui++)
              {
                if (qIsNaN(creatures[ui]->fitness))
                  { continue; }

                for (quint32 j = ui + 1; j < c; j++)
                  {
                    if (qIsNaN(creatures[j]->fitness))
                      { continue; }

                    if (
                      qFuzzyCompare(creatures[ui]->fitness, creatures[j]->fitness) ||
                      (difference(ui, j) < maxDifference))
                      { creatures[j]->fitness = NAN; }
                  }
              }

            for (ui = c - 1; ui > 0; ui--)
              {
                if (qIsNaN(creatures[ui]->fitness))
                  {
                    delete creatures[ui];
                    creatures.removeAt(ui);
                  }
              }
          }
        }

      generation++;

      for (ui = creatures.count(); ui < populationMin; ui++)
        {
          creatures.append(root->create());
          creatures.last()->fitness = NAN;
        }

      if (createNew)
        {
          creatures.append(root->create());
          creatures.last()->fitness = NAN;
        }

      quint32 c1, c2;

      if (selectFromNew)
        {
          quint32 count;

          for (ui = creatures.count(); ui < populationMax; ui++)
            {
              count = creatures.count();
              c1 = RNG::getint(0, count);
              c2 = RNG::getint(0, count, c1);

              if (simpleCrossover)
                { crossover(c1, c2); }
              else
                {
                  creatures.append(root->create());
                  creatures.last()->crossover(creatures[c1], creatures[c2]);
                  creatures.last()->fitness = NAN;
                }
            }
        }
      else
        {
          quint32 upper = creatures.count();

          for (ui = creatures.count(); ui < populationMax; ui++)
            {
              c1 = RNG::getint(0, upper);
              c2 = RNG::getint(0, upper, c1);

              if (simpleCrossover)
                { crossover(c1, c2); }
              else
                {
                  creatures.append(root->create());
                  creatures.last()->crossover(creatures[c1], creatures[c2]);
                  creatures.last()->fitness = NAN;
                }
            }
        }

      for (i = 1; i < creatures.count(); i++)
        {
          if (simpleMutation)
            if (mutateOnce)
              {
                if (RNG::getreal() < probabilityMutation)
                  {
                    creatures[i]->initDoubleParam(RNG::getint(0, paramCount - 1));
                    creatures[i]->fitness = NAN;
                  }
              }
            else
              {
                for (quint32 j = 0; j < paramCount; j++)
                  {
                    if (RNG::getreal() < probabilityMutation)
                      {
                        creatures[i]->initDoubleParam(j);
                        creatures[i]->fitness = NAN;
                      }
                  }
              }
          else
            {
              creatures[i]->mutate(mutateOnce, probabilityMutation);
              creatures[i]->fitness = NAN;
            }
        }

      if (threadable)
        {
          QtConcurrent::blockingMap(creatures, calc);
        }
      else
        {
          for (ui = 0; ui < populationMax; ui++)
            {
              if (qIsNaN(creatures[ui]->fitness))
                { creatures[ui]->calculate(); }
            }
        }

      if (increase)
        { std::sort(creatures.begin(), creatures.end(), cmpi); }
      else
        { std::sort(creatures.begin(), creatures.end(), cmpd); }

      for (ui = populationMax - 1; ui >= populationMin; ui--)
        {
          delete creatures[ui];
          creatures.removeLast();
        }

      if (generation % divider == 0)
        {
          if (generation % (divider * 10) == 0)
            { divider *= 10; }

          emit addPoint(generation, creatures[0]->fitness);
        }

      mutex.unlock();

      if ((stopG && generation >= stopGen) ||
          (stopT && stopTime <= elapsedTime) ||
          (stopF &&
           ((increase && (creatures.first()->fitness >= stopFt)) ||
            (!increase && (creatures.first()->fitness <= stopFt)))
          ))
        {
          stop = true;
          stopRequested = false;
        };

      if ((et = t.elapsed()) > 500)
        {
          elapsedTime += et;
          requestUpdate();
          t.start();
        }
    }
}

