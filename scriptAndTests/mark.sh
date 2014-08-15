#!/bin/bash
for i in submissions/*;
do {
cd work
#export mark=0
cp -f "../$i" ece454a4.zip
touch test_file1
echo hahahahaha > test_file1

arr=$(echo $i | tr "/." " ")

echo $arr
#read -p "Press [Enter] key to start backup..."
cp ../unpack.sh .
cp ../t1_client.c .
cp ../t2_client.c .
cp ../t3_client.c .

./unpack.sh > "../feedback/$arr" 2>&1 

srvout=$(cat ./mark)
read mark <<< "$srvout"

touch results
echo $i $mark
echo $i $mark >> ../results
more test_file1
rm -rf t1_client t2_client t3_client test_file1 test_file2 test_file3
rm -rf *
cd ..
#read -p "Press [Enter] key to start backup..."
};
done 

