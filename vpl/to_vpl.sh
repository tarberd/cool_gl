cp ../cool/*.{glade,cpp,h} .
cp ../libcool_gl/src/*.cpp .
cp ../libcool_gl/include/cool_gl/*.h .

sed -i 's/#include <cool_gl\/\(\w*\).h>/#include "\1.h"/g' *
