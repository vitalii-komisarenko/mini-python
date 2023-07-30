./test

PROG_DIR='test_python_progs'

function test_python_prog() {
    file="$PROG_DIR/$1.py"
    cmp --silent <(../mini-python $file) <(python3 $file) && return
    echo $file did not run properly
    echo mini_python output:
    ../mini-python $file
    echo python output:
    python3 $file
}

test_python_prog hello
