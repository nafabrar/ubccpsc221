#include <iostream>

int a = 7;
int b = 6;
int* c = &b;
void test( int& x, int y, int*& z ) {
    x++;
    y++;
   z= &a; }
int main() {
    test(a,b,c); 
    std::cout << a << " " << b << " " << *c << std::endl;
    return 0;
}
