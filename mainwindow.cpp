#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "image_loader.h"
#include <QFileDialog>
#include <QDir>

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
    auto current_dir = QDir::currentPath();

    const auto fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), current_dir.append("/../res"), tr("Image Files (*.png *.jpg *.bmp)"));

    if (!fileName.isEmpty()) {
        original_img = ImageLoader::readImage(fileName.toStdString());
        const auto data = original_img.ptr();
        const auto size = original_img.step1() * original_img.rows;

        image = QImage(original_img.cols, original_img.rows, QImage::Format::Format_Indexed8);
        image.loadFromData(data, size);

        pixmap = QPixmap(image.width(), image.height());
//        if( pixmap.load(fileName) ) {
        if( pixmap.loadFromData(data, size, "PNG") ) {

            ui->img_5->setPixmap(pixmap);
            ui->img_5->update();
        }

    }
}
