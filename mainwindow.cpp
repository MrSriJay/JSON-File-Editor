#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDir>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFileDialog>
#include <QUndoCommand>
#include <QShortcut>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->jsonTextEdit->setText("{\n}");
    ui->jsonTextEdit->fontPointSize();
    pointSize = 2;
    openFileName="New Document";
    lab = new QLabel("   Current Document: "+openFileName);
    statusBar()->addWidget(lab);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    jsonTextEdit_View();
    jsonTree_View();
}


void MainWindow::on_jsonTextEdit_textChanged()
{
    QString textdate=ui->jsonTextEdit->toPlainText();
    if(textdate!=""){
        jsonTree_View();
    }

}


void MainWindow::on_actionSave_triggered()
{
    saveJson();
}

void MainWindow::on_actionClear_triggered()
{
    m_currentJsonObject = QJsonObject();
    ui->jsonTextEdit->clear();
    ui->jsonTreeView->clearSelection();
    ui->jsonTextEdit->setText(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
    lab->clear();
    openFileName="New Document";
    lab = new QLabel("   Current Document: "+openFileName);
    statusBar()->addWidget(lab);
}

void MainWindow::on_actionUndo_triggered()
{

   ui->jsonTextEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
     ui->jsonTextEdit->redo();
}

void MainWindow::on_actionExit_triggered()
{
    QCoreApplication::quit();
}

void MainWindow::on_actionSelect_All_triggered()
{
     ui->jsonTextEdit->selectAll();
}

void MainWindow::on_actionCut_triggered()
{
    #if QT_CONFIG(clipboard)
        ui->jsonTextEdit->cut();
    #endif
}

void MainWindow::on_actionCopy_triggered()
{
    #if QT_CONFIG(clipboard)
        ui->jsonTextEdit->copy();
    #endif
}

void MainWindow::on_actionPaste_triggered()
{
    #if QT_CONFIG(clipboard)
        ui->jsonTextEdit->paste();
    #endif
}

void MainWindow::on_actionincreaseFontSize_triggered()
{
    pointSize = pointSize+10;
    QTextCharFormat format;
    format.setFontPointSize(pointSize);
    ui->jsonTextEdit->selectAll();
    ui->jsonTextEdit->mergeCurrentCharFormat(format);
}

void MainWindow::on_actiondecreaseFontSize_triggered()
{
    pointSize = pointSize-10;
    QTextCharFormat format;
    format.setFontPointSize(pointSize);
    ui->jsonTextEdit->selectAll();
    ui->jsonTextEdit->mergeCurrentCharFormat(format);

}


//-------------- Functions|Actions------------------------------------

void MainWindow::jsonTextEdit_View(){
    openFileName = QFileDialog::getOpenFileName(this,
                                                            tr("Open Json File"),
                                                            QString(),
                                                            tr("JSON (*.json)"));
    QFileInfo fileInfo(openFileName);
    QDir::setCurrent(fileInfo.path());

    QFile jsonFile(openFileName);
    if (!jsonFile.open(QIODevice::ReadOnly))
    {
        return;
    }

    QByteArray saveData = jsonFile.readAll();
    QJsonDocument jsonDocument(QJsonDocument::fromJson(saveData));
    m_currentJsonObject = jsonDocument.object();
    ui->jsonTextEdit->clear();
    ui->jsonTextEdit->setText(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
    lab->clear();
    lab = new QLabel("   Current Document: "+openFileName);
    statusBar()->addWidget(lab);
}

void MainWindow::jsonTree_View(){

    QString data=ui->jsonTextEdit->toPlainText();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8());
    m_currentJsonObject = jsonDocument.object();
    view= new QTreeView;
    model = new QJsonModel;
    view->setModel(model);
    model->loadJson(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
    ui->jsonTreeView->setModel(model);
}


void MainWindow::saveJson(){

    QString saveFileName = QFileDialog::getSaveFileName(this,
                                                        tr("Save Json File"),
                                                        QString(),
                                                        tr("JSON (*.json)"));
    QFileInfo fileInfo(saveFileName);
    QDir::setCurrent(fileInfo.path());
    QFile jsonFile(saveFileName);
    if (!jsonFile.open(QIODevice::WriteOnly))
    {
        return;
    }

    jsonFile.write(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented));
    jsonFile.close();
}







