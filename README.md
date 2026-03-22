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

```bash
cd dilithium-demo/server
docker compose up -d
```

Сервер доступен: `http://localhost:8000`

Документация API:
- Swagger UI: http://localhost:8000/docs
- ReDoc: http://localhost:8000/redoc

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

Скачайте готовый релиз из GitHub Actions или соберите самостоятельно:

```powershell
git clone https://github.com/lelesp/dilithium-demo.git
cd dilithium-demo
.\.github\workflows\build-windows.yml
```

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
