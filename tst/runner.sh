#!/bin/sh

echo "==================================================="
while read file
do           
  if ! ../tst/compare.sh "$file" $1
  then
    echo "---------------------------------------------------"
    echo -n "$file "
    printf "%$((42-${#file}))s " | tr ' ' -
    echo " FAILED"
  else
    echo -n "$file "
    printf "%$((38-${#file}))s " | tr ' ' -
    echo " SUCCESSFUL"
  fi 
  echo "==================================================="
done < ../tst/testcases   
