import MySQLdb
import RPi.GPIO as GPIO
import re
import sys
import os
import serial

#DB connection variables
host = 'localhost'
user = 'root'
password = 'root'
db = 'assignment2'
dbConn = MySQLdb.connect(host, user, password, db) or die("Could Not Connect To Db")

#Check that all our tables exist, and set them up if they dont
with dbConn:
    cursor = dbConn.cursor()
    cursor.execute("CREATE TABLE IF NOT EXISTS temperatureDB(id int(7) NOT NULL AUTO_INCREMENT, Temp varchar(7) NOT NULL, primary key (id));")
    cursor.execute("CREATE TABLE IF NOT EXISTS TempLimit(temp_id int(5) NOT NULL AUTO_INCREMENT, TempLimit int(2), primary key (temp_id));")
    cursor.execute("CREATE TABLE IF NOT EXISTS FanStatus(id int(1) NOT NULL, FanOnOrOff varchar(3), primary key (id));")

    cursor.execute("CREATE TABLE IF NOT EXISTS players(player_id int(4), current_level int(1), high_score int(1), constraint player_pk primary key (player_id));")

# Serial commmunication
ser = serial.Serial('/dev/ttyACM0', 9600)
ser.flushInput()
ser.flushOutput()

temperature = ""
playerId = ""
level = ""

message = ser.readline()
message = re.sub("[^0-9.]", "", message)
sys.stdout.write("Input received: " + message + "\n")
msglength = len([c for c in message if c.isdigit()]);
# sys.stdout.write("actual length: " + str(len(message)) + "\n")

tempSize = int(message[0:1])
temperature = message[1:tempSize + 1]

# size of stirng = fist character of message + the length of the temp val
expectedSize = tempSize +1; 

if (message[(tempSize + 1): (tempSize + 2)]) == "1":
	playerId = message[tempSize + 2: tempSize + 6]
	level = message[tempSize + 6: tempSize + 7]
	expectedSize += 5;
	
expectedSize += 1

# sys.stdout.write("Temp: " + str(temperature) + "\n")
# sys.stdout.write("Player ID: " + str(playerId) + "\n")
# sys.stdout.write("Level " + str(level)+ "\n")

sys.stdout.write("Expected Input size: " +str(expectedSize) + "\n")
sys.stdout.write("Temperature: " + temperature + "\n")



if (len(message) == expectedSize):
    cursor.execute("INSERT INTO temperatureDB (Temp) VALUES ('%s')" % (temperature))
 #   print ("Temperature Added", temperature)
    dbConn.commit()
    sys.stdout.write("Valid Input Read\n")
else:
    sys.stdout.write("invalid\n")
    cursor.close()