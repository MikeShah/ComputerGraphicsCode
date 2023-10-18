# Run with: python3 build.py
import os
import platform

# (1)==================== COMMON CONFIGURATION OPTIONS ======================= #
COMPILER="g++ -g -std=c++17"   # The compiler we want to use 
                                #(You may try g++ if you have trouble)
SOURCE="./src/*.cpp"    # Where the source code lives
EXECUTABLE="prog"        # Name of the final executable
# ======================= COMMON CONFIGURATION OPTIONS ======================= #

# (2)=================== Platform specific configuration ===================== #
# For each platform we need to set the following items
ARGUMENTS=""            # Arguments needed for our program (Add others as you see fit)
INCLUDE_DIR=""          # Which directories do we want to include.
LIBRARIES=""            # What libraries do we want to include

if platform.system()=="Linux":
    ARGUMENTS="-D LINUX" # -D is a #define sent to preprocessor
    INCLUDE_DIR="-I ./include/ -I ./../../common/thirdparty/glm/"
    LIBRARIES="-lSDL2 -ldl"
elif platform.system()=="Darwin":
    ARGUMENTS="-D MAC" # -D is a #define sent to the preprocessor.
    INCLUDE_DIR="-I ./include/ -I/Library/Frameworks/SDL2.framework/Headers -I./../../common/thirdparty/old/glm"
    LIBRARIES="-F/Library/Frameworks -framework SDL2"
elif platform.system()=="Windows":
    ARGUMENTS="-D MINGW -static-libgcc -static-libstdc++" 
    INCLUDE_DIR="-I./include/ -I./../../common/thirdparty/old/glm/"
    EXECUTABLE="prog.exe"
    LIBRARIES="-lmingw32 -lSDL2main -lSDL2 -mwindows"
# (2)=================== Platform specific configuration ===================== #

# (3)====================== Building the Executable ========================== #
# Build a string of our compile commands that we run in the terminal
compileString=COMPILER+" "+ARGUMENTS+" "+SOURCE+" -o "+EXECUTABLE+" "+" "+INCLUDE_DIR+" "+LIBRARIES
# Print out the compile string
# This is the command you can type
print("===============================================================================")
print("====================== Compiling on: "+platform.system()+" =============================")
print("===============================================================================")

print("Below is the command this script is running to compile your source code arguments.")
print("\tNote: You could type this out, or otherwise just run this script\n")
print(compileString)
print("\n")
print("-I is the path to header files, or the directories at which .h and .hpp files should be searched to be found.")
print("\t for example: "+INCLUDE_DIR+"\n")
print("-l (lowercase L) are the libraries being linked. These are libraries that have been compiled already, and we're 'gluing' into our program")
print("Note: Mac users instead may also package thing as a 'framework' and use -F to link.")
print("\t for example: "+LIBRARIES+"\n")
print("At some point, if you become frustrated with compilation, it is worth watching the following to understand what is going on")
print("Watch the full video, and try some compiling some examples from scratch to learn and understand compilation")
print("\thttps://www.youtube.com/watch?v=ksJ9bdSX5Yo&list=PLvv0ScY6vfd8YRjgGvXKJRAMZQAxNypcH&index=3")
print("===============================================================================")
print("===============================================================================")
# Run our command 
# Here I am using an exit_code so you can
# also compile & run in one step as
# python3 build.py && ./prog
# If compilation fails, ./prog will not run.
exit_code = os.system(compileString)
exit(0 if exit_code==0 else 1)
# ========================= Building the Executable ========================== #


# Why am I not using Make?
# 1.)   I want total control over the system. 
#       Occasionally I want to have some logic
#       in my compilation process (like searching for missing files).
# 2.)   Realistically our projects are 'small' enough 
#       this will not matter.
# 3.)   Feel free to implement your own make files or autogenerate it from this
#       script
# 4.)   It is handy to know Python


