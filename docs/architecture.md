# BookFlow Architecture

BookFlow uses a simple layered design:

- `Application` handles console input and output.
- `Library` owns business rules and coordinates all entities.
- `FileManager` loads, saves, backs up, and exports data.
- `SearchEngine` performs book search and filtering.
- `ReportGenerator` creates statistics and report files.
- Model structs represent books, readers, loans, reservations, categories, and settings.

The code avoids external dependencies so the project remains easy to build in a school or portfolio environment.
