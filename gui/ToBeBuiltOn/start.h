#ifndef START_H
#define START_H

#include<QtGui>
#include<QWidget>
#include<QApplication>
#include<QObject>
#include<QMainWindow>
#include<ui_start.h>

namespace Ui {
class Start;
}

class Start : public QMainWindow
{
    Q_OBJECT

public:
    explicit Start(QWidget *parent = 0);
    ~Start();


  signals:


private slots:

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionNew_triggered();


   void on_xGridS_valueChanged(double newXGridVal);

   void on_yGridS_valueChanged(double newYGridVal);

   void on_xMaxS_valueChanged(double newXAbsVal);

   void on_yMaxS_valueChanged(double newYAbsVal);

   void beginLink();

   void on_saveSettings_clicked();

private:
    Ui::Start *ui;

    //define here the grid variables - these vaules will be changed by the linking functions
    //which simply allocate a value to the double from one of the spin boxes. There may be
    //a more eloquent soultion, however this will suffice until a later version.

    double x_grid_value;
    double y_grid_value;
    double xAbs;
    double yAbs;
    QString myFilename;
    QString originalText;

};

#endif // START_H
