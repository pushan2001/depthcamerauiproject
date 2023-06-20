#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "cv-helper.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Initialize RealSense pipeline
    rs2::config cfg;
    cfg.enable_stream(RS2_STREAM_DEPTH, 848, 480, RS2_FORMAT_Z16, 30);
    pipe.start(cfg);

    // Set up the timer to call updateFrame 30 times per second
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateFrame);
    timer->start(1000/30);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateFrame()
{
    // Capture a frame from the camera, similar to what your existing main function does
    rs2::align align_to(RS2_STREAM_INFRARED);
    auto data = pipe.wait_for_frames();
    data = align_to.process(data);
    auto dep_frame = data.get_depth_frame();

    // Convert the frame to a cv::Mat
    cv::Mat dep_mat = frame_to_mat(dep_frame);

    // Display the image
    displayImage(dep_mat);
}

void MainWindow::displayImage(const cv::Mat &img)
{
    // Convert the image from BGR to RGB
    cv::Mat rgbImg;
    cv::cvtColor(img, rgbImg, cv::COLOR_BGR2RGB);

    // Convert the OpenCV image to QImage
    QImage qImg((uchar*)rgbImg.data, rgbImg.cols, rgbImg.rows, rgbImg.step, QImage::Format_RGB888);

    // Display the image on the QLabel
    ui->imageLabel->setPixmap(QPixmap::fromImage(qImg));

    // Resize the QLabel to fit the image
    ui->imageLabel->resize(ui->imageLabel->pixmap().size());
}
