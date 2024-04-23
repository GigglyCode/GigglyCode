compiler = cl
exec = gigly.exe
sources = $(wildcard src/*.cpp)
include = "src/include"
flags = /EHsc
flags_gcc = -Wall -g

$(exec): $(sources)
	$(compiler) -I$(include) $(sources) /Fe$(exec) $(flags)

gcc:
	g++ -I$(include) $(sources) -o $(exec) $(flags_gcc)

gcc_run: gcc
	@echo ---------------------------------------------------------
	./$(exec) ./dump/hello.gc

run: $(exec)
	@echo ---------------------------------------------------------
	$(exec) ./dump/hello.gc

clean:
	del *.obj
	del *.exe
