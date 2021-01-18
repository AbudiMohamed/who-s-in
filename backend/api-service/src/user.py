from   fastapi import HTTPException
from database import connect
from dotenv import load_dotenv
import psycopg2
import json
import os

load_dotenv()

def add_user(id:str, username:str, email:str, name:str):
    connection = connect()
    cursor = connection.cursor()
    try:
        user = cursor.execute('INSERT INTO users (id, username, name,email) VALUES (%s, %s, %s, %s)',
                            (int(id), username, name, email))
        connection.commit()
        return {'msg' : 'recored created'}
    except (Exception, psycopg2.Error) as error :
        msg = f'Error_msg: {error}'
        return HTTPException(status_code=500, detail=msg)

def get_all_users():
    connection = connect()
    cursor = connection.cursor()
    users = [] 
    try:
        get = cursor.execute(f'SELECT * FROM users')
        user_data = cursor.fetchall()
        for user in user_data:
            text = {
                "id" : user[0],
                "username" : user[1],
                "name": user[2],
                "email": user[3]
            }
            users.append(text)
        return users
    except (Exception, psycopg2.Error) as error :
        print ("Error_msg: ", error)


def get_user(id:int):
    connection = connect()
    cursor = connection.cursor()
    try:
        get = cursor.execute(f'SELECT * FROM users WHERE id={id}')
        user_data = cursor.fetchone()
        return {
            "id": user_data[0],
            "username": user_data[1],
            "name": user_data[2],
            "email": user_data[3]
    }
    except (Exception, psycopg2.Error) as error :
        print ("Error_msg: ", error)

def check_exists(id:int):
    connection = connect()
    cursor = connection.cursor()
    try:
        cursor.execute(f'SELECT id FROM users WHERE id={id}')
        check = cursor.fetchone() 
        if check == None:
            return False
        else:
            return True
    except (Exception, psycopg2.Error) as error :
        print ("Error_msg: ", error)

def delete_user(id:int):
    connection = connect()
    cursor = connection.cursor()
    if check_exists(id):
        try:
            cursor.execute(f'DELETE FROM users WHERE id={id} RETURNING *')
            connection.commit()
            return {"msg":"recored deleted"}
            
        except (Exception, psycopg2.Error) as error :
            print ("Error_msg: ", error)
    else:
        return {"msg":"Can't find the ID"}
