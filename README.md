# Pipe-Based IPC Demo (C)

A small Unix program showing two-way inter-process communication with `fork()` and two anonymous pipes.

## How it works
1. The **parent** scans the current directory for `.usp` files and sends each filename to the child over pipe 1.
2. The **child** opens each file, reads a name (line 1) and date of birth (line 2, `DD-MM-YYYY`), validates the date, works out the person's age and sends `Name:Age` back over pipe 2.
3. The parent appends each result to `result.txt`.

Files with an invalid date of birth are skipped with a message on stderr.

## Build and run
```bash
make
cd samples && ../age_calculator
cat result.txt
```

Sample input (`samples/sample1.usp`):
```
Alice
15-01-1999
```

## Concepts demonstrated
`fork`, `pipe`, low-level `open`/`read`/`write`, `opendir`/`readdir`, file permissions, `strtok`, Makefiles.
