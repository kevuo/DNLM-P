#!/bin/bash
input_file="$1"
num_threads="$2"
output_file="$3"
iterations="$4"

for (( i = 0; i < $iterations; i++ )); do
	./mycc $input_file $num_threads $output_file
done

time_array=()
counter=0
while IFS='' read -r line || [[ -n "$line" ]]; do
    #totalSum=`echo $totalSum + $line | bc`
    #totalSum=$($totalSum="+";bc <<< "$line")
    #bc <<< "$num1 + $num2"
	#totalSum=echo "$totalSum + $line" | bc
	time_array[$counter]=$line
	counter=$((counter+1))
    #totalSum=$(IFS="+";bc <<< "$line")
done < "$output_file"
totalSum=$( IFS="+"; bc <<< "${time_array[*]}" )
echo "Sum is: $totalSum"
averageTime=$(echo "scale=4;$totalSum/$iterations"|bc)
echo "Average processing time is: $averageTime"
