#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow *wnd;

void run(quint32 generation)
{
  int i, u, g;
  QTime t;
  t.start();

  while (wnd->work)
    {
      generation++;

      if (RNG::getreal() < wnd->new_probability)
        {
          Population::create();
        }

      for (i = Population::count(); i < wnd->maxpop; i++)
        {
          u = RNG::getint(0, Population::count());
          g = RNG::getint(0, Population::count(), u);
          Population::inherit(u, g);
        }

      for (i = 0; i < wnd->maxpop; i++)
        {
          Population::mutate(i, wnd->mutation_probability);
        }

      Population::sort();
      Population::shrink(wnd->minpop);

      if (t.elapsed() > 1000)
        {
          wnd->SetProgressInfo(generation, true);
          t.start();
        }
    }

  while (t.elapsed() < 500) {}

  wnd->SetProgressInfo(generation, true);
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  ui->setupUi(this);
  mutation_probability = 0.025000;
  new_probability = 0.005000;
  minpop = 10;
  maxpop = 50;
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::SetProgressInfo(quint32 value, bool updateinfo)
{
  this->ui->label_GenerationNumber->setText(QString::number(value));

  if (updateinfo)
    {
      this->ui->list_Creatures->clear();
      QStringList *lptr = Population::getstrings();
      this->ui->list_Creatures->addItems(*lptr);
      delete lptr;
    }
}

void MainWindow::closeEvent(QCloseEvent *ce)
{
  if (work)
    {
      ce->ignore();
      this->ui->action_Stop->trigger();
    }
}

void MainWindow::on_spin_PopulationMin_valueChanged(int value)
{
  ui->spin_PopulationMax->setMinimum(value + 1);
  minpop = value;
}

void MainWindow::on_action_Run_triggered()
{
  this->work = true;
  this->ui->action_Run->setDisabled(true);
  this->ui->action_Reset->setDisabled(true);
  this->ui->action_Stop->setEnabled(true);
  this->ui->list_Creatures->clearSelection();
  QtConcurrent::run(run, this->ui->label_GenerationNumber->text().toInt());
}

void MainWindow::on_action_Stop_triggered()
{
  if (work)
    {
      this->ui->list_Creatures->clear();
      work = false;
    }

  this->ui->action_Run->setEnabled(true);
  this->ui->action_Reset->setEnabled(true);
  this->ui->action_Stop->setDisabled(true);
}

void MainWindow::on_action_Reset_triggered()
{
  this->ui->label_GenerationNumber->setText("0");
  this->ui->list_Creatures->clear();
  Population::clear();
  Population::create(this->ui->spin_PopulationMax->value());
  this->ui->list_Creatures->clear();
  QStringList *lptr = Population::getstrings();
  this->ui->list_Creatures->addItems(*lptr);
}

void MainWindow::on_action_openCreature_triggered()
{
  wnd = this;

  if ((creature_library = QFileDialog::getOpenFileName(this, "Открыть", NULL, tr("?? (*.dll1)"))) != "")
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
  if (work)
    { return; }

  QStringList *strs = Population::showinfo(index.row());
  QString message;

  for (int i = 0; i < strs->count(); i++)
    {
      message += strs->at(i) + '\n';
    }

  QMessageBox(QMessageBox::Information, "I", message, QMessageBox::Ok).exec();
  delete strs;
}

void MainWindow::on_Spin_MutationChance_valueChanged(double arg1)
{
  mutation_probability = arg1;
}

void MainWindow::on_Spin_NewChance_valueChanged(double arg1)
{
  new_probability = arg1;
}

void MainWindow::on_spin_PopulationMax_valueChanged(int arg1)
{
  maxpop = arg1;
}
