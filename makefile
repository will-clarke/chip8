build_command = gcc -o a.out cpu.h


build: cpu.h
	$(build_command)

debug: cpu.h
	$(build_command) -DDEBUG

test:
	something
