#include <cstdlib>
#include <iostream>
#include <cassert>
#include <utility>
#include "SharedPtr.h"

int main() {
   // 1) Basic test with initialization
   SharedPtr<int> ptr1 = new int(5);
   assert(*ptr1 == 5);

   // 2) Copying
   SharedPtr<int> ptr2 = ptr1;
   SharedPtr<int> ptr5 = SharedPtr<int>(ptr2);
   assert(ptr1 == ptr2);
   assert(ptr2 == ptr5);

   // 3) Equality operator
   SharedPtr<int> ptr3 = new int(12);
   assert(!(ptr1 == ptr3));

   // 4) Swapping + equality operator
   ptr1.swap(ptr3);
   assert(ptr3 == ptr2);

   // 5) Reset pointer to be empty + boolean operator
   assert(ptr1);
   ptr1.reset();
   assert(!ptr1);

   //6) Moving
   SharedPtr<int> ptr4 = std::move(ptr2);
   assert(ptr4 == ptr3);
   assert(!ptr2);
   SharedPtr<int> ptr6 = SharedPtr<int>(std::move(ptr4));
   assert(ptr6 == ptr3);
   assert(!ptr4);

   return EXIT_SUCCESS;
}