compiler = cl
exec = ./build/gigly.exe
sources = $(wildcard src/**/*.cpp src/*.cpp src/**/**/*.cpp)
include = "src/**/"
flags = /EHsc
flags_gcc = -Wall -g

clean:
	del *.obj
	del *.plist

$(exec): $(sources)
	@if not exist build mkdir build
	$(compiler) -I$(include) $(sources) /Fe$(exec) $(flags)

gcc:
	@if not exist build mkdir build
	g++ -I$(include) $(sources) -o $(exec) $(flags_gcc)

gcc_run: gcc
	@echo ---------------------------------------------------------
	./$(exec) ./test/testscipt.gc

clang:
	@if not exist build mkdir build
	clang++ -I$(include) $(sources) -o $(exec) $(flags_gcc)

clang_a:
	@if not exist build mkdir build
	clang++ -I$(include) $(sources) --analyze

clang_run: clang
	@echo ---------------------------------------------------------
	./$(exec) ./test/testscipt.gc

run: $(exec)
	@echo ---------------------------------------------------------
	$(exec) ./test/testscipt.gc

cppcheck:
	cppcheck --enable=all --check-level=exhaustive --error-exitcode=1 src/ -i src/include/json.hpp -I src/ --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=unmatchedSuppression
