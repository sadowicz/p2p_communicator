#!/bin/bash

rm -rf .ci/test-results && mkdir .ci/test-results
for file in $(find . -type f -path "*build*" -name test_detail.xml)
do
  echo -n "Processing $file file... "
  unique_name=$(echo ${file:2} | sed 's/\//_/g')
  mv $file .ci/test-results/$unique_name
  echo "[OK]"
done
