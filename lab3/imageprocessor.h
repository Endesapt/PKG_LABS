#ifndef IMAGEPROCESSOR_H
#define IMAGEPROCESSOR_H

#include <QImage>

class ImageProcessor {
public:
    QImage linearContrast(const QImage &image);
    QImage histogramEqualization(const QImage &image);
    QImage convertToGray(const QImage &image); // Метод для преобразования в серое
    QImage processInRGB(const QImage &image); // Метод для обработки в RGB
    QImage processInHSV(const QImage &image); // Метод для обработки в HSV
};

#endif // IMAGEPROCESSOR_H
