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
dbConn = MySQLdb.connect(host, user, password) or die("Could Not Connect To Db")

#Check that all our tables exist, and set them up if they dont
with dbConn:
    cursor = dbConn.cursor()
    cursor.execute("CREATE DATABASE IF NOT EXISTS assignment2;")
    cursor.execute("USE assignment2;")
    cursor.execute("CREATE TABLE IF NOT EXISTS temperatureDB(id int(7) NOT NULL AUTO_INCREMENT, Temp varchar(7) NOT NULL, primary key (id));")
    cursor.execute("CREATE TABLE IF NOT EXISTS TempLimit(temp_id int(5) NOT NULL AUTO_INCREMENT, TempLimit int(2), primary key (temp_id));")
    cursor.execute("CREATE TABLE IF NOT EXISTS FanStatus(id int(1) NOT NULL, FanOnOrOff varchar(3), primary key (id));")
    cursor.execute("CREATE TABLE IF NOT EXISTS players(player_id char(4) NOT NULL, current_level char(1), constraint player_pk primary key (player_id));")
    cursor.execute("INSERT IGNORE INTO FanStatus (id, FanOnOrOff) VALUES ('1', 'Off');")
# Serial commmunication
ser = serial.Serial('/dev/ttyACM1', 9600)
ser.flushInput()
ser.flushOutput()

temperature = ""
playerId = ""
level = ""
playerActive = ""

message = ser.readline()
message = re.sub("[^0-9.]", "", message)
sys.stdout.write("Input received: " + message + "\n")
msglength = len([c for c in message if c.isdigit()]);
# sys.stdout.write("actual length: " + str(len(message)) + "\n")


if message[0:1] != "":
#{
    tempSize = int(message[0:1])
    temperature = message[1:tempSize + 1]

    # size of stirng = fist character of message + the length of the temp val
    expectedSize = tempSize +1; 
	
    playerActive = message[(tempSize + 1): (tempSize + 2)] 

    if  playerActive == "1":
	playerId = message[tempSize + 2: tempSize + 6]
	level = message[tempSize + 6: tempSize + 7]
	expectedSize += 5;
	
    expectedSize += 1
#}

sys.stdout.write("Temp: " + str(temperature) + "\n")
sys.stdout.write("Player ID: " + str(playerId) + "\n")
sys.stdout.write("Level " + str(level)+ "\n")
 
# sys.stdout.write("Expected Input size: " +str(expectedSize) + "\n")
# sys.stdout.write("Temperature: " + temperature + "\n")

# read in data
if True:#(len(message) == expectedSize):
    cursor.execute("INSERT INTO temperatureDB (Temp) VALUES ('%s')" % (temperature))
    rows = cursor.execute("SELECT * FROM players WHERE player_id LIKE '%s'" % (playerId))  
 
    if rows == 0:
	sys.stdout.write("no rows found")
	cursor.execute("INSERT INTO players (player_id, current_level) VALUES ('%s', '%s');" % (playerId, level))
        dbConn.commit()    
    else:
        cursor.execute("UPDATE players SET current_level = %s WHERE player_Id= %s;" % (level, playerId))
        dbConn.commit()
    print ("Temperature Added", temperature)
    sys.stdout.write("Valid Input Read\n\n")
else:
    sys.stdout.write("invalid\n\n")


cursor.close()