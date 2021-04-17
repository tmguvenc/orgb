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

    m_grid[0][0] = ui->img_1;
    m_grid[0][1] = ui->img_2;
    m_grid[0][2] = ui->img_3;

    m_grid[1][0] = ui->img_4;
    m_grid[1][1] = ui->img_5;
    m_grid[1][2] = ui->img_6;

    m_grid[2][0] = ui->img_7;
    m_grid[2][1] = ui->img_8;
    m_grid[2][2] = ui->img_9;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateImage(QLabel* pb, QImage& image, const cv::Vec3d& shiftingFactor) {
    const auto orgbImage = m_orgbConverter->getOrgbImage({ 1.0, 1.0, 1.0 }, shiftingFactor);
    image = QImage(orgbImage.data, m_pixmap.width(), m_pixmap.height(), QImage::Format::Format_RGB888);
    pb->setPixmap(QPixmap::fromImage(image));
    pb->update();
}

void MainWindow::updateImages() {
    /** @note
     *  I could create a composite image using those 9 images created,
     *  but it didn't look good and understandable.
     *  So I dedided to create a picture box grid and place the orgb images
     *  into it.
     */
    double rgShift = m_shiftFactor;
    for(auto row = 0; row < 3; ++row) {
        auto ybShift = -m_shiftFactor;
        for(auto col = 0; col < 3; ++col) {
            updateImage(m_grid[row][col], m_image[row][col], {0.0, ybShift, rgShift});
            ybShift += m_shiftFactor;
        }
        rgShift -= m_shiftFactor;
    }
}

void MainWindow::on_btn_load_img_clicked()
{
    const auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), QDir::currentPath().append("/res"), tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        if( m_pixmap.load(fileName) ) {
            ui->img_5->setPixmap(m_pixmap);
            ui->img_5->update();

            m_orgImage = cv::imread(fileName.toStdString());
            m_orgbConverter = std::make_unique<ORGBConverter>(m_orgImage);
            ui->vs_sf->setValue(30);
            updateImages();
        }
    }
}

void MainWindow::on_vs_sf_valueChanged(int value) {
    m_shiftFactor = static_cast<double>(value) / 100.0;
    ui->lbl_sf->setText(QString::number(m_shiftFactor));
    updateImages();
}
