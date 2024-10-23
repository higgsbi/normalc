if [[ $1 == "--clean" ]]; then
    rm -rf build
    rm -rf out
elif [[ $1 == "--local" ]]; then
    rm -rf build
    mkdir build
    cd build
    cmake .. -DLOCAL_BUILD=ON
    make install
    mv install/ ../out
    cd ..
else 
    rm -rf build
    mkdir build
    cd build
    cmake ..
    sudo make install
    cd ..
fi
