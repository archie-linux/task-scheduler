## Run the program 

```bash
g++ --std=c++17 TaskScheduler.cpp -o TaskScheduler
MacBook-Air:task-scheduler anish$ ./TaskScheduler
```

```bash
ID: 1, Description: Write report, Priority: 2, Deadline: 2025-06-10
ID: 2, Description: Review code, Priority: 1
Executing: Write report (ID: 1)
Executing: Review code (ID: 2)
MacBook-Air:task-scheduler anish$ cat tasks.txt 
1|Write report|2|2025-06-10|completed
2|Review code|1|none|completed
#DEPENDENCIES
2->1
```

## Build and Run

- GTest Installer Script: https://gist.github.com/butuzov/e7df782c31171f9563057871d0ae444a

```bash

CMake Error at CMakeLists.txt:1 (cmake_minimum_required):
  Compatibility with CMake < 3.5 has been removed from CMake.

  Update the VERSION argument <min> value.  Or, use the <min>...<max> syntax
  to tell CMake that the project requires at least <min> but has been updated
  to work with policies introduced by <max> or earlier.

  Or, add -DCMAKE_POLICY_VERSION_MINIMUM=3.5 to try configuring anyway.
```

- Add Flag here:- cmake -Dgtest_build_samples=OFF -Dgtest_build_tests=OFF -DCMAKE_POLICY_VERSION_MINIMUM=3.5 ../


```bash
g++ -std=c++17 test_TaskScheduler.cpp -lgtest -lgtest_main -pthread -o test_tasks
./test_tasks
```
