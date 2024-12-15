#include "imageprocessor.h"
#include <QMessageBox>
#include <algorithm>
QImage ImageProcessor::linearContrast(const QImage &image) {
    QImage result = image;

    int minRed = 255, maxRed = 0;
    int minGreen = 255, maxGreen = 0;
    int minBlue = 255, maxBlue = 0;

    // Находим минимальные и максимальные значения для каждого канала
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);
            int red = color.red();
            int green = color.green();
            int blue = color.blue();

            minRed = std::min(minRed, red);
            maxRed = std::max(maxRed, red);
            minGreen = std::min(minGreen, green);
            maxGreen = std::max(maxGreen, green);
            minBlue = std::min(minBlue, blue);
            maxBlue = std::max(maxBlue, blue);
        }
    }

    // Проверяем, чтобы избежать деления на ноль
    if (maxRed == minRed) {
        maxRed = minRed + 1; // Изменяем max, чтобы избежать деления на 0
    }
    if (maxGreen == minGreen) {
        maxGreen = minGreen + 1; // Аналогично для зеленого
    }
    if (maxBlue == minBlue) {
        maxBlue = minBlue + 1; // И для синего
    }

    // Применяем линейное контрастирование
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);
            int red = color.red();
            int green = color.green();
            int blue = color.blue();

            int contrastedRed = (red - minRed) * 255 / (maxRed - minRed);
            int contrastedGreen = (green - minGreen) * 255 / (maxGreen - minGreen);
            int contrastedBlue = (blue - minBlue) * 255 / (maxBlue - minBlue);

            result.setPixelColor(x, y, QColor{
                                           std::clamp(contrastedRed, 0, 255),
                                           std::clamp(contrastedGreen, 0, 255),
                                           std::clamp(contrastedBlue, 0, 255)
                                       });
        }
    }

    return result;
}

QImage ImageProcessor::histogramEqualization(const QImage &image) {
    QImage result(image.size(), QImage::Format_RGB32); // Создаем новое изображение в формате RGB32

    // Выделяем три массива для гистограмм
    int histogram[3][256] = {{0}}; // Для R, G и B
    int cumulative[3][256] = {{0}}; // Для накопленных значений

    // Построение гистограммы для каждого канала
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);
            histogram[0][color.red()]++;   // Гистограмма для красного канала
            histogram[1][color.green()]++; // Гистограмма для зеленого канала
            histogram[2][color.blue()]++;  // Гистограмма для синего канала
        }
    }

    // Накопленная сумма для каждого канала
    for (int i = 0; i < 3; ++i) {
        cumulative[i][0] = histogram[i][0]; // Начальное значение накопленной суммы
        for (int j = 1; j < 256; ++j) {
            cumulative[i][j] = cumulative[i][j - 1] + histogram[i][j]; // Накопление значений
        }
    }

    // Преобразование изображения для каждого канала
    int totalPixels = image.width() * image.height(); // Общее количество пикселей
    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);
            int newRed = cumulative[0][color.red()] * 255 / totalPixels; // Новое значение для красного канала
            int newGreen = cumulative[1][color.green()] * 255 / totalPixels; // Для зеленого
            int newBlue = cumulative[2][color.blue()] * 255 / totalPixels; // Для синего

            // Устанавливаем новые значения пикселей
            result.setPixelColor(x, y, QColor{
                                           std::clamp(newRed, 0, 255),
                                           std::clamp(newGreen, 0, 255),
                                           std::clamp(newBlue, 0, 255)
                                       });
        }
    }
    return result; // Возвращаем обработанное изображение
}


QImage ImageProcessor::processInRGB(const QImage &image) {
    QImage resultImage = image;
    for (int y = 0; y < resultImage.height(); ++y) {
        for (int x = 0; x < resultImage.width(); ++x) {
            QColor color = resultImage.pixelColor(x, y);

            // Инвертируем цвета
            int red = 255 - color.red();
            int green = 255 - color.green();
            int blue = 255 - color.blue();

            resultImage.setPixelColor(x, y, QColor(red, green, blue));
        }
    }
    return resultImage;
}

QImage ImageProcessor::processInHSV(const QImage &image) {
    QImage hsvImage = image.convertToFormat(QImage::Format_RGB32); // Конвертируем в формат для работы с HSV

    for (int y = 0; y < hsvImage.height(); ++y) {
        for (int x = 0; x < hsvImage.width(); ++x) {
            QColor color = QColor(hsvImage.pixel(x, y));

            // Преобразуем цвет в HSV
            int h, s, v;
            color.getHsv(&h, &s, &v);

            color.setHsv(h, s, v);
            hsvImage.setPixel(x, y, color.rgb());
        }
    }

    return hsvImage;
}


QImage ImageProcessor::convertToGray(const QImage &image) {
    QImage grayImage(image.size(), QImage::Format_Grayscale8);

    for (int y = 0; y < image.height(); ++y) {
        for (int x = 0; x < image.width(); ++x) {
            QColor color = image.pixelColor(x, y);

            // Рассчитываем яркость по формуле: 0.3*R + 0.59*G + 0.11*B
            int gray = qGray(color.rgb());
            grayImage.setPixelColor(x, y, QColor(gray, gray, gray));
        }
    }
    return grayImage;
}

