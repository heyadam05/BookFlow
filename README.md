# BookFlow

BookFlow is a dependency-free C++17 console library management system. It manages books, readers, loans, reservations, search, filters, reports, exports, logs, and local file persistence.

## Highlights

- Add, list, show, edit, and delete books.
- Register, list, and inspect readers.
- Create and return book loans.
- Create reservations.
- Search books by title, author, ISBN, publisher, or category.
- Filter by category, author, availability, and publication year.
- Generate text reports.
- Export books to CSV or JSON.
- Store data locally in the `data/` directory.
- Write logs to `data/logs.txt`.
- Run unit-style integration tests through CTest.

## Requirements

- CMake 3.16 or newer
- C++17 compiler, such as MinGW g++

## Build

From the project directory:

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
```

If your local MinGW Makefiles generator fails, use the Windows fallback script:

```bat
build_windows.bat
```

## Run

```powershell
.\build\BookFlow.exe
```

The application starts with a command prompt:

```text
> help
```

## Test

Build the project first, then run:

```bash
ctest --test-dir build --output-on-failure
```

You can also run the test executable directly:

```powershell
.\build\BookFlowTests.exe
```

## Commands

| Command | Description |
| --- | --- |
| `help` | Show command help and available commands |
| `book add` | Add a new book interactively |
| `book list` | List all books |
| `book show <id>` | Show details of a specific book |
| `book edit <id> <field> <value>` | Edit a book field |
| `book delete <id>` | Delete a book by ID |
| `reader add` | Register a new reader |
| `reader list` | List all readers |
| `reader show <id>` | Show details of a specific reader |
| `loan create` | Create a new book loan |
| `loan return <bookId>` | Return a borrowed book |
| `reservation create` | Create a reservation for a book |
| `search <text>` | Search books by title, author, ISBN, publisher, or category |
| `filter category <name>` | Filter books by category |
| `filter author <name>` | Filter books by author |
| `filter available` | Show only currently available books |
| `filter year >2020` | Filter books by publication year (for example, after 2020) |
| `stats` | Show library statistics |
| `report` | Generate a text report |
| `export csv <path>` | Export books to a CSV file |
| `export json <path>` | Export books to a JSON file |
| `exit` | Exit the application |

Example workflow:

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

## Data And Files

BookFlow stores application data under `data/`:

- `books.json`
- `readers.json`
- `loans.json`
- `reservations.json`
- `categories.json`
- `settings.json`
- `logs.txt`

The internal persistence format is a compact escaped record format kept dependency-free for portability. User-facing exports support CSV and JSON output.
