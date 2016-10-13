#ifndef WWW_H
#define WWW_H

#include <QDialog>

namespace Ui {
class www;
}

class www : public QDialog
{
    Q_OBJECT

public:
    explicit www(QWidget *parent = 0);
    ~www();

public slots:
    void updateL(double *arr);

private:
    Ui::www *ui;
};

#endif // WWW_H
