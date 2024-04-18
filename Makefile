compiler = cl
exec = gigly.exe
sources = $(wildcard src/*.cpp)
include = "src/include"
flags = /EHsc

$(exec): $(sources)
	$(compiler) -I$(include) $(sources) /Fe$(exec) $(flags)

run: $(exec)
	$(exec) ./dump/hello.gc

clean:
	del main.obj
