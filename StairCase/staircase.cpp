#include <iostream> 

int main(); 

int main () {
  double x{}; 
  int stair{}; 
  int a{0}; 
  int b{0}; 
  std::cout << "Enter value of x: " ;
  std::cin >> x; 
  if (x >= 0) {
    for (double k{x}; k>=0.0; --k) {
      a+=1; 
    }
    stair = a-1;
  }
  else {
    for (double l{x}; l<0.0; l++) {
      b+=1;
    }
    stair = (-1)*(b); 
  }
  std::cout << "The value of stair(x) is:"<< stair << std::endl; 
  return 0; 
}