if [[ $# -eq 0 ]]; then
	echo "No argument supplied"
	exit 1
fi

if [[ ! -f $1 ]]; then
	echo $1" does not exist"
	exit 1
fi

if [[ ! ${1##*.} = c ]]; then
	echo $1" is not a source file"
	exit 1
fi

test_dir=$PWD

if [[ ! -d "../build/" ]]; then
	mkdir ../build
	cd ../build/
	cmake ..
else
	cd ../build/
fi

sudo make install | grep -w "Installing\|.h"

cd ../test/

gcc $1 -L /usr/local/lib -lstandard 

if [[ $2 == "debug" ]]; then
	valgrind -s --leak-check=full --track-origins=yes ./a.out
else 
	./a.out
fi

rm ./a.out
 
