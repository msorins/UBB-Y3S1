## LAB 5 -> part1

### Step 0
brew install flex

### Step I
Define types in myscanner.h

### Step II
Define types in myscanner.l

### Step III
Run flex myscanner.l

### Step IV
Create and run program that is going to use lex.yy.c, called myscanner.c

### Step V
Compile
gcc myscanner.c  -o myscanner

### Step VI
./myscanner
Then: main { int a = 1 | { cin a ;  a = a + 2} }


## LAB 5 -> part2

### Step 0
brew install bison

### Step 1
write myscanner.y

## Step 2
Run
bison -d myscanner.y
flex myscanner_yacc.l

## Step 3
gcc lex.yy.c myscanner.tab.c  -o myscanner

## Step 4
./myscanner main { int a = 1 | { cin a ;  a = a + 2} }
./myscanner main { int a = 1 | { cin a ;  a = a ++ 2} }