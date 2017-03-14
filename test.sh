#/usr/bin/bash
#################################
#IPK project 1 tester
#author: Jan Kubalek
#usage: ./test.sh [port] [user]
#
#instruction:
# -1 - have a tesfile "./test.txt" ready to test "get"
#	0 - have your project client program runnable under command "./ftrest"
#	1 - run your project server program "ftrestd" on [port]
#  2 - have a [user] directory in server's root directory
#  3 - run test and go through the output
#  4 - every test has description and expected output
#	5 - tests leave behind all output files; use "rm ttt*" to remove them

if [[ $# -ne 2 ]]
then
	echo "correct call: $0 [port] [user]"
	exit -1
fi

PORT=$1
USER=$2

echo "port: $PORT"
echo "user: $USER"
echo "tests: $TESTS"

CLIENT_P="./ftrest"
PREFIX="http://localhost:"
TEST_F="test.txt"

echo "client call: $CLIENT_P [COMMAND] $PREFIX$PORT/$USER/[REMOTE_PATH] [LOCAL_PATH]"

TN=0

#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: mkd: ok: "

echo "-----------------"
echo "$CLIENT_P mkd $PREFIX$PORT/$USER/D1"
$CLIENT_P mkd $PREFIX$PORT/$USER/D1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: mkd: existing: already exists"

echo "-----------------"
echo "$CLIENT_P mkd $PREFIX$PORT/$USER/D1"
$CLIENT_P mkd $PREFIX$PORT/$USER/D1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: mkd: false path: dir not found"

echo "-----------------"
echo "$CLIENT_P mkd $PREFIX$PORT/$USER/D2/D2"
$CLIENT_P mkd $PREFIX$PORT/$USER/D2/D2 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: put: ok: "

echo "-----------------"
echo "$CLIENT_P put $PREFIX$PORT/$USER/D1/F1 $TEST_F"
$CLIENT_P put $PREFIX$PORT/$USER/D1/F1 $TEST_F 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: put: existing: already exists"

echo "-----------------"
echo "$CLIENT_P put $PREFIX$PORT/$USER/D1/F1 $TEST_F"
$CLIENT_P put $PREFIX$PORT/$USER/D1/F1 $TEST_F 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: put: non-existent: unknown error"

echo "-----------------"
echo "$CLIENT_P put $PREFIX$PORT/$USER/D1/F2 fimfarum"
$CLIENT_P put $PREFIX$PORT/$USER/D1/F2 fimfarum 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: lst: ok: (F1)"

echo "-----------------"
echo "$CLIENT_P lst $PREFIX$PORT/$USER/D1"
$CLIENT_P lst $PREFIX$PORT/$USER/D1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: lst: file: not a dir"

echo "-----------------"
echo "$CLIENT_P lst $PREFIX$PORT/$USER/D1/F1"
$CLIENT_P lst $PREFIX$PORT/$USER/D1/F1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: lst: non-existent: dir not found"

echo "-----------------"
echo "$CLIENT_P lst $PREFIX$PORT/$USER/D2"
$CLIENT_P lst $PREFIX$PORT/$USER/D2 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: get: ok: "

echo "-----------------"
echo "$CLIENT_P get $PREFIX$PORT/$USER/D1/F1 test_d.txt"
$CLIENT_P get $PREFIX$PORT/$USER/D1/F1 test_d.txt 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: get: non-existent: file not found"

echo "-----------------"
echo "$CLIENT_P get $PREFIX$PORT/$USER/F2 hope"
$CLIENT_P get $PREFIX$PORT/$USER/F2 hope 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: get: folder: not a file"

echo "-----------------"
echo "$CLIENT_P get $PREFIX$PORT/$USER/D1 cake"
$CLIENT_P get $PREFIX$PORT/$USER/D1 cake 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: rmd: file: not a dir"

echo "-----------------"
echo "$CLIENT_P rmd $PREFIX$PORT/$USER/D1/F1"
$CLIENT_P rmd $PREFIX$PORT/$USER/D1/F1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: rmd: non-empty: dir not empty"

echo "-----------------"
echo "$CLIENT_P rmd $PREFIX$PORT/$USER/D1"
$CLIENT_P rmd $PREFIX$PORT/$USER/D1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: del: ok:"

echo "-----------------"
echo "$CLIENT_P del $PREFIX$PORT/$USER/D1/F1"
$CLIENT_P del $PREFIX$PORT/$USER/D1/F1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: del: folder: not a file"

echo "-----------------"
echo "$CLIENT_P del $PREFIX$PORT/$USER/D1"
$CLIENT_P del $PREFIX$PORT/$USER/D1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: del: non-existent: file not found"

echo "-----------------"
echo "$CLIENT_P del $PREFIX$PORT/$USER/F2"
$CLIENT_P del $PREFIX$PORT/$USER/F2 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: rmd: ok: "

echo "-----------------"
echo "$CLIENT_P rmd $PREFIX$PORT/$USER/D1"
$CLIENT_P rmd $PREFIX$PORT/$USER/D1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: rmd: non-existent: dir not found"

echo "-----------------"
echo "$CLIENT_P rmd $PREFIX$PORT/$USER/D1"
$CLIENT_P rmd $PREFIX$PORT/$USER/D1 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
exit 0
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: "

echo "-----------------"
echo "$CLIENT_P  $PREFIX$PORT/$USER/"
$CLIENT_P  $PREFIX$PORT/$USER/ 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: "

echo "-----------------"
echo "$CLIENT_P  $PREFIX$PORT/$USER/"
$CLIENT_P  $PREFIX$PORT/$USER/ 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: "

echo "-----------------"
echo "$CLIENT_P  $PREFIX$PORT/$USER/"
$CLIENT_P  $PREFIX$PORT/$USER/ 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
#################################################################
TN=$(($TN+1))
echo "**********************"
echo "TEST $TN: "

echo "-----------------"
echo "$CLIENT_P  $PREFIX$PORT/$USER/"
$CLIENT_P  $PREFIX$PORT/$USER/ 2>ttt$TN.err 1>ttt$TN.out; echo $? 1>ttt$TN.exit
echo "-stdout-"
cat ttt$TN.out
echo "-stderr-"
cat ttt$TN.err
echo "-exit-code-"
cat ttt$TN.exit
