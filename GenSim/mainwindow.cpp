#include "mainwindow.h"
#include "ui_mainwindow.h"

#if defined(Q_OS_WIN)
#  define extlib "*.dll"
#elif defined(Q_OS_LINUX)
#  define extlib "*.so"
#else
#  define extlib "*.!!"
#endif


MainWindow *wnd;

void run(quint32 generation)
{
  qint32 i, u, g;
  QTime t;
  t.start();
  qint32 maxpop, minpop, minpop_clear;
  qreal new_probability, mutation_probability;

  while (wnd->work)
    {
      maxpop = wnd->maxpop;
      minpop = minpop_clear = wnd->minpop;
      new_probability = wnd->new_probability;
      mutation_probability = wnd->mutation_probability;
      generation++;

      if (RNG::getreal() < new_probability)
        {
          Population::create();
          minpop++;
        }

      for (i = Population::count(); i < maxpop; i++)
        {
          u = RNG::getint(0, minpop);
          g = RNG::getint(0, minpop, u);
          Population::crossover(u, g);
        }

      for (i = 1; i < maxpop; i++)
        {
          Population::mutate(i, mutation_probability);
        }

      Population::select();
      Population::shrink(minpop_clear);

      if (t.elapsed() > 1000)
        {
          wnd->SetProgressInfo(generation);
          t.start();
        }
    }

  while (t.elapsed() < 500) {}

  wnd->SetProgressInfo(generation);
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  mutation_probability = 0.1;
  new_probability = 0.5;
  minpop = 10;
  maxpop = 50;
  work = false;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::SetProgressInfo(quint32 value)
{
  ui->label_GenerationNumber->setText(QString::number(value));
  ui->list_Creatures->clear();
  QStringList *lptr = Population::getstrings();
  ui->list_Creatures->addItems(*lptr);
  delete lptr;
}

void MainWindow::closeEvent(QCloseEvent *ce)
{
  if (work)
    {
      ce->ignore();
      ui->action_Stop->trigger();
    }
}

void MainWindow::on_spin_PopulationMin_valueChanged(qint32 value)
{
  ui->spin_PopulationMax->setMinimum(value + 1);
  minpop = value;
}

void MainWindow::on_action_Run_triggered()
{
  work = true;
  ui->action_Run->setDisabled(true);
  ui->action_Reset->setDisabled(true);
  ui->action_Stop->setEnabled(true);
  ui->list_Creatures->clearSelection();
  QtConcurrent::run(run, this->ui->label_GenerationNumber->text().toInt());
}

void MainWindow::on_action_Stop_triggered()
{
  if (work)
    {
      ui->list_Creatures->clear();
      work = false;
    }

  ui->action_Run->setEnabled(true);
  ui->action_Reset->setEnabled(true);
  ui->action_Stop->setDisabled(true);
}

void MainWindow::on_action_Reset_triggered()
{
  ui->label_GenerationNumber->setText("0");
  ui->list_Creatures->clear();
  Population::clear();
  Population::create(this->ui->spin_PopulationMax->value());
  ui->list_Creatures->clear();
  QStringList *lptr = Population::getstrings();
  ui->list_Creatures->addItems(*lptr);
  delete lptr;
}

void MainWindow::on_action_openCreature_triggered()
{
  wnd = this;

  QString ext;
  ext=extlib;

  if ((creature_library = QFileDialog::getOpenFileName(this, "Открыть", NULL, "?? ("+ext+")")) != "")
    {
      if (loader)
        {
          //Delete all Creatures
          Population::clear();
          loader->unload();
          delete loader;
        }

      loader = new QPluginLoader(creature_library);
      QObject *plugin = loader->instance();

      if (plugin)
        {
          // Now create new population
          Population::setroot(qobject_cast<ICreature *>(plugin));
          ui->label_CreatureName->setText(Population::getName());
          on_action_Reset_triggered();
          ui->action_Run->setEnabled(true);
          ui->action_Reset->setEnabled(true);
        }
      else
        {
          QMessageBox(QMessageBox::Warning, "Error", "Not plugin!", QMessageBox::Ok, this).exec();
        }
    }
}

void MainWindow::on_list_Creatures_doubleClicked(const QModelIndex &index)
{
  if (work)
    { return; }

  QStringList *strs = Population::showinfo(index.row());
  QString message;

  for (qint32 i = 0; i < strs->count(); i++)
    {
      message += strs->at(i) + '\n';
    }

  QMessageBox(QMessageBox::Information, "Info", message, QMessageBox::Ok).exec();
  delete strs;
}

void MainWindow::on_Spin_MutationChance_valueChanged(qreal arg1)
{
  mutation_probability = arg1;
}

void MainWindow::on_Spin_NewChance_valueChanged(qreal arg1)
{
  new_probability = arg1;
}

void MainWindow::on_spin_PopulationMax_valueChanged(qint32 arg1)
{
  maxpop = arg1;
}

