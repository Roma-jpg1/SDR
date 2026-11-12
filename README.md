<p align="center">
  <img src="docs/images/satellite-orbit.gif" alt="Орбитальные спутники над Землёй" width="100%">
</p>

# Производственная практика - SDR
## Петров Роман Юрьевич

## Структура проекта

```md
"SDR"
├── docs/
│   ├── images/         # Иллюстрации, графики и спектрограммы
│   ├── report1.md      # Отчет №1
│   ├── report2.md      # Отчет №2
│   └── report3.md      # Отчет №3
├── include/
│   ├── config.h        # Частоты, SPS, фильтр h и режим модуляции
│   ├── modulate.h      # BPSK, QPSK, апсэмплинг и свертка
│   ├── sdr.h           # Инициализация PlutoSDR
│   └── tx_rx.h         # Прием и передача
├── src/
│   ├── main.cpp        # Запуск rx/tx режимов
│   ├── modulate.cpp    # Генерация бит, BPSK/QPSK и свертка
│   ├── plot.py         # Отрисовка I/Q
│   ├── sdr.cpp         # Создание устройства в SoapySDR
│   └── tx_rx.cpp       # Прием, отправка и сохранение сэмплов
├── thirdparty/         # Сторонние проекты и библиотеки
│   ├── libad9361-iio
│   ├── libiio
│   ├── imgui
│   ├── implot
│   ├── SoapyPlutoSDR
│   └── SoapySDR
├── CMakeLists.txt
├── .gitmodules
├── .gitignore
├── README.md
└── и пр.
```

## Сборка и запуск проекта

### 1. Требования и зависимости
Для работы проекта необходимы компилятор C++, CMake, Python3, а также библиотеки для взаимодействия с Adalm-Pluto (libiio, libad9361-iio, SoapySDR и драйвер SoapyPlutoSDR).

Установка основных пакетов в Ubuntu/Debian:
```bash
sudo apt update
sudo apt install -y build-essential cmake git libusb-1.0-0-dev libxml2-dev libbison-dev flex libcdk5-dev python3-numpy python3-matplotlib
```

### 2. Инициализация сабмодулей
```bash
git submodule update --init --recursive
```

### 3. Сборка зависимостей (thirdparty)
1. **libiio**:
   ```bash
   cd thirdparty/libiio
   cmake -B build -DENABLE_IPV6=OFF
   cmake --build build -j$(nproc)
   sudo cmake --install build
   sudo ldconfig
   cd ../..
   ```

2. **libad9361-iio**:
   ```bash
   cd thirdparty/libad9361-iio
   cmake -B build
   cmake --build build -j$(nproc)
   sudo cmake --install build
   sudo ldconfig
   cd ../..
   ```

3. **SoapySDR**:
   ```bash
   cd thirdparty/SoapySDR
   cmake -B build
   cmake --build build -j$(nproc)
   sudo cmake --install build
   sudo ldconfig
   cd ../..
   ```

4. **SoapyPlutoSDR**:
   ```bash
   cd thirdparty/SoapyPlutoSDR
   cmake -B build
   cmake --build build -j$(nproc)
   sudo cmake --install build
   sudo ldconfig
   cd ../..
   ```

### 4. Сборка проекта
```bash
mkdir build
cd build 
cmake ..
make
```

---

### 5. Поиск устройств и запуск
#### Определение URI подключенных устройств
Для поиска подключенных плат Adalm-Pluto выполните:
```bash
SoapySDRUtil --find
```
В выводе команды найдите URI нужных устройств.

#### Запуск программы
Синтаксис запуска:
```bash
cd build
./main <rx|tx> [uri]
```

### 6. Визуализация принятых сэмплов
Для отображения графиков I/Q компонент и амплитуды записанного сигнала:
1. Убедитесь, что файл `rxdata.pcm` находится по пути `./build/rxdata.pcm`:
2. Запустите скрипт визуализации:
   ```bash
   python3 src/plot.py
   ```

## Практические работы
|№|Задание|Отчет|Дата сдачи|
|:-:|-|:-:|:-:|
|1|Построение радио-приёмника.|docs/report1.md|15.09.26|
|2|Прием-передача сэмплов на 2 SDR.|docs/report2.md|20.10.26|


## Занятия
|№|Дата занятия|Тема лекции|Тема практики|
|:-:|--|--|--|
|1|08.09.26|Основы структурной схемы цифровой радиосвязи.|Архитектура Adalm Pluto SDR. GNU Radio. Построение радио-приёмника|
|2|15.09.26|Основы структурной схемы цифровой радиосвязи|GNU Radio. Построение радио-приёмника, радио-передатчика.|
|3|22.09.26|Введение в архитектуру SDR-устройств| Знакомство с библиотеками Soapy SDR, Libio для работы с Adalm Pluto SDR. Инициализация SDR-устройства. Работа с буфером: получение цифровых IQ-отсчетов.|
|4|29.09.26|Введение в архитектуру SDR-устройств| Знакомство с библиотеками Soapy SDR, Libio для работы с Adalm Pluto SDR. Инициализация SDR-устройства. Работа с буфером: получение цифровых IQ-отсчетов. Продолжение.|
|5|06.10.26|Введение в архитектуру SDR-устройств|Работа с буфером: получение цифровых IQ-отсчетов. Прием-передача сэмплов на 2 SDR. Продолжение.|
|6|13.10.26|Введение в архитектуру SDR-устройств|Работа с буфером: получение цифровых IQ-отсчетов. Прием-передача сэмплов на 2 SDR. Свертка|
|7|20.10.26| - |Работа с буфером: получение цифровых IQ-отсчетов. Прием-передача сэмплов на 2 SDR.|
|8|27.10.26|Приёмник: фильтрация, синхронизация и демодуляция.|Передатчик: биты -> модуляция -> upsampling -> свертка -> TX. Начало приемника.|
|9|03.11.26|Приёмник: фильтрация, синхронизация и демодуляция.|Приемник. Выбор символьных отсчётов и их визуализация после согласованного фильтра|
|10|10.11.26|-|Приемник. Выбор символьных отсчётов и их визуализация после согласованного фильтра. Добавление визуального интерфейса ImGUI для визуализации данных с AdalmPluto.|
|11|17.11.26| | |
|12|24.11.26| | |
|13|01.12.26| | |
|14|08.12.26| | |
|15|15.12.26| | |
|16|22.12.26| | |
|17|29.12.26| | |

<!-- | | | | | -->
