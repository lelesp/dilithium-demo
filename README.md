# Dilithium Demo - Post-Quantum Digital Signatures

Демонстрационный проект постквантового алгоритма цифровой подписи **Dilithium3** с клиент-серверной архитектурой.

## Архитектура

```
┌─────────────────────────────────────────────────────────────────────┐
│                            Docker                                   │
│  ┌──────────────────┐         ┌──────────────────┐                │
│  │  dilithium-app   │◄────────►│      redis       │                │
│  │     (FastAPI)   │         │  (хранение ключей│                │
│  │                  │         │   и подписей)    │                │
│  └────────┬─────────┘         └──────────────────┘                │
└───────────│─────────────────────────────────────────────────────────┘
            │ REST API
    ┌───────┴───────┐
    │  Qt6 Client   │
    │               │
    │ • Загрузить файл                                              │
    │ • Подписать ───────▶ POST /api/sign ──▶ ZIP: {file, .sig}   │
    │ • Проверить ───────▶ GET /api/keys/{id} + локальная liboqs  │
    └───────────────┘
```

## Структура проекта

```
dilithium-demo/
├── server/
│   ├── Dockerfile
│   ├── docker-compose.yml
│   ├── requirements.txt
│   └── app/
│       ├── main.py
│       ├── routes.py
│       ├── config.py
│       ├── crypto/
│       │   └── dilithium.py
│       ├── storage/
│       │   └── redis_client.py
│       └── models/
│           └── schemas.py
│
├── client/
│   ├── CMakeLists.txt
│   ├── Dockerfile
│   └── src/
│       ├── main.cpp
│       ├── mainwindow.ui
│       ├── mainwindow.cpp / .h
│       ├── apiclient.cpp / .h
│       └── crypto.cpp / .h
│
└── README.md
```

## Быстрый старт

### 1. Запуск сервера

```bash
cd dilithium-demo/server
docker-compose up -d
```

Сервер будет доступен по адресу: `http://localhost:8000`

**Документация API:**
- Swagger UI: http://localhost:8000/docs
- ReDoc: http://localhost:8000/redoc

### 2. Сборка клиента

#### Linux/macOS

```bash
cd dilithium-demo/client

# Установка зависимостей (Arch Linux)
sudo pacman -S qt6-base liboqs

# Сборка
mkdir build && cd build
cmake ..
make
```

#### Windows

1. Установите Qt6 и liboqs
2. Откройте проект в Qt Creator
3. Соберите проект

### 3. Использование

1. **Запустите клиент** (`DilithiumClient`)
2. **Настройте URL сервера** (по умолчанию: `http://localhost:8000`)
3. **Сгенерируйте ключи**: нажмите "Generate Key Pair"
4. **Подпишите файл**:
   - Выберите файл через "Select File"
   - Нажмите "Sign File (via Server)"
   - Сохраните ZIP-архив
5. **Проверьте подпись** (на другом клиенте):
   - Загрузите ZIP через "Load ZIP"
   - Введите Key ID
   - Нажмите "Get Public Key"
   - Нажмите "Verify Signature"

## API эндпоинты

| Метод | Endpoint | Описание |
|-------|----------|----------|
| `GET` | `/` | Информация о сервисе |
| `POST` | `/api/keys/generate` | Генерация ключей → `{key_id}` |
| `GET` | `/api/keys/{key_id}` | Публичный ключ (hex) |
| `POST` | `/api/sign` | Подпись файла → ZIP архив |
| `GET` | `/health` | Статус сервера |

## Формат подписи (.sig файл)

```json
{
  "key_id": "uuid-123",
  "algorithm": "Dilithium3",
  "signature": "base64...",
  "original_filename": "document.pdf",
  "timestamp": "2026-03-22T12:00:00Z"
}
```

## Алгоритм

**Dilithium3** — постквантовый алгоритм цифровой подписи, основанный на задаче Learning With Errors (LWE) в модульных решётках. Стандартизирован NIST в 2024 году.

- **Уровень безопасности**: 192 бита
- **Размер ключа**: ~1952 байта
- **Размер подписи**: ~3293 байта

## Зависимости

### Сервер
- Python 3.11+
- FastAPI
- Redis
- crystals-dilithium

### Клиент
- Qt6
- liboqs
- CMake 3.16+
