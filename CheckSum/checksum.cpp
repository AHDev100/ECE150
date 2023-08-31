#include <iostream>
#include <climits>

#ifndef MARMOSET_TESTING
int main();
#endif

unsigned int add_checksum( unsigned int n );
void add_checksum( unsigned int array[], std::size_t capacity );
bool verify_checksum( unsigned int n );
unsigned int remove_checksum( unsigned int n );
void remove_checksum( unsigned int array[], std::size_t capacity );

#ifndef MARMOSET_TESTING
int main() {
    unsigned int value_to_protect{1};
    unsigned int protected_value = add_checksum(1);
    std::cout << "The value " << value_to_protect
    << " with the checksum added is " << protected_value
    << "." << std::endl;
    if (verify_checksum(protected_value))
    {
        std::cout << "The checksum is valid." << std::endl;
    }
    else {
        std::cout << "The checksum is invalid." << std::endl;
    }
    const std::size_t QTY_VALUES {3};
    unsigned int value_series[QTY_VALUES] {20201122, 20209913, 20224012};
    add_checksum(value_series, QTY_VALUES);
    std::cout << "Series with checksums added: ";
    for (std::size_t series_index {0}; series_index < QTY_VALUES; series_index++)
    {
        std::cout << value_series[series_index] << " ";
    }
    std::cout << std::endl;
    return 0;
    }
#endif

unsigned int add_checksum(unsigned int n){ //This works
  int to_divide_by{1};
  int other_divide{10}; 
  int div1{}; 
  int div2{}; 
  int rem{}; 
  int rem2{};
  int mult_one{};
  int mult_two{};
  int mult_total{}; 
  int checksum{};
  if (n < 0 || n > 99999999) {
    return UINT_MAX; 
  }
  else {
    while (n >= to_divide_by) {
      div1 = n/to_divide_by; 
      div2 = n/other_divide; 
      rem = div1%10; 
      rem2 = div2%10; 
      to_divide_by *= 100;
      other_divide *= 100;
      if (2*rem > 9) {
        mult_two += (((2*rem)/10) +((2*rem)%10)); 
      }
      else {
        mult_two += 2*rem;
      }
      if (rem2 > 9) {
        mult_one += (((rem2)/10) +((rem2)%10)); 
      }
      else {
        mult_one += rem2;
      }
    }
    mult_total = mult_one + mult_two;
    checksum = (mult_total*9) % 10; 
    return (n*10) + checksum; 
  }    
} 

void add_checksum( unsigned int array[], std::size_t capacity ){ //This works
  for (int i{0}; i<capacity; i++) {
    array[i] = add_checksum(array[i]); 
  }
}

bool verify_checksum(unsigned int n){ //This works
  if (n <= 999999999 && add_checksum(n/10) == n) {
    return true; 
  }
  else {
    return false; 
  }
}

unsigned int remove_checksum( unsigned int n ){ //This works
  if (n > 999999999 || n != add_checksum(n/10)) {
    return UINT_MAX; 
  }
  else {
    return n/10; 
  }
}

void remove_checksum( unsigned int array[], std::size_t capacity ){ //This works
  for (int i{0}; i < capacity; i++) {
    array[i] = remove_checksum(array[i]);
  }
}