#include "start.h"
#include "ui_start.h"
#include <QtCore>
#include <QApplication>
#include <QtGui>
#include <QWidget>
#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QClipboard>
#include <QMimeData>

//set up action functions -QClipboard
//set up QProcess on buttons
//Make boxes write to file -> changed to save all the values at once
//saves the user from writing to an existing file as they change their values
//when loading a preset, show the settings on the GUI

Start::Start(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Start)
{
    ui->setupUi(this);

    //qsettings -> I have a new idea here to be added

//Add connections here
    connect(ui->xGridS, SIGNAL(valueChanged(double)), this,SLOT(on_xGridS_valueChanged(double)));
    connect(ui->yGridS, SIGNAL(valueChanged(double)), this,SLOT(copyValue(double)));
    connect(ui->xMaxS, SIGNAL(valueChanged(double)), this,SLOT(copyValue(double)));
    connect(ui->yMaxS, SIGNAL(valueChanged(double)), this,SLOT(copyValue(double)));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(beginLink()));
    connect(ui->saveSettings,SIGNAL(clicked()), this,SLOT(on_saveSettings_clicked()));

}

Start::~Start()
{
    delete ui;
}

void Start::beginLink()
{
//    QProcess newProcess = new QProcess;
//    newProcess = -EXTERNAL
//    run(*.txt)
}

void Start::on_actionNew_triggered()
{
    myFilename = "";
    //reinitilaise all the widgets to zero

}

void Start::on_actionOpen_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Document"),QDir::currentPath(),tr("Document files (*.doc *.rtf);;All files (*.*)") );
    if( !filename.isEmpty())
    {
   //when we open an existing filename we want to:
        //read the file
        //initilise the corresponding data entires to the correct spin boxes
        QFile rFile(filename);
        if(rFile.open(QFile::ReadOnly | QFile::Text))
        {
        myFilename = filename;// sets the filename and hence location
        QTextStream in(&rFile);
        QString text = in.readAll(); // may wish to read line by way
        rFile.close();//close reading file being streamed
        //message box saying file has been read
        //initialise spinbox settings
    }

}
}

void Start::on_actionSave_triggered()
{ //this functionality is now taken care of in the save settings button press

    //save an existing plot
    //this will be easy to do using save images
//QFile sFile(mFilename);
    //if(sFile.open(QFile:WriteOnly | QFile::Text)
  //  {
      //      QTextStream out(&sFile);

     //       out << ui->textEdit->toPlainText();

      //      sFile.flush();
     //       sFile.close();
   // }

}

void Start::on_actionSave_as_triggered()
{
   //save an existing plot as a different format
    QString file = QFileDialog::getSaveFileName(this,"Save file as...");

            if (!file.isEmpty())
    {
            myFilename=file;
     on_saveSettings_clicked();

    }
}

void Start::on_actionCopy_triggered()
{
//copy a string of text - here it will always be an integer string. Could use a validotor but should be fine
    QClipboard *clipboard = QApplication::clipboard();
    originalText = clipboard->text();
        //clipboard->setText();

}

void Start::on_actionPaste_triggered()
{
    //paste a string of text
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();
    if (mimeData->hasText()) {
     //       clipboard->setText(mimeData->text());
     //       clipboard->setTextFormat(Qt::PlainText);
    }
    //else {setText(tr("Cannot display data"));
    }
//}

void Start::on_actionCut_triggered()
{
  //cut a string a text
  //  ui->textEdit->cut();
}

//can add in undo and redo

void Start::on_xGridS_valueChanged(double newXGridVal)
{
       x_grid_value = newXGridVal; // or can probably use ui.xGridS.value()
       update();
}

void Start::on_yGridS_valueChanged(double newYGridVal)
{
    y_grid_value = newYGridVal;
    update();
}

void Start::on_xMaxS_valueChanged(double newXAbsVal)
{
    xAbs = newXAbsVal;
    update();
}


void Start::on_yMaxS_valueChanged(double newYAbsVal)
{
    yAbs = newYAbsVal;
    update();
}




void Start::on_saveSettings_clicked()
{
    //have all the spin boxes write to the file
    //open the file
    //conditionals
    //getValue of the boxes
    //write the terms in a certain way
    //close the file once finished writing
    //alert the user it has been saved
    //next direct action is to press the push button for run

    //test for filename
    QFile rFile(myFilename); //pointer to pointer
    if(rFile.open(QFile::WriteOnly | QFile::Text))
     {
      QTextStream out(&rFile);
      out<<qSetFieldWidth(8)<<x_grid_value<<left<<y_grid_value<<left<<xAbs<<left<<yAbs;
      rFile.flush();
      rFile.close();
      //add in a message box here for confirmation
    }

}
