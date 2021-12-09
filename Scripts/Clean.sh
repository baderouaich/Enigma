cd ..

find . -type d -name "Bin" -exec rm -r {} +;
find . -type d -name "Bin-Intermediate" -exec rm -r {} +;
find . -name "Makefile" -delete;
find . -name "*.make" -delete;

cd Scripts