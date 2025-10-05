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
