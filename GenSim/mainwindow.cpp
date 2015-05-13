#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *wnd;

bool cmp(Creature *i, Creature *j) { return (i->fitness > j->fitness); }

void run(quint32 generation, int minpop, int maxpop)
{
  int i, u, g;
  int divider = 1;
  QTime t;
  t.start();

  while (wnd->work)
    {
      generation++;
      std::sort(wnd->population->creatures.begin(), wnd->population->creatures.end(), cmp);

      if (RNG::getreal() < wnd->new_probability)
        {
          wnd->population->randomize(minpop - 1);
        }

      for (i = minpop; i < maxpop; i++)
        {
          u = RNG::getint(0, minpop);
          g = RNG::getint(0, minpop, u);
          wnd->population->inherit(i, u, g);
          wnd->population->mutate(i, wnd->mutation_probability);
        }

      wnd->population->calculate();

      if (generation % divider == 0)
        {
          wnd->SetProgressInfo(generation, generation % (divider * 2) == 0, minpop);

          if (t.elapsed() < 250)
            {
              divider *= 2;
            }
          else
            if (t.elapsed() > 3000)
              {
                divider /= (divider > 1) ? 2 : 1;
              }

          t.start();
        }
    }

  while (t.elapsed() < 500) {}

  wnd->SetProgressInfo(generation, true, minpop);
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::SetProgressInfo(quint32 value, bool updateinfo, int count)
{
  this->ui->label_GenerationNumber->setText(QString::number(value));

  if (updateinfo)
    {
      this->ui->list_Creatures->clear();
      this->ui->list_Creatures->addItems(population->getinfo(count));
    }

  if (value > 100000)
    { this->ui->action_Stop->trigger(); }
}

void MainWindow::on_spin_PopulationMin_valueChanged(int value)
{
  this->ui->spin_PopulationMax->setMinimum(value + 1);
}

void MainWindow::on_action_Run_triggered()
{
  population->fill(this->ui->spin_PopulationMax->value());
  this->work = true;
  this->ui->action_Run->setDisabled(true);
  this->ui->action_Reset->setDisabled(true);
  this->ui->action_Stop->setEnabled(true);
  this->ui->list_Creatures->clearSelection();
  QtConcurrent::run(run, this->ui->label_GenerationNumber->text().toInt(),
                    this->ui->spin_PopulationMin->value(),
                    this->ui->spin_PopulationMax->value());
}

void MainWindow::on_action_Stop_triggered()
{
  if (work)
    {
      work = false;
      this->ui->list_Creatures->clear();
    }

  this->ui->action_Run->setEnabled(true);
  this->ui->action_Reset->setEnabled(true);
  this->ui->action_Stop->setDisabled(true);
}

void MainWindow::on_action_Reset_triggered()
{
  this->ui->label_GenerationNumber->setText("0");
  this->ui->list_Creatures->clear();
  population->fill(this->ui->spin_PopulationMax->value());

  for (int i = 0; i < population->creatures.size(); i++)
    {
      population->randomize(i);
    }

  population->calculate();
  this->ui->list_Creatures->addItems(population->getinfo(this->ui->spin_PopulationMax->value()));
}

void MainWindow::on_action_openCreature_triggered()
{
  wnd = this;
  int i;

  if ((creature_library = QFileDialog::getOpenFileName(this, "Открыть", NULL, tr("?? (*.dll)"))) != "")
    {
      if (loader)
        {
          //Delete all Creatures
          for (i = 0; i < population->creatures.size(); i++)
            { delete population->creatures[i]; }

          population->creatures.clear();
          loader->unload();
          delete loader;
        }

      loader = new QPluginLoader(creature_library);
      QObject *plugin = loader->instance();

      if (plugin)
        {
          // Now create new population
          int count = this->ui->spin_PopulationMax->value();
          population = qobject_cast<IPopulation *>(plugin);
          population->fill(count);
          this->on_action_Reset_triggered();
          this->ui->action_Run->setEnabled(true);
          this->ui->action_Reset->setEnabled(true);
        }
      else
        {
          QMessageBox(QMessageBox::Warning, "Error", "Not plugin!", QMessageBox::Ok, this).exec();
        }
    }
}

void MainWindow::on_list_Creatures_doubleClicked(const QModelIndex &index)
{
  population->showfullinfo(index.row());
}

void MainWindow::on_Spin_MutationChance_valueChanged(double arg1)
{
  mutation_probability = arg1;
}

void MainWindow::on_Spin_NewChance_valueChanged(double arg1)
{
  new_probability = arg1;
}

