#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QDir>
#include <QImage>
#include "orgbconverter.h"
#include <opencv2/highgui.hpp>

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

            orgImage = cv::imread(fileName.toStdString());

            orgbConverter = std::make_unique<ORGBConverter>(orgImage);

            const auto orgbImage = orgbConverter->getOrgbImage({ 1.0, 1.0, 1.0 }, { m_lumaShift, m_ybShift, m_rgShift });
            image = QImage(orgbImage.data, pixmap.width(), pixmap.height(), QImage::Format::Format_RGB888);
            ui->img_1->setPixmap(QPixmap::fromImage(image));
            ui->img_1->update();
        }
    }
}

void MainWindow::updateImage() {
    const auto orgbImage = orgbConverter->getOrgbImage({ 1.0, 1.0, 1.0 }, {m_lumaShift, m_ybShift, m_rgShift});
    image = QImage(orgbImage.data, pixmap.width(), pixmap.height(), QImage::Format::Format_RGB888);
    ui->img_1->setPixmap(QPixmap::fromImage(image));
    ui->img_1->update();
}

void MainWindow::on_vs_luma_valueChanged(int value) {
    m_lumaShift = static_cast<double>(value) / 100;
    ui->lbl_luma->setText(QString::number(m_lumaShift));
    updateImage();
}

void MainWindow::on_vs_yb_valueChanged(int value) {
    m_ybShift = static_cast<double>(value) / 100;
    ui->lbl_yb->setText(QString::number(m_ybShift));
    updateImage();
}

void MainWindow::on_vs_rg_valueChanged(int value) {
    m_rgShift = static_cast<double>(value) / 100;
    ui->lbl_rg->setText(QString::number(m_rgShift));
    updateImage();
}
