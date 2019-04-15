cp ../cool/*.{,cpp,h} .
cp ../cool/data/*.glade .
cp ../cool_gl/src/*.cpp .
cp ../cool_gl/include/cool_gl/*.h .

sed -i 's/#include <cool_gl\/\(\w*\).h>/#include "\1.h"/g' *
