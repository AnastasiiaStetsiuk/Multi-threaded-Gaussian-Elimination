# Multi-threaded-Gaussian-Elimination


This project demonstrates a multi-threaded application of the Gaussian elimination algorithm using the "Peer-to-Peer" model and synchronization techniques with Win32 API in C++. The program performs Gaussian elimination on a matrix using multiple threads to parallelize the computation.

## Features

- **Multi-threaded Gaussian Elimination:** Divides the matrix processing among multiple threads.
- **Synchronization:** Uses events and mutexes for thread synchronization and console output management.
- **Peer-to-Peer Model:** Implements a synchronization scheme where threads signal each other to coordinate their execution.

## Requirements

- **Operating System:** Windows (as it uses Win32 API for threading and synchronization)
- **Compiler:** A C++ compiler supporting C++11 or later (e.g., MSVC, MinGW)

## Usage

1. **Compile the Program:**

   To compile the program, use a C++ compiler that supports Win32 API. For example, using MSVC:

   ```sh
   cl /EHsc /MD ConsoleApplication4.cpp
   ```

   Or using MinGW:

   ```sh
   g++ -o gaussian_elimination ConsoleApplication4.cpp -lgdi32
   ```

2. **Run the Executable:**

   Execute the compiled program:

   ```sh
   gaussian_elimination.exe
   ```

3. **Output:**

   The program will display the progress of each thread and the final matrix after Gaussian elimination.

## Code Overview

- **ThreadData Struct:** Contains data required by each thread, including the matrix, synchronization objects, and thread identifiers.
- **GaussianElimination Function:** The function executed by each thread. It performs Gaussian elimination on the matrix and handles synchronization using events and mutexes.
- **Main Function:** Sets up the threads, initializes synchronization objects, and starts the Gaussian elimination process. After completion, it outputs the final matrix.

## Example

Here is a brief example of how the matrix is transformed during execution:

```
Initial matrix:
3 2 -4 3
2 3 3 15
5 -3 1 14

Progress output by threads (example):
Thread 0 starts step 0
Thread 0 finished step 0
Thread 1 starts step 0
...

Final matrix:
...
```

## Contributing

Contributions are welcome! If you have suggestions or improvements, please create a pull request or open an issue.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
