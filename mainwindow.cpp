#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include "orgbconverter.h"

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
        tr("Open Image"), QDir::currentPath().append("/../res"), tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        if( pixmap.load(fileName) ) {
            ui->img_5->setPixmap(pixmap);
            ui->img_5->update();

            orgImage.create(pixmap.height(), pixmap.width(), CV_8UC3);
            memcpy(orgImage.data, pixmap.toImage().constScanLine(0), orgImage.step1() * orgImage.rows);

            orgbConverter = std::make_unique<ORGBConverter>(orgImage);
        }
    }
}
