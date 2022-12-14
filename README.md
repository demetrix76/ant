# Тестовое задание #

Необходимо реализовать решение задачи на С++.
На бесконечной координатной сетке находится муравей. Муравей может перемещаться на 1 клетку вверх (x,y+1), вниз (x,y-1), влево (x-1,y), вправо (x+1,y), по одной клетке за шаг.

Клетки, в которых сумма цифр в координате X плюс сумма цифр в координате Y больше, чем 25, недоступны муравью. Например, клетка с координатами (59, 79) недоступна, т.к. 5+9+7+9=30, что больше 25.

Сколько клеток может посетить муравей, если его начальная позиция (1000,1000), (включая начальную клетку)?

## Сборка проекта ##
```cmake /path/to/source```

```cmake --build . --config Release```

(требуется компилятор с поддержкой C++17)

Опционально, возможна визуализация задачи. Для этого требуются библиотеки ZLib и LibPNG. Также нужно включить опцию USE_LIBPNG в настройках CMake проекта:

```cmake -DUSE_LIBPNG=ON /path/to/source```

В таком случае PNG-картинка будет выводиться в стандартный вывод, и её можно сохранить

```./Ant > preview.png```

(под Windows, разумеется, ./ нужно опустить)

или, например, сразу отобразить, если установлен ImageMagick:

```./Ant | display -```

На этом изображении клетки, доступные для посещения, отмечены белым цветом; недоступные - чёрным.
Клетки, которые смог посетить наш герой — зелёным.
