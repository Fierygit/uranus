
cd ./Lab3

echo "this is travis"
echo "this is file to build makefile for the test"
echo $(pwd)

# make 
mkdir build || exit
cd build 
cmake ..
make

echo $?
echo "success!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"
# 执行lab3 目录下的test_lab3.sh 脚本


