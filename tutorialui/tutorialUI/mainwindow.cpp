#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "cv-helper.hpp"

void MainWindow::startPipeline() {
    // Stop the pipeline if it's already running
    if (isPipeStarted) {
        pipe.stop();
        isPipeStarted = false;
    }

    // Configure the pipeline based on the current frame type
    rs2::config cfg;
    switch(currentFrameType) {
    case FrameType::DEPTH:
        cfg.enable_stream(RS2_STREAM_DEPTH, 1280, 720, RS2_FORMAT_Z16, 30);
        break;
    case FrameType::COLOR:
        cfg.enable_stream(RS2_STREAM_COLOR, 1280, 720, RS2_FORMAT_RGB8, 30);
        break;
    }

    // Start the pipeline with the new configuration
    pipe.start(cfg);
    isPipeStarted = true;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect the QComboBox signal to the updateFrameType slot
    connect(ui->frameSelectionBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFrameType(int)));

    // Initialize RealSense pipeline
    startPipeline();  // Use the new function to start the pipeline

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
    rs2::align align_to(RS2_STREAM_COLOR);
    auto data = pipe.wait_for_frames();
    data = align_to.process(data);

    // Check if the frame is available.
    if (!data) {
        qDebug() << "No data from frame";
        return;
    }

    rs2::frame frame;
    switch (currentFrameType) {
    case FrameType::DEPTH:
        // Make sure depth frame exists before getting it
        if(data.first_or_default(RS2_STREAM_DEPTH)){
            frame = data.get_depth_frame(); // Find and colorize the depth data
        } else {
            qDebug() << "Depth frame not found";
            return;
        }
        break;

    case FrameType::COLOR:
        // Make sure color frame exists before getting it
        if(data.first_or_default(RS2_STREAM_COLOR)){
            frame = data.get_color_frame(); // Find the color data
        } else {
            qDebug() << "Color frame not found";
            return;
        }
        break;

        // Handle other frame types as needed
    }

    // Convert the frame to a cv::Mat
    cv::Mat dep_mat = frame_to_mat(frame);

    // Display the image
    displayImage(dep_mat);
}


void MainWindow::updateFrameType(int index) {
    switch(index) {
    case 0:
        currentFrameType = FrameType::DEPTH;
        break;
    case 1:
        currentFrameType = FrameType::COLOR;
        break;
        // Handle other frame types as needed
    }

    startPipeline();  // Restart the pipeline with the new frame type
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
