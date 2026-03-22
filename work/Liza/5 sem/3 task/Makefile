OPTS = -fstack-protector-all -W -Wall -Wextra -Wunused -Wcast-align -Werror -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith -Wformat-security -Wmissing-format-attribute -Wformat=1 -Wwrite-strings -Wcast-align -Wno-long-long -std=gnu99 -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wold-style-definition -Wdeclaration-after-statement -Wbad-function-cast -Wnested-externs -O3 -g -lpthread

ALL = a.out
all: $(ALL)

%.out: %.o matrix_io.o solve1.o
	gcc $(OPTS) $^ -o $@
%.o: %.c
	gcc -c $(OPTS) $<
clean: 
	rm -f *.o all
