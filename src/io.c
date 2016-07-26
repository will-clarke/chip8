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
    if(cpu->keyboard[i] > 0) cpu->keyboard[i] -= 1;

  char input = getch();
  switch(input){
  case('1'): cpu->keyboard[0x0] = 10; break;
  case('2'): cpu->keyboard[0x1] = 10; break;
  case('3'): cpu->keyboard[0x2] = 10; break;
  case('4'): cpu->keyboard[0x3] = 10; break;
  case('q'): cpu->keyboard[0x4] = 10; break;
  case('w'): cpu->keyboard[0x5] = 10; break;
  case('e'): cpu->keyboard[0x6] = 10; break;
  case('r'): cpu->keyboard[0x7] = 10; break;
  case('a'): cpu->keyboard[0x8] = 10; break;
  case('s'): cpu->keyboard[0x9] = 10; break;
  case('d'): cpu->keyboard[0xA] = 10; break;
  case('f'): cpu->keyboard[0xB] = 10; break;
  case('z'): cpu->keyboard[0xC] = 10; break;
  case('x'): cpu->keyboard[0xD] = 10; break;
  case('c'): cpu->keyboard[0xE] = 10; break;
  case('v'): cpu->keyboard[0xF] = 10; break;
  case(27):
  case('*'): end_ncurses(); exit(0);
  }
}

void output_display(struct cpu* cpu, WINDOW* window){
  //   row1, row2
  // [[64..],[64..]...]
  for(int y = 0; y < 32; y++){
    move(y, 0);
    for(int x = 0; x < 64; x++)
      addch(cpu->display[y * 64 + x] ? ' ' | A_REVERSE : ' ');
  }
  refresh();
}
