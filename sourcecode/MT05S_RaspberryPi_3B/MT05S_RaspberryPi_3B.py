#!/usr/bin/python3
import os,time

ROMCODE='28-060504030201'
device_file ='/sys/bus/w1/devices/'+ROMCODE+'/w1_slave'
print("//----------------------------------------------------------------");
print("// Start Testing MT05S");
print("// Rom Code= " + ROMCODE);
print("// Change ROM CODE FOR YOUR SENSOR !!!!!!");
print("//----------------------------------------------------------------");

def read_raw():
    f = open(device_file,'r')
    lines = f.readlines()
    f.close()
    return lines

def read_mt05s():
    lines = read_raw()
    while lines[0].strip()[-3:] != 'YES':
        print('Incorrect Data')
        time.sleep(0.2)
        lines = read_raw()
    byteList = lines[1].rsplit((' '))
    if len(byteList) >9 :
        soilTempHi8=int(byteList[0], 16)
        soilTempLo8=int(byteList[1], 16)
        soilTemp = soilTempHi8*256+soilTempLo8

        soilMoistureHi8=int(byteList[2], 16)
        soilMoistureLo8=int(byteList[3], 16)
        soilMoisture = soilMoistureHi8*256+soilMoistureLo8

        soilECHi8=int(byteList[4], 16)
        soilECLo8=int(byteList[5], 16)
        soilEC = soilECHi8*256+soilECLo8
    return [ soilTemp, soilMoisture, soilEC  ]

while True:
    values = read_mt05s()
    print('Soil Temp(C)= %.2f  Moisture(%%)= %.2f  EC(ms/cm)= %.3f' %(values[0]/100.0,  values[1]/100.0,  values[2]/1000.0))
    time.sleep(1)