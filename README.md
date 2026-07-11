# BookFlow

BookFlow is a C++17 console-based Library Management System. It manages books, readers, loans, reservations, search, filters, reports, exports, logs, and local file persistence.

## Features

- Add, list, show, edit, and delete books
- Register, list, and inspect readers
- Create and return book loans
- Create reservations
- Search books by title, author, ISBN, publisher, or category
- Filter by category, author, availability, and publication year
- Generate text reports
- Export books to CSV or JSON
- Store data locally in the `data/` directory
- Write logs to `data/logs.txt`
- Run unit-style integration tests through CTest

## Project Structure

```text
BookFlow/
├── CMakeLists.txt
├── README.md
├── include/
├── src/
├── tests/
├── data/
├── backups/
└── reports/
```

## Requirements

- CMake 3.16 or newer
- A C++17 compiler, such as MinGW g++

## Build

From the project directory:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

If your local MinGW Makefiles generator is broken, use the included Windows fallback script:

```bat
build_windows.bat
```

## Run

```bash
.\build\BookFlow.exe
```

The application starts with a command prompt:

```text
> help
```

## Main Commands

```text
help
book add
book list
book show <id>
book edit <id> <field> <value>
book delete <id>
reader add
reader list
reader show <id>
loan create
loan return <bookId>
reservation create
search <text>
filter category <name>
filter author <name>
filter available
filter year >2020
stats
report
export csv <path>
export json <path>
exit
```

## Example Workflow

```text
book add
reader add
loan create
book list
loan return 1
reservation create
stats
report
export json data/books_export.json
exit
```

## Testing

Build the project first, then run:

```bash
ctest --test-dir build --output-on-failure
```

You can also run the test executable directly:

```bash
.\build\BookFlowTests.exe
```

## Data Files

BookFlow stores application data in local files under `data/`:

- `books.json`
- `readers.json`
- `loans.json`
- `reservations.json`
- `categories.json`
- `settings.json`
- `logs.txt`

The internal persistence format is a compact escaped record format kept dependency-free for portability. User-facing exports support real CSV and JSON output.

## Notes

This project is intentionally dependency-free. It uses only the C++ standard library, which makes it easy to build on Windows with MinGW and CMake.
