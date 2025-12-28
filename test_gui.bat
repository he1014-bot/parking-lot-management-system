@echo off
echo 10 > input.txt
echo 3 >> input.txt
echo 7 >> input.txt
echo 0 >> input.txt
echo 0 >> input.txt
.\ParkingSystem.exe < input.txt
del input.txt
