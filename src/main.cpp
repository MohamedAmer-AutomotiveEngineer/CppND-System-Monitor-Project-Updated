#include "ncurses_display.h"
#include "system.h"

int main() {
  int Process::procIndex = 0;
  System system;
  
  NCursesDisplay::Display(system);
}