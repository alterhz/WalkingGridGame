#!/bin/bash

success()
{
    echo "       .-''''''-.      "
    echo "      .'          '.   "
    echo "     /   O      O   \  "
    echo "    :                : "
    echo "    :                : "  $1
    echo "    : ',          ,' : "
    echo "     \  '-......-'  /  "
    echo "      '.          .'   "
    echo "        '-......-'     "
    exit 0
}

fail()
{
    echo "       .-''''''-.     "
    echo "     .'          '.   "
    echo "    /   O      O   \  "
    echo "   :           \`    : "
    echo "   :                : "  fail in $1
    echo "   :    .------.    : "
    echo "    \  '        '  /  "
    echo "     '.          .'   "
    echo "       '-......-'     "
    exit 1
}

print_stderr_message()
{
    if [ -s stderr_message ]; then 
        echo '------------ message from stderr ------------------';
	cat stderr_message; rm stderr_message
        echo '------------ message from stderr ------------------';
    fi
}

# update src code
if ! svn update ..; then
	fail 'update src code'
fi

# produce build system files
if [ ! -e linux_build ]; then
	mkdir linux_build
fi
cd linux_build
if ! cmake ..; then
	fail 'produce build system files'
fi

# clean
if [ $# == 0 ]; then
	make clean 
fi

# build targets
if ! make -j4 2> >(tee stderr_message); then
	print_stderr_message
	fail 'build targets'
fi 
print_stderr_message

# test
if ! make test; then
	fail 'test'
fi

success 'You are right!'
