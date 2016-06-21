#include "mainwindow.h"
#include "ui_mainwindow.h"

// Разрешено ли обрабатывать нажатия кнопок
// Они зачем-то обрабатываются при старте программы...
bool allowed;

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  allowed = false;
  ui->setupUi(this);
  qRegisterMetaType<QList<double> /**/>(); /// NOTE "double" потому, что QMetaType не может использовать "qreal"
  // Связываемся с TPopulation
  connect(&p, SIGNAL(updateView(quint32, quint32, QList<double>)), this, SLOT(updateResults(quint32, quint32, QList<double>)));
  connect(&p, SIGNAL(addPoint(quint32, qreal)), this, SLOT(addPoint(quint32, qreal)));
  connect(&p, SIGNAL(stopped(bool)), this, SLOT(stop(bool)));
  p.start();
  // Настройки по умолчанию
  s.populationMax = 50;
  s.populationMin = 10;
  s.probabilityMutation = 0.1;
  s.selectFromNew = false;
  s.mutateOnce = true;
  s.createNew = true;
  s.clear = true;
  s.threadable = false;
  s.increase = true;
  // Настройки графиков
  curve.setPen(Qt::darkBlue, 2);
  curve.setRenderHint(QwtPlotItem::RenderAntialiased, true);
  curve.setSamples(poly);
  curve.attach(ui->plot);
  histo.setCurveAttribute(QwtPlotCurve::Fitted);
  histo.setCurveFitter(new QwtWeedingCurveFitter());
  histo.setPen(Qt::darkGreen, 2);
  histo.setRenderHint(QwtPlotItem::RenderAntialiased, true);
  histo.setSamples(poly_m);
  histo.attach(ui->plot_m);
  ui->plot->setTitle("f(n)");
  ui->plot_m->setTitle(tr("Разброс результатов"));
  ui->plot_m->setAxisScale(QwtPlot::xBottom, 1, 10, 1);
  ui->plot->setAxisScaleEngine(QwtPlot::xBottom, new QwtLogScaleEngine());
  ui->frame->setVisible(false);
  iw = NULL;
  QwtPlotPicker *picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand, QwtPicker::ActiveOnly, ui->plot->canvas());
  picker->setRubberBandPen(QColor(Qt::darkRed));
  picker->setTrackerPen(QColor(Qt::black));
  picker->setStateMachine(new QwtPickerDragPointMachine());
  QwtPlotGrid *grid = new QwtPlotGrid();
  grid->setMajorPen(QPen(Qt::darkGray, 1));
  grid->setMinorPen(QPen(Qt::lightGray, 1));
  grid->enableXMin(true);
  grid->enableYMin(true);
  grid->attach(ui->plot);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::closeEvent(QCloseEvent *ce)
{
  // При закрытии - явно остановить поток расчёта
  p.requestAbort();

  if (iw) { iw->close(); }

  ce->accept();
}

// \/ ToolButtons

// Открытие файла плагина
void MainWindow::on_toolButton_Open_clicked()
{
  // Расширение файла зависит от ОС, поэтому используем макрос
  QString ext = extlib;

  // Получаем имя файла плагина
  if ((creature_library = QFileDialog::getOpenFileName(this, tr("Открыть"), NULL, "?? (" + ext + ")")) != "")
    {
      // Если уже открывали плагин - чистим
      if (loader)
        {
          p.clear();
          qobject_cast<ICreature *>(loader->instance())->clean();
          loader->unload();
          delete loader;
        }

      loader = new QPluginLoader(creature_library);
      QObject *plugin = loader->instance();

      // Если открыли именно файл плагина, а не случайную библиотеку
      if (plugin)
        {
          // Настраиваем TPopulation
          ui->label_CreatureName->setText(p.setRoot(qobject_cast<ICreature *>(plugin)));
          iw = p.w;
          on_toolButton_Reset_clicked();
          ui->toolButton_Reopen->setEnabled(true);
          ui->toolButton_Run->setEnabled(true);
          ui->toolButton_Reset->setEnabled(true);
          ui->toolButton_Info->setEnabled(true);
          allowed = true;
        }
      else
        {
          QMessageBox(QMessageBox::Warning, tr("Ошибка"), tr("Это не плагин!"), QMessageBox::Ok, this).exec();
          allowed = false;
          ui->toolButton_Reopen->setDisabled(true);
          ui->toolButton_Run->setDisabled(true);
          ui->toolButton_Reset->setDisabled(true);
          ui->toolButton_Info->setDisabled(true);
          iw = NULL;
          delete loader;
          loader = NULL;
          creature_library = "";
        }
    }
}

// Установка новых параметров плагина. Выполняется как переоткрытие файла плагина
void MainWindow::on_toolButton_Reopen_clicked()
{
  ui->list_Creatures->clearSelection();

  if (creature_library == "")
    { return; }

  p.clear();
  qobject_cast<ICreature *>(loader->instance())->clean();
  loader->unload();
  delete loader;
  loader = new QPluginLoader(creature_library);
  QObject *plugin = loader->instance();

  if (plugin)
    {
      // Настраиваем TPopulation
      ui->label_CreatureName->setText(p.setRoot(qobject_cast<ICreature *>(plugin)));
      iw = p.w;
      on_toolButton_Reset_clicked();
      ui->toolButton_Run->setEnabled(true);
      ui->toolButton_Reset->setEnabled(true);
      ui->toolButton_Info->setEnabled(true);
      allowed = true;
    }
  else
    {
      QMessageBox(QMessageBox::Warning, tr("Ошибка"), tr("Это не плагин!"), QMessageBox::Ok, this).exec();
      allowed = false;
      ui->toolButton_Run->setDisabled(true);
      ui->toolButton_Reset->setDisabled(true);
      ui->toolButton_Info->setDisabled(true);
      iw = NULL;
      creature_library = "";
    }
}

// Запуск поиска решения
void MainWindow::on_toolButton_Run_clicked()
{
  ui->toolButton_Open->setDisabled(true);
  ui->toolButton_Reopen->setDisabled(true);
  ui->toolButton_Run->setDisabled(true);
  ui->toolButton_Reset->setDisabled(true);
  ui->toolButton_Stop->setEnabled(true);
  ui->list_Creatures->clearSelection();
  p.requestContinue();
}

// Остановка поиска решения
void MainWindow::on_toolButton_Stop_clicked()
{
  p.requestStop();
  ui->toolButton_Stop->setDisabled(true);
}

// Очистка популяции для поиска решения с самого начала
void MainWindow::on_toolButton_Reset_clicked()
{
  ui->list_Creatures->clearSelection();
  p.updateSettings(s, true);
  p.requestUpdate();
  poly.clear();
  curve.setSamples(poly);
  p.setStopConditions(ui->checkBox_MaxGeneration->isChecked(), ui->spin_MaxGen->value(),
                      ui->checkBox_MaxTime->isChecked(), ui->time->time(),
                      ui->checkBox_BestResult->isChecked(), ui->spin_f->value());
}

// Открытие\закрытие окна с подробной информацией
void MainWindow::on_toolButton_Info_clicked(bool checked)
{
  if (checked)
    { iw->show(); }
  else { iw->close(); }
}
// /\ ToolButtons

// \/ Базовые настройки
void MainWindow::on_spin_PopulationMin_valueChanged(qint32 value)
{
  ui->spin_PopulationMax->setMinimum(value + 2);
  ui->plot_m->setAxisScale(QwtPlot::xBottom, 1, value, 1);
  s.populationMin = value;
  p.updateSettings(s, false);
}

void MainWindow::on_spin_PopulationMax_valueChanged(qint32 arg1)
{
  s.populationMax = arg1;
  p.updateSettings(s, false);
}

void MainWindow::on_spin_MutationChance_valueChanged(qreal arg1)
{
  s.probabilityMutation = arg1;
  p.updateSettings(s, false);
}

// Делим вероятность мутаций на 2
void MainWindow::on_prob_2_clicked()
{
  ui->spin_MutationChance->setValue(ui->spin_MutationChance->value() / 2);
}

// Порядок сортировки
// Ищем максимум или минимум
void MainWindow::on_checkBox_Decrease_clicked(bool checked)
{
  s.increase = !checked;
  p.updateSettings(s, false);
}
// /\ Базовые настройки

// \/ Расширенные настройки
void MainWindow::on_checkBox_Extended_clicked(bool checked)
{
  ui->frame->setVisible(checked);
}

void MainWindow::on_checkBox_SelectFromNew_clicked(bool checked)
{
  s.selectFromNew = checked;
  p.updateSettings(s, false);
}

void MainWindow::on_checkBox_MutateOnce_clicked(bool checked)
{
  s.mutateOnce = checked;
  p.updateSettings(s, false);
}

void MainWindow::on_checkBox_CreateNew_clicked(bool checked)
{
  s.createNew = checked;
  p.updateSettings(s, false);
}

void MainWindow::on_checkBox_Clear_clicked(bool checked)
{
  s.clear = checked;
  p.updateSettings(s, false);
}

void MainWindow::on_checkBox_Threadable_clicked(bool checked)
{
  s.threadable = checked;
  p.updateSettings(s, false);
}
// /\ Расширенные настройки

// \/ Условия останова
void MainWindow::on_checkBox_MaxGeneration_clicked(bool checked)
{
  p.setStopConditions(checked, ui->spin_MaxGen->value(),
                      ui->checkBox_MaxTime->isChecked(), ui->time->time(),
                      ui->checkBox_BestResult->isChecked(), ui->spin_f->value());
}

void MainWindow::on_spin_MaxGen_valueChanged(qint32 arg1)
{
  p.setStopConditions(ui->checkBox_MaxGeneration->isChecked(), arg1,
                      ui->checkBox_MaxTime->isChecked(), ui->time->time(),
                      ui->checkBox_BestResult->isChecked(), ui->spin_f->value());
}

void MainWindow::on_checkBox_MaxTime_clicked(bool checked)
{
  p.setStopConditions(ui->checkBox_MaxGeneration->isChecked(), ui->spin_MaxGen->value(),
                      checked, ui->time->time(),
                      ui->checkBox_BestResult->isChecked(), ui->spin_f->value());
}

void MainWindow::on_time_timeChanged(const QTime &time)
{
  p.setStopConditions(ui->checkBox_MaxGeneration->isChecked(), ui->spin_MaxGen->value(),
                      ui->checkBox_MaxTime->isChecked(), time,
                      ui->checkBox_BestResult->isChecked(), ui->spin_f->value());
}

void MainWindow::on_checkBox_BestResult_clicked(bool checked)
{
  p.setStopConditions(ui->checkBox_MaxGeneration->isChecked(), ui->spin_MaxGen->value(),
                      ui->checkBox_MaxTime->isChecked(), ui->time->time(),
                      checked, ui->spin_f->value());
}

void MainWindow::on_spin_f_valueChanged(qreal arg1)
{
  p.setStopConditions(ui->checkBox_MaxGeneration->isChecked(), ui->spin_MaxGen->value(),
                      ui->checkBox_MaxTime->isChecked(), ui->time->time(),
                      ui->checkBox_BestResult->isChecked(), arg1);
}
// /\ Условия останова

// \/ Результаты

// Выбрали другое существо, надо обновить информацию в окне...
void MainWindow::on_list_Creatures_itemSelectionChanged()
{
  // ... но только если поиск решения не выполняется в данный момент
  if (ui->toolButton_Run->isEnabled())
    { p.requestFullInfo(ui->list_Creatures->currentRow()); }
}
// /\ Результаты

// \/ Слоты TPopulation
void MainWindow::updateResults(quint32 generation, quint32 time, const QList<double> &values)
{
  ui->label_GenerationNumber->setText(QString::number(generation));
  ui->list_Creatures->clear();
  quint32 c = 0;
  poly_m.clear();
  qreal best;
  QTime t;
  quint32 h, m, sec;
  h = time / (1000 * 60 * 60);
  time -= h * (1000 * 60 * 60);
  m = time / (1000 * 60);
  time -= m * (1000 * 60);
  sec = time / 1000;
  time -= sec * 1000;
  t.setHMS(h, m, sec, time);
  ui->label_Time->setText(t.toString(tr("hh:mm:ss.zzz")));

  if (!values.isEmpty())
    {
      best = values.first();

      foreach (qreal v, values)
        {
          ui->list_Creatures->addItem(QString::number(v, 'g', 9));
          poly_m << QPointF(++c, (s.increase) ? v / best : best / v);
        }

      histo.setSamples(poly_m);
    }

  ui->toolButton_Info->setChecked(iw->isVisible());
  setWindowTitle("GeneticSim :: " + ui->label_CreatureName->text() + tr(" :: Поколение ") + ui->label_GenerationNumber->text());
}

void MainWindow::addPoint(quint32 generation, qreal fitness)
{
  if (!poly.isEmpty())
    {
      qint32 ccc = poly.last().x();
      qint32 pok = qLn(ccc) / qLn(10);
      qint32 man = ccc / qPow(10, pok);

      if (poly.last().x() != qPow(10, pok)*man)
        { poly.removeLast(); }
    }

  poly << QPointF(generation, fitness);
  curve.setSamples(poly);
}

void MainWindow::stop(bool requested)
{
  if (!allowed)
    { return; }

  ui->toolButton_Open->setEnabled(true);
  ui->toolButton_Reopen->setEnabled(true);
  ui->toolButton_Run->setEnabled(true);
  ui->toolButton_Reset->setEnabled(true);
  ui->toolButton_Stop->setDisabled(true);

  if (!requested)
    {
      QMessageBox(QMessageBox::NoIcon, tr("Всё"), tr("Отбор завершён"), QMessageBox::Ok, this).exec();
    }

  p.requestUpdate();
}
// /\ Слоты TPopulation
