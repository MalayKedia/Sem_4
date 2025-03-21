correct=0
total=0

echo "TESTSET: Running Testcases with reader preference"
g++ test-reader-pref.cpp rwlock-reader-pref.cpp -o rwlock-reader-pref -lpthread

echo "CASE1: Reader Preference with 5 reader and 1 writer"
total=$((total+1))
out=`./rwlock-reader-pref 5 1`
echo $out
if [ "$out" = "PASSED" ]; then
    correct=$((correct+1))
fi

echo "CASE2: Reader Preference with 5 reader and 3 writer"
total=$((total+1))
out=`./rwlock-reader-pref 5 3`
echo $out
if [ "$out" = "PASSED" ]; then
    correct=$((correct+1))
fi

echo "CASE3: Reader Preference with 5 reader and 5 writer"
total=$((total+1))
out=`./rwlock-reader-pref 5 5`
echo $out
if [ "$out" = "PASSED" ]; then
    correct=$((correct+1))
fi

echo "TESTSET: Running Testcases with writer preference"
g++ test-writer-pref.cpp rwlock-writer-pref.cpp -o rwlock-writer-pref -lpthread

echo "CASE1: Writer Preference with 5 reader and 1 writer"
total=$((total+1))
out=`./rwlock-writer-pref 5 1`
echo $out
if [ "$out" = "PASSED" ]; then
    correct=$((correct+1))
fi


echo "CASE2: Writer Preference with 5 reader and 3 writer"
total=$((total+1))
out=`./rwlock-writer-pref 5 3`
echo $out
if [ "$out" = "PASSED" ]; then
    correct=$((correct+1))
fi

echo "CASE3: Writer Preference with 5 reader and 5 writer"
total=$((total+1))
out=`./rwlock-writer-pref 5 5`
echo $out
if [ "$out" = "PASSED" ]; then
    correct=$((correct+1))
fi

echo "Test Cases Passed: $correct"
echo "Test Cases Total: $total"

