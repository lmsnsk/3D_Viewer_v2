TARGET=3D_Viewer
FLAGS=-Wall -Werror -Wextra -std=c++17
CC=g++ $(FLAGS)
FILE=s21_3D_Viewer
MODULES=modules/*
CONTROLLER=controller/*
FILETEST=tests/*
FILEREPORT=s21_test_report
FILETESTO=s21_test
DIRREPORT=report
COVFLAG=-fprofile-arcs -ftest-coverage
ASAN=-g -fsanitize=address

OS = $(shell uname)

ifeq ($(OS), Darwin)
	TESTFLAGS=-pthread -lcheck -lgtest -lstdc++
	OPEN_CMD=open
else
	TESTFLAGS=-pthread -lcheck_pic -lrt -lsubunit -lgtest -lstdc++
	OPEN_CMD=xdg-open
endif


all: clean gcov_report install

#####################################
# all: temp

# temp:
# 	$(CC) -fsanitize=address -g modules/*.c  dev_main/*.c $(TESTFLAGS)

# comp:
# 	$(CC) modules/*.c  dev_main/*.c $(TESTFLAGS)

# val:
# 	valgrind --tool=memcheck --leak-check=yes --track-origins=yes -s ./a.out
#####################################

install:
	mkdir ./build
	mkdir ./app
	cd ./build && qmake ../view/ && make && cp viewer ../app/$(TARGET).app
	
uninstall:
	rm -rf ./app ./build

dist:
	mkdir dist
	mkdir dist/src
	cp -r view/ tests/ modules/ obj/ controller/ Makefile *.h *.html dist/src/
	cd ./dist && tar cvzf $(TARGET)_v2.0.tgz *
	rm -rf dist/src/

dvi:
	$(OPEN_CMD) $(TARGET).html

test: 
	$(CC) $(FILETEST).cpp $(MODULES).cpp $(CONTROLLER).cpp -o $(FILETESTO) $(TESTFLAGS)
	./$(FILETESTO)

asan: 
	$(CC) $(ASAN) $(FILETEST).cpp $(MODULES).cpp $(CONTROLLER).cpp -o $(FILETESTO) $(TESTFLAGS)
	./$(FILETESTO)

test_val: 
	$(CC) $(FILETEST).cpp $(MODULES).cpp $(CONTROLLER).cpp -o $(FILETESTO) $(TESTFLAGS)
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes -s ./$(FILETESTO)

gcov_report:
	$(CC) $(COVFLAG) $(FILETEST).cpp $(MODULES).cpp $(CONTROLLER).cpp -o $(FILEREPORT) $(TESTFLAGS)
	./$(FILEREPORT)
	lcov -t "$(FILE)" -o test.info -c -d .  
	genhtml -o $(DIRREPORT) test.info
	rm -rf *.o *.a *.gcno *.gcda *.info *.log $(FILETESTO) $(FILEREPORT)
	$(OPEN_CMD) $(DIRREPORT)/index.html

rep: gcov_report

clean:
	rm -rf *.o *.a *.gcno *.gcda *.info *.log $(DIRREPORT) $(FILETESTO) \
	$(FILEREPORT) ./build* ./dist ./app

#style
cpp:
	cppcheck --enable=all --suppress=missingIncludeSystem \
	*.cpp *.hpp $(MODULES).cpp $(FILETEST).cpp $(CONTROLLER).cpp

clang:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n *.hpp $(MODULES).cpp 
	rm .clang-format 

check: cpp clang