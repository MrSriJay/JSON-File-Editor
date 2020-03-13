#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QTreeView>
#include <QMainWindow>
#include <QJsonObject>
#include <QUndoCommand>
#include <QShortcut>
#include <QLabel>
#include "qjsonmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionOpen_triggered();

    void on_jsonTextEdit_textChanged();

    void on_actionSave_triggered();

    void on_actionUndo_triggered();

    void on_actionClear_triggered();

    void on_actionRedo_triggered();

    void on_actionExit_triggered();

    void on_actionSelect_All_triggered();

    void on_actionCut_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionincreaseFontSize_triggered();

    void on_actiondecreaseFontSize_triggered();

private:

    Ui::MainWindow *ui;
    QTreeView * view ;
    QAction *undoAction;
    QAction *redoAction;
    QJsonModel * model;
    QUndoStack *undoStack;
    QJsonObject m_currentJsonObject;
    void jsonTextEdit_View();
    void jsonTree_View();
    void saveJson();
    void createUndoStackAndActions();
    QShortcut * shortcut;
    qreal pointSize;
    QString openFileName;
    QLabel *lab;
};

#endif // MAINWINDOW_H
