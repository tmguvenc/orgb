#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "image_loader.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_load_img_clicked()
{
    const auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/jana", tr("Image Files (*.png *.jpg *.bmp)"));

    original_img = ImageLoader::readImage(fileName.toStdString());
    const auto data = original_img.ptr();
    const auto size = original_img.step1() * original_img.rows;

    ui->img_5->setPixmap(QPixmap::fromImage(QImage::fromData(data, size)));
}
