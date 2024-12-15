# Документация разработчика
## Программа визуализации растровых алгоритмов

### 1. Общее описание

Программа представляет собой реализацию базовых растровых алгоритмов рисования с графическим интерфейсом пользователя. Программа разработана в соответствии с требованиями лабораторной работы №4.

### 2. Архитектура программы

#### 2.1 Основные компоненты
- Главное окно приложения (MainWindow)
- Графическая сцена (QGraphicsScene)
- Панель управления с элементами ввода
- Модули реализации алгоритмов

#### 2.2 Структура классов
```
MainWindow
├── setupUI()
├── drawGridNumbers()
├── clearScene()
├── putPixel()
├── Алгоритмы
│   ├── stepByStepLine()
│   ├── DDALine()
│   ├── bresenhamLine()
│   └── bresenhamCircle()
└── drawLine()
```

### 3. Реализация алгоритмов

#### 3.1 Пошаговый алгоритм
```cpp
void stepByStepLine(int x1, int y1, int x2, int y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double steps = std::max(std::abs(dx), std::abs(dy));
    
    double xIncrement = dx / steps;
    double yIncrement = dy / steps;
    
    double x = x1;
    double y = y1;
    
    for(int i = 0; i <= steps; i++) {
        putPixel(round(x), round(y));
        x += xIncrement;
        y += yIncrement;
    }
}
```

#### 3.2 Алгоритм ЦДА
```cpp
void DDALine(int x1, int y1, int x2, int y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;
    double steps = std::max(std::abs(dx), std::abs(dy));
    
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;
    
    float x = x1;
    float y = y1;
    
    for(int i = 0; i <= steps; i++) {
        putPixel(round(x), round(y));
        x += xIncrement;
        y += yIncrement;
    }
}
```

### 4. Система координат и масштабирование

#### 4.1 Система координат
- Начало координат в центре экрана
- Положительные направления: X - вправо, Y - вверх
- Шаг сетки: 10 единиц
- Диапазон координат: [-200, 200]

#### 4.2 Преобразование координат
```cpp
void putPixel(int x, int y, const QColor& color) {
    QBrush brush(color);
    scene->addRect(x * gridSize, -y * gridSize - gridSize,
                   gridSize, gridSize,
                   QPen(color.darker()), brush);
}
```



#### 5.2 Результаты тестирования
- Пошаговый алгоритм: ~0.5мс
- Алгоритм ЦДА: ~0.3мс
- Алгоритм Брезенхема (линия): ~0.2мс
- Алгоритм Брезенхема (окружность): ~0.4мс

### 6. Расширение функциональности

#### 6.1 Добавление нового алгоритма
1. Создать метод реализации алгоритма
2. Добавить пункт в выпадающий список
3. Добавить обработку в метод drawLine()
4. Обновить документацию

#### 6.2 Модификация интерфейса
1. Добавить необходимые элементы управления в setupUI()
2. Создать обработчики событий
3. Обновить логику отображения

### 7. Сборка и развертывание

#### 7.1 Требования к среде разработки
- Qt 5.12 или выше
- Компилятор с поддержкой C++11
- CMake 3.1 или выше


