compiler = cl
exec = ./build/gigly.exe
sources = $(wildcard src/**/*.cpp src/*.cpp src/**/**/*.cpp)
include = "src/**/"
flags = /EHsc
flags_gcc = -Wall -g

clean:
	del *.obj

$(exec): $(sources)
	@if not exist build mkdir build
	$(compiler) -I$(include) $(sources) /Fe$(exec) $(flags)
	del *.obj

cppcheck:
	cppcheck --enable=all --force --check-level=exhaustive --error-exitcode=1 src/ -i src/include/json.hpp -I src/ --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=unmatchedSuppression

gcc:
	@if not exist build mkdir build
	g++ -I$(include) $(sources) -o $(exec) $(flags_gcc)
	del *.obj

gcc_run: gcc
	@echo ---------------------------------------------------------
	./$(exec) ./test/testscipt.gc

run: $(exec)
	@echo ---------------------------------------------------------
	$(exec) ./test/testscipt.gc
