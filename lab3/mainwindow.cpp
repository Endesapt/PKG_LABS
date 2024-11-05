#include "mainwindow.h"
#include <QImage>
#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    originalImageLabel(new QLabel(this)),
    processedImageLabel(new QLabel(this)),
    selectImageButton(new QPushButton("Выбрать изображение", this)),
    linearContrButton(new QPushButton("Применить линейное контрастированеи", this)),
    HistButton(new QPushButton("Применить выравнивание гистограммы", this)),
    saveButton(new QPushButton("Сохранить обработанное изображение", this)),
    rgbButton(new QPushButton("Поэлементное RGВ", this)),
    hsvButton(new QPushButton("Поэлементное HSV", this)),
    grayButton(new QPushButton("Поэлементное Серое", this)),
    imageProcessor(new ImageProcessor()),
    colorModel("RGB"),
    originalImagePath("") // Инициализация пустой строки для пути к изображению
{

    setWindowIcon(QIcon(":/ico.ico"));
    setMinimumSize(1000, 1000);

    // Включаем поддержку перетаскивания
    originalImageLabel->setAcceptDrops(true);

    // Установка политики размера для миниатюр
    originalImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    processedImageLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *originalLayout = new QVBoxLayout;
    originalLayout->addWidget(originalImageLabel);
    originalLayout->setAlignment(Qt::AlignCenter); // Центрируем вертикально

    QVBoxLayout *processedLayout = new QVBoxLayout;
    processedLayout->addWidget(processedImageLabel);
    processedLayout->setAlignment(Qt::AlignCenter); // Центрируем вертикально

    // Макет для изображений
    QHBoxLayout *imageLayout = new QHBoxLayout;
    imageLayout->addLayout(originalLayout);
    imageLayout->addLayout(processedLayout);
    imageLayout->setAlignment(Qt::AlignCenter); // Центрируем горизонтально

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(imageLayout);
    mainLayout->addWidget(selectImageButton, 0, Qt::AlignCenter); // Центрируем кнопку выбора изображения

    // Добавляем кнопки выбора цветового пространства
    mainLayout->addWidget(rgbButton);
    mainLayout->addWidget(hsvButton);
    mainLayout->addWidget(grayButton);

    mainLayout->addWidget(HistButton);
    mainLayout->addWidget(linearContrButton);
    mainLayout->addWidget(saveButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    connect(selectImageButton, &QPushButton::clicked, this, &MainWindow::selectImage);
    connect(HistButton, &QPushButton::clicked, this, &MainWindow::hist);
    connect(linearContrButton, &QPushButton::clicked, this, &MainWindow::linearContr);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveProcessedImage);
    connect(rgbButton, &QPushButton::clicked, this, &MainWindow::setColorModelRGB);
    connect(hsvButton, &QPushButton::clicked, this, &MainWindow::setColorModelHSV);
    connect(grayButton, &QPushButton::clicked, this, &MainWindow::setColorModelGray);
}

// Переопределяем метод для обработки перетаскивания
void MainWindow::dragEnterEvent(QDragEnterEvent *event) {
    if (event->mimeData()->hasUrls()) {
        event->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *event) {
    const QMimeData *mimeData = event->mimeData();
    if (mimeData->hasUrls()) {
        QList<QUrl> urlList = mimeData->urls();
        if (!urlList.isEmpty()) {
            QString filePath = urlList.first().toLocalFile();
            if (QImageReader::supportedImageFormats().contains(QFileInfo(filePath).suffix().toLower().toUtf8())) {
                loadImage(filePath);
            } else {
                QMessageBox::warning(this, "Ошибка", "Файл не является изображением.");
            }
        }
    }
}

void MainWindow::loadImage(const QString &filePath) {
    QImage originalImage(filePath);

    originalImageLabel->setPixmap(QPixmap::fromImage(originalImage).scaled(500, 500, Qt::KeepAspectRatio));
    originalImagePath = filePath;
}

MainWindow::~MainWindow() {
    delete imageProcessor;
}

void MainWindow::selectImage() {
    QString filePath = QFileDialog::getOpenFileName(this, "Выберите изображение", QString(), "Images (*.png *.jpg *.bmp)");

    loadImage(filePath);
}


void MainWindow::saveProcessedImage() {

    QString savePath = QFileDialog::getSaveFileName(this, "Сохранить изображение", QString(),
                                                    "PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp);;All Files (*)");


    QImage processedImage = processedImageLabel->pixmap().toImage();
    QString format;
    if (savePath.endsWith(".png")) {
        format = "PNG";
    } else if (savePath.endsWith(".jpg")) {
        format = "JPEG";
    } else {
        format = "BMP";
    }
    processedImage.save(savePath, format.toUtf8().constData());
    QMessageBox::information(this, "Успех", "Изображение успешно сохранено.");

}

void MainWindow::linearContr() {
    QImage originalImage(originalImagePath);
    QImage processedImage = imageProcessor->linearContrast(originalImage);
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(500, 500, Qt::KeepAspectRatio));
}

void MainWindow::hist() {
    QImage originalImage(originalImagePath);
    QImage processedImage = imageProcessor->histogramEqualization(originalImage);
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(500, 500, Qt::KeepAspectRatio));
}

void MainWindow::setColorModelRGB() {
    QImage originalImage(originalImagePath);
    QImage processedImage = imageProcessor->processInRGB(originalImage);
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(500, 500, Qt::KeepAspectRatio));
}

void MainWindow::setColorModelHSV() {
    QImage originalImage(originalImagePath);
    QImage processedImage = imageProcessor->processInHSV(originalImage);
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(500, 500, Qt::KeepAspectRatio));
}

void MainWindow::setColorModelGray() {
    QImage originalImage(originalImagePath);
    QImage processedImage = imageProcessor->convertToGray(originalImage);
    processedImageLabel->setPixmap(QPixmap::fromImage(processedImage).scaled(500, 500, Qt::KeepAspectRatio));
}
