# Техническая документация разработчика
## Лабораторная работа: Трехмерная визуализация и преобразования

### 1. Описание программы

#### 1.1 Назначение
Программа предназначена для создания, визуализации и манипулирования трехмерными объектами с использованием Three.js. Основной функционал включает построение трехмерной буквы, выполнение геометрических преобразований и отображение ортографических проекций.

#### 1.2 Технические характеристики
- Язык программирования: JavaScript
- Библиотеки: 
  - Three.js (r127)
  - OrbitControls
  - dat.GUI
- Среда выполнения: Веб-браузер с поддержкой WebGL

### 2. Структура программы

#### 2.1 Основные компоненты
```javascript
// Инициализация Three.js
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, width/height, 0.1, 1000);
const renderer = new THREE.WebGLRenderer();

// Система координат
const axesHelper = createThickAxes(5);
const gridHelper = new THREE.GridHelper(10, 20);

// Объект буквы
const textGeometry = new THREE.TextGeometry();
const textMesh = new THREE.Mesh(textGeometry, material);

// Проекции
const projectionScenes = [
    {canvas: '#projOxy', camera: new THREE.OrthographicCamera()},
    {canvas: '#projOxz', camera: new THREE.OrthographicCamera()},
    {canvas: '#projOyz', camera: new THREE.OrthographicCamera()}
];
```

#### 2.2 Параметры преобразований
```javascript
const transformParams = {
    scale: 1,
    translateX: 0,
    translateY: 0,
    translateZ: 0,
    rotateX: 0,
    rotateY: 0,
    rotateZ: 0
};
```

### 3. Реализация основных функций

#### 3.1 Создание осей координат
```javascript
function createThickAxes(size = 5) {
    const axesGroup = new THREE.Group();
    // Создание осей X, Y, Z
    const createAxis = (color, start, end) => {
        const material = new THREE.MeshBasicMaterial({ color });
        const geometry = new THREE.CylinderGeometry(0.05, 0.05, size, 32);
        const axis = new THREE.Mesh(geometry, material);
        // Позиционирование и ориентация оси
        axis.position.copy(start.clone().add(end).multiplyScalar(0.5));
        axis.lookAt(end);
        axis.rotateX(Math.PI / 2);
        return axis;
    };
    
    // Добавление осей в группу
    axesGroup.add(createAxis(0xff0000, new THREE.Vector3(0,0,0), new THREE.Vector3(size,0,0)));
    axesGroup.add(createAxis(0x00ff00, new THREE.Vector3(0,0,0), new THREE.Vector3(0,size,0)));
    axesGroup.add(createAxis(0x0000ff, new THREE.Vector3(0,0,0), new THREE.Vector3(0,0,size)));
    
    return axesGroup;
}
```

#### 3.2 Обновление преобразований
```javascript
function updateTransform() {
    if (!textMesh) return;
    
    // Матрица преобразования
    const matrix = new THREE.Matrix4();
    
    // Применение преобразований
    textMesh.scale.set(
        transformParams.scale,
        transformParams.scale,
        transformParams.scale
    );
    
    textMesh.position.set(
        transformParams.translateX,
        transformParams.translateY,
        transformParams.translateZ
    );
    
    textMesh.rotation.set(
        transformParams.rotateX,
        transformParams.rotateY,
        transformParams.rotateZ
    );
    
    // Получение итоговой матрицы
    matrix.compose(
        textMesh.position,
        textMesh.quaternion,
        textMesh.scale
    );
    
    console.log('Матрица преобразования:', matrix.elements);
}
```



#### 4.2 Ортографические проекции
Матрицы проекций на координатные плоскости:
- Oxy: z = 0
- Oxz: y = 0
- Oyz: x = 0

### 5. Требования к системе

#### 5.1 Программные требования
- Современный веб-браузер с поддержкой WebGL
- Рекомендуемые браузеры:
  - Google Chrome 90+
  - Firefox 88+
  - Safari 14+

#### 5.2 Аппаратные требования
- Видеокарта с поддержкой WebGL 2.0
- Оперативная память: минимум 4 ГБ
- Процессор: Intel Core i3 или аналогичный

### 6. Тестирование

#### 6.1 Проверка корректности преобразований
1. Задать начальные координаты объекта
2. Применить последовательность преобразований
3. Сравнить полученную матрицу с ожидаемой
4. Проверить визуальное соответствие

#### 6.2 Проверка проекций
1. Проверить соответствие проекций координатным плоскостям
2. Убедиться в корректном масштабировании
3. Проверить обновление проекций при преобразованиях

### 7. Известные ограничения
- Возможны артефакты при экстремальных значениях масштабирования
- Производительность зависит от сложности 3D-модели
- Точность вращения ограничена точностью JavaScript


