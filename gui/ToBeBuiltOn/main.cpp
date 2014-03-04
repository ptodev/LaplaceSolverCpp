#include "start.h"
#include <QApplication>
#include <QSplashScreen>
#include <QTimer>
#include <QObject>


int main(int argc, char *argv[])
{
// a working example of a splash screen - this will be implemented in the non main .cpp file when the GUI is fully operational
// as it is, the mainwindow simply closes after running the splash screen
    QApplication app(argc, argv);

    QSplashScreen *splash = new QSplashScreen;
    splash->setPixmap(QPixmap("C:/Users/Ciaran/Downloads/startup2.png"));
    splash->show();
    splash->showMessage("Loading Modules -  we thank you for your patience");

    Start w;
    QTimer::singleShot(10000,splash,SLOT(close()));
    QTimer::singleShot(10000,&w,SLOT(show()));
   // w.show();
    return app.exec();
}
