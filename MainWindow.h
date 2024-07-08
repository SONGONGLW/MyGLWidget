#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void SlotDrawActionTriggered();
    void SlotImgActionTriggered();
    void SlotLineModeActionTriggered(bool checked);
    void SlotClearActionTriggered();
    void SlotTmpActionTriggered(bool checked);
    void SlotSaveActionTriggered();

private:
    void InitUi();
    void InitData();
    void InitConect();


private:
    Ui::MainWindow *ui;

    QTimer *timer = nullptr;
};
#endif // MAINWINDOW_H
