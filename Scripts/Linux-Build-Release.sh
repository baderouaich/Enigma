echo "Cleaning up for a fresh build..."
sudo bash ./Clean.sh


echo "Adding ppa:ubuntu-toolchain-r/ppa repository..."
sudo add-apt-repository ppa:ubuntu-toolchain-r/ppa
sudo apt update


echo "Installing latest C/C++ Compiler..."
sudo apt install gcc-10 g++-10


echo "Installing required libraries..."
sudo apt install libgl1-mesa-dev libxi-dev libx11-dev libxcursor-dev libxrandr-dev libxcomposite-dev libxinerama-dev libtbb-dev


echo "Extracting premake5 binary..."
tar xvzf ../Premake/Linux/premake-5.0.0-alpha16-linux.tar.gz -C ../Premake/Linux/


echo "Generating project Makefiles..."
sudo bash ./Gen-Linux-Makefile.sh

cd ..
echo "Building project..."
make --file=./Makefile config=release CC=gcc-10 CPP=g++-10 --jobs=$(nproc)
cd Scripts

echo "Displaying Enigma version..."
../Bin/Release-linux-x86_64/Enigma/Enigma --version


echo "DONE!"