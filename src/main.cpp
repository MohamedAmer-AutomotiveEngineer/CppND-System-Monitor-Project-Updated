#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  int Process::procIndex = 0;
  NCursesDisplay::Display(system);
}