
run:
	@echo ---------------------------------------------------------
	./build/gigly.exe -f ./test/testscipt.gc -o ./dump/testscipt.o 

rund:
	@echo ---------------------------------------------------------
	./build/Debug/gigly.exe -f ./test/testscipt.gc -o ./dump/testscipt.o

# compiler = cl
# exec = ./build/gigly.exe
# sources = $(wildcard src/**/*.cpp src/*.cpp src/**/**/*.cpp)
# include = -I"src/**/" -I"C:\Program Files (x86)\LLVM\include" -I"C:\Program Files (x86)\LLVM\lib"
# flags = /EHsc
# flags_gcc = -Wall -g

# $(exec): $(sources)
# 	@if not exist build mkdir build
# 	$(compiler) $(include) $(sources) /Fe$(exec) $(flags)

# clean:
# 	del *.obj
# 	del *.plist

# gcc:
# 	@if not exist build mkdir build
# 	g++ $(include) $(sources) -o $(exec) $(flags_gcc)

# gcc_run: gcc
# 	@echo ---------------------------------------------------------
# 	./$(exec) ./test/testscipt.gc

# clang:
# 	@if not exist build mkdir build
# 	clang++ $(include) $(sources) -o $(exec) $(flags_gcc)

# clang_a:
# 	@if not exist build mkdir build
# 	clang++ $(include) $(sources) --analyze

# clang_run: clang
# 	@echo ---------------------------------------------------------
# 	./$(exec) ./test/testscipt.gc

# run: $(exec)
# 	@echo ---------------------------------------------------------
# 	$(exec) ./test/testscipt.gc

cppcheck:
	cppcheck --enable=all --check-level=exhaustive --error-exitcode=1 src/ -i src/include/json.hpp -I src/ --suppress=missingIncludeSystem --suppress=unusedFunction --suppress=unmatchedSuppression
