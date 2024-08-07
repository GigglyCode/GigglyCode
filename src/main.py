from lexer import Lexer
import sys

LEXER_DEBUG = True
DEV_MODE = True

def main() -> None:
    # Reading code
    if not DEV_MODE:
        if len(sys.argv) != 5:
            print("Invalid number of arguments. Usage: exe -f <file> -o <output>")
            sys.exit(1)

        input_file = ""
        output_file = ""

        for i in range(1, len(sys.argv), 2):
            arg = sys.argv[i]
            if arg == "-f":
                input_file = sys.argv[i + 1]
            elif arg == "-o":
                output_file = sys.argv[i + 1]
            else:
                print("Invalid argument:", arg)
                sys.exit(1)
    else:
        input_file = r"d:\soham_code\GigglyCode\test\testscipt.gc"
        output_file = r"d:\soham_code\GigglyCode\dump\testscipt.ll"
    if input_file == "" or output_file == "":
        print("Input file or output file not provided.")
        sys.exit(1)

    try:
        with open(input_file, 'r') as file:
            file_content = file.read()
    except IOError:
        print("Unable to open input file:", input_file)
        sys.exit(1)

    # Lexing
    lexer = Lexer(file_content)
    if LEXER_DEBUG:
        print("Tokens:")
        for tok in lexer.tokens:
            print(tok)

if __name__ == "__main__":
    main()