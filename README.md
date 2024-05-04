## 3D_Viewer

Программа разработана с использованием фреймворка Qt.
Программа позволяет откравать и просматривать файлы каркасных 3D-моделей с
расширением `.obj`. Возможности программы позволяют:
	
- Перемещать модель по осям x, y, z 
- Вращать модель вокруг осей x, y, z 
- Масштабировать модель 
- Менять ширину, цвет и тип линий 
- Менять размер, цвет и тип форму вершин 
- Менять цвет фона 
- Сохранять скриншоты зоны визуализации в форматах `.bmp`  и `.jpeg` 
- Сохранять gif-анимации текущих пользовательских преобразований модели 
- Сохранять текущее состояние параметров сцены между сессиями 
	

# Установка программы
Установка программы выполняется с помощью команды `make install` и производится в каталог SmartCalc на рабочий стол пользователя.

# Удаление программы
Удаление программы выполняется с помощью команды `make uninstall`.

# Архивация
Команда `make dist` позволяет произвести архивацию всей исходной конфигурации программы.
