#include "kernel.h"

#include <iostream.h>

extern int userMain(int argc, char* argv[]);


int main(int argc, char* argv[]){

  Kernel::init_Kernel();

  int v = userMain(argc , argv);

  Kernel::termin_Kernel();

  cout<<"Konacan kraj: "<< v <<endl;

  return v;

}