#ifndef CLIPPINGWIDGET_H
#define CLIPPINGWIDGET_H


#include <QWidget>
#include <QPainter>
#include <QFile>
#include <QTextStream>


struct Line {
    QPointF p1, p2;
};

enum OutCode {
    INSIDE = 0, // 0000
    LEFT = 1,   // 0001
    RIGHT = 2,  // 0010
    BOTTOM = 4, // 0100
    TOP = 8     // 1000
};

class ClippingWidget : public QWidget {
    QVector<Line> lines;
    QRectF clipRect;

protected:
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        // Рисуем координатную сетку
        int gridSize = 20; // Размер ячейки сетки
        painter.setPen(QColor(220, 220, 220));
        for (int x = 0; x < width(); x += gridSize) {
            painter.drawLine(x, 0, x, height());
        }
        for (int y = 0; y < height(); y += gridSize) {
            painter.drawLine(0, y, width(), y);
        }

        // Рисуем отсекающее окно
        painter.setPen(Qt::red);
        QRectF scaledClipRect(clipRect.topLeft() * gridSize, clipRect.bottomRight() * gridSize);
        painter.drawRect(scaledClipRect);

        // Рисуем исходные отрезки
        painter.setPen(Qt::blue);
        for (const auto &line : lines) {
            QPointF p1 = line.p1 * gridSize;
            QPointF p2 = line.p2 * gridSize;
            painter.drawLine(p1, p2);
        }

        // Отсекаем и рисуем видимые части
        painter.setPen(Qt::green);
        for (const auto &line : lines) {
            Line clippedLine;
            if (cohenSutherlandClip(line, clippedLine)) {
                QPointF p1 = clippedLine.p1 * gridSize;
                QPointF p2 = clippedLine.p2 * gridSize;
                painter.drawLine(p1, p2);
            }
        }
    }

    int computeOutCode(float x, float y) {
        int code = OutCode::INSIDE;
        if (x < clipRect.left()) code |= OutCode::LEFT;
        else if (x > clipRect.right()) code |= OutCode::RIGHT;
        if (y < clipRect.top()) code |= OutCode::TOP;
        else if (y > clipRect.bottom()) code |= OutCode::BOTTOM;
        return code;
    }

    bool cohenSutherlandClip(const Line &line, Line &clippedLine) {
        float x0 = line.p1.x();
        float y0 = line.p1.y();
        float x1 = line.p2.x();
        float y1 = line.p2.y();

        int outcode0 = computeOutCode(x0, y0);
        int outcode1 = computeOutCode(x1, y1);

        bool accept = false;

        while (true) {
            if (!(outcode0 | outcode1)) {
                accept = true;
                break;
            } else if (outcode0 & outcode1) {
                break;
            } else {
                float x, y;
                int outcodeOut = outcode0 ? outcode0 : outcode1;

                if (outcodeOut & OutCode::TOP) {
                    x = x0 + (x1 - x0) * (clipRect.top() - y0) / (y1 - y0);
                    y = clipRect.top();
                } else if (outcodeOut & OutCode::BOTTOM) {
                    x = x0 + (x1 - x0) * (clipRect.bottom() - y0) / (y1 - y0);
                    y = clipRect.bottom();
                } else if (outcodeOut & OutCode::RIGHT) {
                    y = y0 + (y1 - y0) * (clipRect.right() - x0) / (x1 - x0);
                    x = clipRect.right();
                } else if (outcodeOut & OutCode::LEFT) {
                    y = y0 + (y1 - y0) * (clipRect.left() - x0) / (x1 - x0);
                    x = clipRect.left();
                }

                if (outcodeOut == outcode0) {
                    x0 = x;
                    y0 = y;
                    outcode0 = computeOutCode(x0, y0);
                } else {
                    x1 = x;
                    y1 = y;
                    outcode1 = computeOutCode(x1, y1);
                }
            }
        }

        if (accept) {
            clippedLine.p1 = QPointF(x0, y0);
            clippedLine.p2 = QPointF(x1, y1);
            return true;
        }
        return false;
    }

public:
    ClippingWidget(const QString &fileName, QWidget *parent = nullptr) : QWidget(parent) {
        loadFromFile(fileName);
    }

    void loadFromFile(const QString &fileName) {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Cannot open file:" << fileName;
            return;
        }

        QTextStream in(&file);
        int n;
        in >> n;
        for (int i = 0; i < n; ++i) {
            float x1, y1, x2, y2;
            in >> x1 >> y1 >> x2 >> y2;
            lines.append({QPointF(x1, y1), QPointF(x2, y2)});
        }

        float xmin, ymin, xmax, ymax;
        in >> xmin >> ymin >> xmax >> ymax;
        clipRect = QRectF(QPointF(xmin, ymin), QPointF(xmax, ymax));

        file.close();
    }
};

#endif // CLIPPINGWIDGET_H
