# Banker's Algorithm

-   교착 상태 회피 알고리즘

## Using Single Resource

### 컴파일

```
gcc banker-single-resource.c -o banker-single-resource
```

### 실행

```
.\banker-single-resource.exe
```

### 입력

```
Banker's Algorithm Using Single Resource.

Enter the currently available resource: 2

Enter the number of processes: 4
Enter the currently allocated resource (P1 P2 ...): 1 1 2 4
Enter the maximum resource required (P1 P2 ...): 6 5 4 7
```

### 출력

```
===== Table for Resource Allocation =====

Available resources: 2

Process   Allocation   Maximum    Need
   P1          1          6         5
   P2          1          5         4
   P3          2          4         2
   P4          4          7         3

=========================================

Safe sequence Found: 2 -> 3 -> 0 -> 1
Executing Processes...

[Process 2]
        Maximum   : 4
        Allocated : 2
        Need      : 2
        Available : 2
        2  >= 2   : true
        Resource allocated! 2
        Process code running...
        Process using resources...
        Process code completed...
        Process releasing resources...
        Resource released! 4
        Now Available : 4

[Process 3]
        Maximum   : 7
        Allocated : 4
        Need      : 3
        Available : 4
        4  >= 3   : true
        Resource allocated! 3
        Process code running...
        Process using resources...
        Process code completed...
        Process releasing resources...
        Resource released! 7
        Now Available : 8

[Process 0]
        Maximum   : 6
        Allocated : 1
        Need      : 5
        Available : 8
        8  >= 5   : true
        Resource allocated! 5
        Process code running...
        Process using resources...
        Process code completed...
        Process releasing resources...
        Resource released! 6
        Now Available : 9

[Process 1]
        Maximum   : 5
        Allocated : 1
        Need      : 4
        Available : 9
        9  >= 4   : true
        Resource allocated! 4
        Process code running...
        Process using resources...
        Process code completed...
        Process releasing resources...
        Resource released! 5
        Now Available : 10

All processes finished.
```

## Using Multiple Resources

### 컴파일

```
gcc banker-multiple-resources.c -o banker-multiple-resources
```

### 실행

```
.\banker-multiple-resources.exe
```

### 입력

```
Banker's Algorithm Using Multiple Resources.

Enter the number of resources: 3
Enter the currently available resources (R1 R2 ...): 3 3 2

Enter the number of processes: 5
Enter the currently allocated resource to Process 0 (R1 R2 ...): 0 1 0
Enter the currently allocated resource to Process 1 (R1 R2 ...): 2 0 0
Enter the currently allocated resource to Process 2 (R1 R2 ...): 3 0 2
Enter the currently allocated resource to Process 3 (R1 R2 ...): 2 1 1
Enter the currently allocated resource to Process 4 (R1 R2 ...): 0 0 2

Enter the maximum resource required by Process 0 (R1 R2 ...): 7 5 3
Enter the maximum resource required by Process 1 (R1 R2 ...): 3 2 2
Enter the maximum resource required by Process 2 (R1 R2 ...): 9 0 2
Enter the maximum resource required by Process 3 (R1 R2 ...): 2 2 2
Enter the maximum resource required by Process 4 (R1 R2 ...): 4 3 3
```

### 출력

```
===== Table for Resource Allocation =====

Available resources: 3 3 2

Process   Allocation   Maximum    Need
   P0        0 1 0      7 5 3     7 4 3
   P1        2 0 0      3 2 2     1 2 2
   P2        3 0 2      9 0 2     6 0 0
   P3        2 1 1      2 2 2     0 1 1
   P4        0 0 2      4 3 3     4 3 1

=========================================

Safe sequence Found: 1 -> 3 -> 4 -> 0 -> 2
Executing Processes...

[Process 1]
        Maximum   : 3 2 2
        Allocated : 2 0 0
        Need      : 1 2 2
        Available : 3 3 2
        3 3 2 >= 1 2 2 : true
        Resource allocated! 1 2 2
        Process code running...
        Process using resources...
        Process code completed...
        Process releasing resources...
        Resource released! 3 2 2
        Now Available : 5 3 2

[Process 3]
        Maximum   : 2 2 2
        Allocated : 2 1 1
        Need      : 0 1 1
        Available : 5 3 2
        5 3 2 >= 0 1 1 : true
        Resource allocated! 0 1 1
        Process code running...
        Process using resources...
        Process code completed...
        Process releasing resources...
        Resource released! 2 2 2
        Now Available : 7 4 3

[Process 4]
        Maximum   : 4 3 3
        Allocated : 0 0 2
        Need      : 4 3 1
        Available : 7 4 3
        7 4 3 >= 4 3 1 : true
        Resource allocated! 4 3 1
        Process code running...
        Process using resources...
        Process code completed...
        Process releasing resources...
        Resource released! 4 3 3
        Now Available : 7 4 5

[Process 0]
        Maximum   : 7 5 3
        Allocated : 0 1 0
        Need      : 7 4 3
        Available : 7 4 5
        7 4 5 >= 7 4 3 : true
        Resource allocated! 7 4 3
        Process code running...
        Process using resources...
        Process code completed...
        Process releasing resources...
        Resource released! 7 5 3
        Now Available : 7 5 5

[Process 2]
        Maximum   : 9 0 2
        Allocated : 3 0 2
        Need      : 6 0 0
        Available : 7 5 5
        7 5 5 >= 6 0 0 : true
        Resource allocated! 6 0 0
        Process code running...
        Process using resources...
        Process code completed...
        Process releasing resources...
        Resource released! 9 0 2
        Now Available : 10 5 7

All processes finished.
```
