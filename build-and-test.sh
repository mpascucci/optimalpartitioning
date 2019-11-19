if test -d build; then
    cd build
else
    mkdir build
fi

cmake ..; make;
test/example

cd ..