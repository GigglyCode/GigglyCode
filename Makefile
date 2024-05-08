compiler = cl
exec = ./build/gigly.exe
sources = $(wildcard src/**/*.cpp src/*.cpp src/**/**/*.cpp)
include = "src/**/"
flags = /EHsc
flags_gcc = -Wall -g

clean:
	del *.obj

$(exec): $(sources)
	$(compiler) -I$(include) $(sources) /Fe$(exec) $(flags)
	del *.obj

gcc:
	g++ -I$(include) $(sources) -o $(exec) $(flags_gcc)
	del *.obj

gcc_run: gcc
	@echo ---------------------------------------------------------
	./$(exec) ./test/testscipt.gc

run: $(exec)
	@echo ---------------------------------------------------------
	$(exec) ./test/testscipt.gc
