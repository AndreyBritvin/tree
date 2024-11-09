CC = g++

INCLUDE = -Iinclude -IMy_logging_system/include

CFLAGS = -lm -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer -Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr

all: tree.out

tree.out: build/main.o build/my_tree.o My_logging_system/build/my_log.o build/tree_dump.o
	@$(CC) $(CFLAGS) $(INCLUDE) build/main.o build/my_tree.o My_logging_system/build/my_log.o build/tree_dump.o -o tree.out

build/main.o: src/main.cpp
	@$(CC) $(CFLAGS) $(INCLUDE) -c src/main.cpp -o build/main.o

build/my_tree.o: src/my_tree.cpp include/my_tree.h
	@$(CC) $(CFLAGS) $(INCLUDE) -c src/my_tree.cpp -o build/my_tree.o

build/tree_dump.o: src/tree_dump.cpp include/my_tree.h
	@$(CC) $(CFLAGS) $(INCLUDE) -c src/tree_dump.cpp -o build/tree_dump.o

My_logging_system/build/my_log.o: My_logging_system/src/my_log.cpp My_logging_system/include/my_log.h
	@$(CC) $(CFLAGS) $(INCLUDE) -c My_logging_system/src/my_log.cpp -o My_logging_system/build/my_log.o

run:
	./tree.out

clean:
	rm -rf build
	mkdir build
	rm tree.out

