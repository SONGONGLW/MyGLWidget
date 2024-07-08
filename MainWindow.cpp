#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QPixmap>

#include <QFileDialog>

#include <QTimer>
#include <thread>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    InitUi();
    InitData();
    InitConect();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::InitUi()
{
    setCentralWidget(ui->openGLWidget);

}

void MainWindow::InitData()
{
    timer = new QTimer();
    connect(timer, &QTimer::timeout, this, [&]{
        char const *filename = nullptr;
        ui->openGLWidget->drawShape(MyGLWidget::eRect);
        ui->openGLWidget->loadImage(filename);

//        QImage img = dasabi ? QImage("C:\\Users\\SONG\\Desktop\\素材\\2023.jpg")
//                            : QImage("C:\\Users\\SONG\\Desktop\\素材\\9999.jpg");
//        QImage img2 = img.convertToFormat(QImage::Format_RGB888);

//        ui->openGLWidget->drawShape(MyGLWidget::eRect/*eBigRect*/);
//        ui->openGLWidget->loadImage(img2);

    });

    QList<QString> str;

    std::vector<std::string> stdstr = {"a", "b", "c", "d", "e", "f", "g", "h"};
    for(auto it = stdstr.crbegin(); it != stdstr.crend(); ++it)
    {
        qDebug() << *it;

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }



}

void MainWindow::InitConect()
{
    connect(ui->draw_img, &QAction::triggered, this, &MainWindow::SlotImgActionTriggered);
    connect(ui->draw_rect, &QAction::triggered, this, &MainWindow::SlotDrawActionTriggered);
    connect(ui->line_mode, &QAction::triggered, this, &MainWindow::SlotLineModeActionTriggered);
    connect(ui->clear, &QAction::triggered, this, &MainWindow::SlotClearActionTriggered);
    connect(ui->tmp_action, &QAction::triggered, this, &MainWindow::SlotTmpActionTriggered);
    connect(ui->save_action, &QAction::triggered, this, &MainWindow::SlotSaveActionTriggered);
}


void MainWindow::SlotDrawActionTriggered()
{
    QImage img = QImage("C:\\Users\\SONG\\Desktop\\素材\\3333.png");
    QImage img2 = img.convertToFormat(QImage::Format_RGB888).mirrored();

    ui->openGLWidget->drawShape(MyGLWidget::eRect);
    ui->openGLWidget->loadImage(img2);
}

void MainWindow::SlotImgActionTriggered()
{
    ui->openGLWidget->drawShape(MyGLWidget::eTextureRect);
    ui->openGLWidget->loadImage("C:\\Users\\SONG\\Desktop\\素材\\2026.jpg");
}

void MainWindow::SlotLineModeActionTriggered(bool checked)
{
    if(checked)
    {
        ui->openGLWidget->setPolygonMode(MyGLWidget::eLine);
    }
    else
    {
        ui->openGLWidget->setPolygonMode(MyGLWidget::eFill);
    }
}

void MainWindow::SlotClearActionTriggered()
{
    ui->openGLWidget->drawShape(MyGLWidget::eNone);
    ui->openGLWidget->loadImage(QImage("C:\\Users\\SONG\\Desktop\\素材\\9999.jpg"));
}

void MainWindow::SlotTmpActionTriggered(bool checked)
{
    if(checked)
    {
        timer->start(16);
    }
    else
    {
        timer->stop();
    }
}

void MainWindow::SlotSaveActionTriggered()
{
    QString saveName = QFileDialog::getSaveFileName(nullptr, "保存图片", "C:\\Users\\SONG\\Desktop\\素材", "*.png;;*.xpm;;*.jpg;;*.ico");
    if(saveName.isEmpty())
        return;
    ui->openGLWidget->saveImage(saveName);
}

