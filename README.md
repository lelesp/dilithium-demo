# ML-DSA-65 Demo - Post-Quantum Digital Signatures

Демонстрационный проект постквантового алгоритма цифровой подписи **ML-DSA-65** (CRYSTALS-Dilithium) с клиент-серверной архитектурой.

## Архитектура

```
┌────────────────────────────────────────────────────────────────────────┐
│                            Docker                                      │
│  ┌──────────────────────────────────────────────────────────────┐    │
│  │                      dilithium-server                        │    │
│  │                         (FastAPI)                           │    │
│  │                                                             │    │
│  │  POST /api/sign ──► Генерация ключей ──► Подпись файла     │    │
│  └──────────────────────────────────────────────────────────────┘    │
└───────────────────────────────────────────────────────────────────────┘
                              │ REST API
                              │
              ┌───────────────┴───────────────┐
              │       Qt6 Client             │
              │                              │
              │  • Выбрать файл             │
              │  • Подписать ────────────▶  │
              │  • Загрузить ZIP            │
              │  • Верифицировать (liboqs)  │
              └──────────────────────────────┘
```

## Структура проекта

```
dilithium-demo/
├── server/                          # FastAPI сервер
│   ├── Dockerfile
│   ├── docker-compose.yml
│   ├── requirements.txt
│   └── app/
│       ├── main.py                  # API документация
│       ├── routes.py                # Эндпоинты
│       └── crypto/
│           └── dilithium.py          # ML-DSA-65 через dilithium-py
│
├── client/                          # Qt6 клиент
│   ├── CMakeLists.txt
│   ├── vcpkg.json
│   ├── Dockerfile                   # Arch Linux сборка
│   └── src/
│       ├── main.cpp
│       ├── mainwindow.ui
│       ├── mainwindow.cpp / .h
│       ├── apiclient.cpp / .h      # REST клиент
│       └── crypto.cpp / .h          # Верификация через liboqs
│
└── .github/
    └── workflows/
        └── build-windows.yml        # CI/CD для Windows
```

## Быстрый старт

### 1. Запуск сервера

#### Windows (нативно, без Docker)

Требуется только **Python 3.11+** ([python.org](https://www.python.org/)). Redis и Docker не нужны.

```bat
cd dilithium-demo\server
run-server.bat
```

Скрипт создаст виртуальное окружение, установит зависимости и запустит сервер.

#### Linux/macOS (Docker)

```bash
cd dilithium-demo/server
docker compose up -d
```

#### Linux/macOS (нативно)

```bash
cd dilithium-demo/server
python3 -m venv .venv && source .venv/bin/activate
pip install -r requirements.txt
uvicorn app.main:app --host 0.0.0.0 --port 8000
```

Сервер доступен: `http://localhost:8000`

Документация API:
- Swagger UI: http://localhost:8000/docs
- ReDoc: http://localhost:8000/redoc

> **Примечание:** в `docker-compose.yml` объявлен Redis, но операция подписи его
> не использует — для демонстрации сервер запускается полностью самостоятельно.

### 2. Клиент

#### Linux/macOS

```bash
cd dilithium-demo/client

# Arch Linux
sudo pacman -S qt6-base liboqs libzip cmake

# Ubuntu/Debian
sudo apt install qt6-base-dev liboqs-dev libzip-dev cmake build-essential

# Сборка
mkdir build && cd build
cmake ..
make
./DilithiumClient
```

#### Windows

Самый простой путь — **скачать готовую сборку** (компилировать ничего не нужно):

1. Откройте страницу [Releases](https://github.com/lelesp/dilithium-demo/releases) →
   релиз **«Windows build (latest)»** → скачайте `DilithiumClient-Windows.zip`.
   (Либо вкладка **Actions** → последний успешный запуск *Build Windows Executable* →
   раздел **Artifacts**.)
2. Распакуйте архив и запустите `DilithiumClient.exe` — все нужные DLL (Qt, liboqs,
   libzip) уже лежат рядом.

Сборка собирается автоматически через GitHub Actions (prebuilt Qt + vcpkg для
liboqs/libzip) при каждом пуше в `main` или вручную через
**Actions → Build Windows Executable → Run workflow**.

<details>
<summary>Собрать самостоятельно на Windows (опционально)</summary>

Требуется: Visual Studio 2022 (C++), CMake, [vcpkg](https://github.com/microsoft/vcpkg)
и Qt 6 (например через [aqtinstall](https://github.com/miurahr/aqtinstall) или
официальный установщик Qt).

```powershell
vcpkg install liboqs:x64-windows libzip:x64-windows

cmake -S client -B build -G "Visual Studio 17 2022" -A x64 `
  -DCMAKE_TOOLCHAIN_FILE="<vcpkg>/scripts/buildsystems/vcpkg.cmake" `
  -DVCPKG_TARGET_TRIPLET=x64-windows `
  -DCMAKE_PREFIX_PATH="<путь-к-Qt>/6.7.2/msvc2019_64"

cmake --build build --config Release
windeployqt build\Release\DilithiumClient.exe
```
</details>

### 3. Использование

1. **Запустите клиент**
2. **Настройте URL сервера** (по умолчанию: `http://localhost:8000`)
3. **Подпишите файл**:
   - Выберите файл через "Select File"
   - Нажмите "Sign File (via Server)"
   - Сохраните ZIP-архив
4. **Проверьте подпись**:
   - Загрузите ZIP через "Load Package"
   - Нажмите "Verify Signature"

## Структура ZIP архива

```
archive.zip
├── document.pdf          # Оригинальный файл (не изменён)
├── document.pdf.sig      # Подпись (hex)
└── document.pdf.pub      # Публичный ключ (hex)
```

## API эндпоинты

| Метод | Endpoint | Описание |
|-------|----------|----------|
| `GET` | `/` | Информация о сервисе |
| `POST` | `/api/sign` | Подпись файла → ZIP архив |
| `GET` | `/api/health` | Статус сервера |

## Алгоритм

**ML-DSA-65** — постквантовый алгоритм цифровой подписи (модульные решётки), стандартизирован NIST в 2024 году.

| Параметр | Значение |
|----------|---------|
| Безопасность | ~192 бита |
| Публичный ключ | 1952 байта (3864 hex) |
| Подпись | 3293 байта (6586 hex) |

## Зависимости

### Сервер
- Python 3.11+
- FastAPI
- dilithium-py (ML-DSA-65)

### Клиент
- Qt6
- liboqs (верификация)
- libzip (чтение архивов)
- CMake 3.16+

## CI/CD

GitHub Actions автоматически собирает Windows executable при пуше в main.

Скачать релиз: https://github.com/lelesp/dilithium-demo/releases
