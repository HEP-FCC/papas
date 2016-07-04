echo "\Clang-Format all files in current dir \n"
find . -name "*.cpp"  -print0 | xargs -0 clang-format -i
find . -name "*.h"  -print0 | xargs -0 clang-format -i
