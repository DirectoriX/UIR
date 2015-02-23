#include "mainwindow.h"
#include "ui_mainwindow.h"

void run(MainWindow *window, quint32 generation, int minpop, int maxpop, qreal mutation_probability)
{
  while (window->work)
    {
      generation++;

      for (int i = 0; i < INT_MAX / 1000; i++) {}

      window->SetProgress(generation);
    }
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

void MainWindow::SetProgress(quint32 value)
{
  this->ui->label_GenerationNumber->setText(QString::number(value));
}

void MainWindow::on_spin_PopulationMin_valueChanged(int value)
{
  this->ui->spin_PopulationMax->setMinimum(value + 1);
}

void MainWindow::on_action_Run_triggered()
{
  this->work = true;
  this->ui->action_Run->setDisabled(true);
  this->ui->action_Reset->setDisabled(true);
  this->ui->action_Stop->setEnabled(true);
  QFuture<void> fut = QtConcurrent::run(run, this, this->ui->label_GenerationNumber->text().toInt(), 1, 2, 3.4);
}

void MainWindow::on_action_Stop_triggered()
{
  work = false;
  this->ui->action_Run->setEnabled(true);
  this->ui->action_Reset->setEnabled(true);
  this->ui->action_Stop->setDisabled(true);
}

void MainWindow::on_action_Reset_triggered()
{
  this->ui->label_GenerationNumber->setText("0");
  this->ui->list_Creatures->clear();

  for (int i = 0; i < population.size(); i++)
    {
      population[i]->randomize();
      population[i]->calculate();
      this->ui->list_Creatures->addItem(population[i]->getinfo());
    }
}

void MainWindow::on_action_openCreature_triggered()
{
  int i;
  creature_library = QFileDialog::getOpenFileName(this, "Открыть", NULL, tr("?? (*.dll)"));
  QPluginLoader loader(creature_library);
  QObject *plugin = loader.instance();

  if (plugin)
    {
      //Delete all Creatures
      for (i = 0; i < population.size(); i++)
        { delete population[i]; }

      population.clear();
      delete &population;
      // Now create new population
      int count = this->ui->spin_PopulationMax->value();
      population = *(new QVector<ICreature *>(count, NULL));
      population[0] = qobject_cast<ICreature *>(plugin);

      for (i = 1; i < count; i++)
        {
          population[i] = qobject_cast<ICreature *>(loader.instance());
        }

      this->on_action_Reset_triggered();
      this->ui->action_Run->setEnabled(true);
    }
  else
    {
      QMessageBox(QMessageBox::Warning, "Error", "Not plugin!", QMessageBox::Ok, this).exec();
    }
}
