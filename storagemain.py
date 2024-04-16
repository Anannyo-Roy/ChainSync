import mysql.connector
import sys

mydb = mysql.connector.connect(host='localhost',password='1234',user='root',database ='cs1')
if(mydb.is_connected):
    print("Connected")

mycursor = mydb.cursor()

class Storage_Management:
    def __init__(self):
        pass
    
    def add_storage(self, name, closest_to, dist, capacity):
        # Insert new storage information into the database
        sql = "INSERT INTO storage (name, closest_to, dist, capacity) VALUES (%s, %s, %s, %s)"
        val = (name, closest_to, dist, capacity)
        mycursor.execute(sql, val)
        mydb.commit()
        print("Storage added successfully.")

    def delete_storage(self, name):
        # Delete storage information from the database
        sql = "DELETE FROM storage WHERE name = %s"
        val = (name,)
        mycursor.execute(sql, val)
        mydb.commit()
        print("Storage deleted successfully.")

    def edit_capacity(self, name, new_capacity):
        # Update the capacity of the specified storage
        sql = "UPDATE storage SET capacity = %s WHERE name = %s"
        val = (new_capacity, name)
        mycursor.execute(sql, val)
        mydb.commit()
        print("Capacity updated successfully.")
