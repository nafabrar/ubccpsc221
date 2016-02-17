#include <iostream>

int main() {
  int x = 5;
  int y = 15;
  int* p1;
  int* p2;

  p1 = &x;
  p2 = &y;

  *p1 = 6;

  *p1 = *p2;

  p2 = p1;

  *p1 = *p2+10;

  return 0;
}
