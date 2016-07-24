#include <stdlib.h>
#include "io.h"

void clear_screen()
{
  // noop yet
}

void load_rom(struct cpu* cpu, int argc, char *argv[]){
  if(argc != 2){
    printf("\nUsage: ./chip8 pong\n\tThe first argument should be a rom name.\n\n");
    exit(0);
  }
  char *rom_name = argv[1];
  /* const char *dir_path = "roms/\0"; */
  /* uint8_t char_count = strlen(rom_name) + strlen(dir_path); */
  /* char file_path[char_count]; */
  /* strcpy(file_path, dir_path); */
  /* strcat(file_path, rom_name); */
  /* const char * filename = file_path; */
  FILE *rom = fopen(rom_name, "rb");
    if(rom == NULL){
      printf("OMG. No file pointer to rom!\n");
      printf("Rom filename = %s\n", rom_name);
      exit(1);
    }
  char byte;
  uint16_t index = 0;
  while(!feof(rom)){
    fread(&byte, sizeof(char), 1, rom);
    cpu->memory[0x200 + index] = byte;
      index++;
  }
  fclose(rom);
}

WINDOW * setup_ncurses(){
  WINDOW* window = initscr();
  noecho();
  cbreak();
  curs_set(0);
  timeout(00001);
  keypad(window, TRUE);

  return window;
}

void end_ncurses(){
  endwin();
}

void process_keyboard_input(struct cpu* cpu, WINDOW* window){
  for(int i = 0; i < 0x10; i++)
    cpu->keyboard[i] = 0;

  /* initscr(); */
  char input = getch();
  /* char input = 'w'; */
  /* char input; */
  switch(input){
  case('1'): cpu->keyboard[0x1] = 1; break;
  case('2'): cpu->keyboard[0x2] = 1; break;
  case('3'): cpu->keyboard[0x2] = 1; break;
  case('4'): cpu->keyboard[0x2] = 1; break;
  case('q'): cpu->keyboard[0x2] = 1; break;
  case('w'): cpu->keyboard[0x2] = 1; break;
  case('e'): cpu->keyboard[0x2] = 1; break;
  case('r'): cpu->keyboard[0x2] = 1; break;
  case('a'): cpu->keyboard[0x2] = 1; break;
  case('s'): cpu->keyboard[0x2] = 1; break;
  case('d'): cpu->keyboard[0x2] = 1; break;
  case('f'): cpu->keyboard[0x2] = 1; break;
  case('z'): cpu->keyboard[0x2] = 1; break;
  case('x'): cpu->keyboard[0x2] = 1; break;
  case('c'): cpu->keyboard[0x2] = 1; break;
  case('v'): cpu->keyboard[0x2] = 1; break;
  case(27):
  case('*'): end_ncurses(); exit(0);
  }
}

void output_display(struct cpu* cpu, WINDOW* window){

  unsigned char i, j;

  for (j = 0; j < 32; j++) {
    move(j, 0);
    for (i = 0; i < 64; i++) {
      addch(cpu->display[32 * i + j] ? ' ' | A_REVERSE : ' ');
    }
  }
  refresh();


}
