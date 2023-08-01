./test

PROG_DIR='test_python_progs'

function test_python_prog() {
    file=$1
    cmp --silent <(../mini-python $file) <(python3 $file) && return
    echo $file did not run properly
    echo mini_python output:
    ../mini-python $file
    echo python output:
    python3 $file
}

for python_file in $PROG_DIR/*.py; do
    test_python_prog $python_file
done
