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
    case FrameType::INFRARED:
        cfg.enable_stream(RS2_STREAM_INFRARED, 1280, 720, RS2_FORMAT_Y8, 30);
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

    //enable mouse tracking for imagelabel
    ui -> imageLabel->setMouseTracking(true);
    //Since we tracking mouse events on a specific Qlabel, we will need to install an eventfilter method
    ui -> imageLabel -> installEventFilter(this);

    frameIndex = 0;
    isLoading = false; //These are check variables for the load function

    // Connect the QComboBox signal to the updateFrameType slot
    connect(ui->frameSelectionBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFrameType(int)));
    connect(ui ->recordButton, &QPushButton::toggled, this, &MainWindow::on_recordButton_toggled);
    connect(ui->loadButton, &QPushButton::toggled, this, &MainWindow::on_loadButton_toggled);


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
    cached_data = data;  // Cache the processed frame

    // Check if the frame is available.
    if (!data) {
        qDebug() << "No data from frame";
        return;
    }

    rs2::frame frame;
    rs2::colorizer color_map;
    switch (currentFrameType) {
    case FrameType::DEPTH:
        // Make sure depth frame exists before getting it
        if(data.first_or_default(RS2_STREAM_DEPTH)){
            frame = color_map.colorize(data.get_depth_frame()); // Find and colorize the depth data
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

    case FrameType::INFRARED:
        // Make sure color frame exists before getting it
        if(data.first_or_default(RS2_STREAM_INFRARED)){
            frame = data.get_infrared_frame(); // Find the color data
        } else {
            qDebug() << "Infrared frame not found";
            return;
        }
        break;

        // Handle other frame types as needed
    }

    // Convert the frame to a cv::Mat
    cv::Mat dep_mat = frame_to_mat(frame);

    // Display the image
    displayImage(dep_mat);

    if(isRecording){
        captureFrame();
    }

    // If isLoading is true, display the loaded frames in a loop
    if (isLoading && !frames.empty()) {
        displayImage(frames[frameIndex]);
        frameIndex = (frameIndex + 1) % frames.size();
    }
}


void MainWindow::updateFrameType(int index) {
    switch(index) {
    case 0:
        currentFrameType = FrameType::DEPTH;
        break;
    case 1:
        currentFrameType = FrameType::COLOR;
        break;
    case 2:
        currentFrameType = FrameType::INFRARED;
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

void MainWindow::on_recordButton_toggled(bool checked){
    if (checked) {
        // Button is checked, start recording frames
        isRecording = true;
    } else {
        // Button is not checked, stop recording frames
        isRecording = false;

        qDebug() << "Number of frames to save: " << frames.size();

        // When recording is stopped, save the recorded frames
        bool saved = saveFrames("/home/alam/depthcamerauiproject/tutorialui/recordedFrames/frames.xml");
        qDebug() << "Frames saved successfully: " << saved;

        // Clear the recorded frames
        //frames.clear();
    }
}

void MainWindow::captureFrame() {
    // Use cached_data instead of fetching again
    if (!cached_data) {
        qDebug() << "No data from frame";
        return;
    }

    // Get the right type of frame
    rs2::frame frame;
    rs2::colorizer color_map;
    switch (currentFrameType) {
    case FrameType::DEPTH:
        // Make sure depth frame exists before getting it
        if(cached_data.first_or_default(RS2_STREAM_DEPTH)){
            frame = color_map.colorize(cached_data.get_depth_frame()); // Find and colorize the depth data
        } else {
            qDebug() << "Depth frame not found";
            return;
        }
        break;

    case FrameType::COLOR:
        // Make sure color frame exists before getting it
        if(cached_data.first_or_default(RS2_STREAM_COLOR)){
            frame = cached_data.get_color_frame(); // Find the color data
        } else {
            qDebug() << "Color frame not found";
            return;
        }
        break;

    case FrameType::INFRARED:
        // Make sure infrared frame exists before getting it
        if(cached_data.first_or_default(RS2_STREAM_INFRARED)){
            frame = cached_data.get_infrared_frame(); // Find the infrared data
        } else {
            qDebug() << "Infrared frame not found";
            return;
        }
        break;

        // Handle other frame types as needed
    }

    // Convert the frame to a cv::Mat
    cv::Mat mat = frame_to_mat(frame);

    // Push this frame onto our vector of captured frames
    frames.push_back(mat.clone());

    qDebug() << "Frames captured, total frames:" << frames.size();
}


bool MainWindow::saveFrames(const std::string& filename) {
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    if (!fs.isOpened()) {
        qDebug() << "Failed to open" << QString::fromStdString(filename);
        return false;
    }

    for (size_t i = 0; i < frames.size(); ++i) {
        std::ostringstream ss;
        ss << "frame_" << i;

        // Write the matrix
        fs << ss.str() << frames[i];
    }

    fs.release();
    return true;
}

void MainWindow::loadFrames(const std::string& filename) {
    cv::FileStorage fs(filename, cv::FileStorage::READ);

    frames.clear();

    int i = 0;
    while (true) {
        std::ostringstream ss;
        ss << "frame_" << i;

        cv::Mat mat;
        fs[ss.str()] >> mat;

        if (mat.empty()) {
            break;
        }

        frames.push_back(mat);
        ++i;
    }

    fs.release();
}

void MainWindow::on_loadButton_toggled(bool checked){
    if (checked) {
        // Button is checked, start loading frames
        loadFrames("/home/alam/depthcamerauiproject/tutorialui/recordedFrames/frames.xml");
        isLoading = true;
        frameIndex = 0;
    } else {
        // Button is not checked, stop loading frames
        isLoading = false;
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->imageLabel && event->type() == QEvent::MouseMove) {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(event);
        handleMouseMoveOnImageLabel(mouseEvent->pos());
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::handleMouseMoveOnImageLabel(const QPoint &pos) {
    int x = static_cast<int>(pos.x() * (1280/static_cast<float>(ui->imageLabel->width())));
    int y = static_cast<int>(pos.y() * (720/static_cast<float>(ui->imageLabel->height())));

    if (currentFrameType == FrameType::DEPTH) {
        if (!cached_data) {
            qDebug() << "No cached data available";
            return;
        }

        if (auto depthFrame = cached_data.first_or_default(RS2_STREAM_DEPTH)) {
            float depth = depthFrame.as<rs2::depth_frame>().get_distance(x, y);
            // Display the data
            ui->statusbar->showMessage("Depth: " + QString::number(depth));
        } else {
            qDebug() << "Depth frame not found";
        }
    } else if (currentFrameType == FrameType::INFRARED) {
        if (!cached_data) {
            qDebug() << "No cached data available";
            return;
        }

        // Get IR intensity
        rs2::frame ir_frame = cached_data.first(RS2_STREAM_INFRARED);
        cv::Mat ir_mat = frame_to_mat(ir_frame);
        ushort intensity = ir_mat.at<ushort>(y, x);

        // Display both intensity and depth value
        ui->statusbar->showMessage("IR Intensity: " + QString::number(intensity));
    }

}

