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

cd ../
./install.sh 
cd $test_dir

gcc -Werror -Wall $1 -L /usr/local/lib -lnormalc

if [[ $2 == "--debug" ]]; then
	valgrind -s --leak-check=full --track-origins=yes ./a.out
else 
	./a.out
fi

rm ./a.out
 
