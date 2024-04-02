compiler = cl
exec = gigly.exe
sources = $(wildcard src/*.c)
include = "src/include"

$(exec): $(sources)
	$(compiler) -I$(include) $(sources) /Fe$(exec)

run: $(exec)
	$(exec) ./dump/hello.gc

clean:
	del main.obj
