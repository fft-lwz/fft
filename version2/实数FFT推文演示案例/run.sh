time=$(date "+%Y.%m.%d %H:%M:%S")
echo "${time}" >> out.log
gcc main.c -o main -lm
echo "Finish compilation" >> out.log
start=$(date +%s)
./main >> out.log
end=$(date +%s)
take=$(( end - start ))
echo "Time consumed: ${take} seconds" >> out.log
echo -e "Finish run.sh\n\n" >> out.log