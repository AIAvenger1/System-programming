/* Example code for the mini-compiler */
int x;
int y;
int z;

x = 10;
y = 20;

if (x < y) {
  z = y - x * 2;
} else {
  z = 0;
}

// This should print 0 because 20 - (10*2) = 0
print(z);

int a;
a = 5;
while (a > 0) {
    print(a);
    a = a - 1;
}