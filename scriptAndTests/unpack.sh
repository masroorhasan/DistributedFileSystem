#!/bin/bash
#export 
#export mark=5
touch mark
echo 0 > mark
#export $mark
set -e
rm -rf ece454a4
unzip ece454a4.zip
cp --remove-destination ../ece454_fs.h .
cd ece454a4
rm -rf ./tmp
make clean
make server

#Note: the following line should create libclient-api.a
make client-api.a 

./fsServer /home/w3smith/scriptAndTests/work >> ./tmp &
sleep 1s
#export 
#export mark=5
echo 5 > ../mark
echo "Passed test 1..."
#export $mark

srvout=$(cat ./tmp)
echo $srvout
rm -rf ./tmp
IFS=' ' read ip port <<< "$srvout"
echo "ip=${ip}, port=${port}"
cd ..
rm -rf t1_client t2_client t3_client test_file2 test_file3
gcc -lstdc++ -o t1_client t1_client.c -Iece454a4 -Lece454a4 -lclient-api
./t1_client . $ip $port 2>&1
echo here
more test_file2
echo "Mo"
result=$(more test_file2)
if [ "$result" != "Write stuff..." ]
then echo "Write does not work"; exit 1
fi

echo 55 > ./mark
# more complex test

cd ece454a4
rm -rf ./tmp
./fsServer /home/w3smith/scriptAndTests/work >> ./tmp &
sleep 1s
srvout=$(cat ./tmp)
echo $srvout
rm -rf ./tmp
IFS=' ' read ip2 port2 <<< "$srvout"
echo "ip2=${ip2}, port2=${port2}"
cd ..

gcc -o t2_client t2_client.c -Iece454a4 -Lece454a4 -lclient-api
rm -rf test_file1
touch test_file1
echo hahahahaha > test_file1
./t2_client . $ip $port local $ip2 $port2 1 2>&1
echo hahahahaha > test_file1
./t2_client . $ip $port local $ip2 $port2 2 2>&1
echo hahahahaha > test_file1
./t2_client . $ip $port local $ip2 $port2 3 2>&1

#export 
#export mark=80
echo "Passed test 2..."
echo 80 > mark
#export $mark

gcc -o t3_client t3_client.c -Iece454a4 -Lece454a4 -lclient-api
rm -rf test_file1
rm -rf ./tmp
./t3_client . $ip $port 1 >> ./tmp & 
./t3_client . $ip $port 2 >> ./tmp2 &
echo "Waiting for test 3 to finish"
sleep 60s

more ./tmp
more ./tmp2

#result=$(cat ./tmp)
if grep -q "Something is wrong... Investigate" ./tmp
then echo "take a closer look" > mark; #export $mark
fi
if grep -q "Give full mark" ./tmp
then echo 100 > mark; echo "Passed test 3..."; #export $mark
fi
rm -rf t1_client t2_client t3_client __MACOSX
pkill Server
