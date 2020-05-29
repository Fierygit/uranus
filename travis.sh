
cd ./Lab3

echo "hello world"
echo $(pwd)

# make 
mkdir build || exit
cd build 
cmake ..
make


