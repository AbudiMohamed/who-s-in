from   fastapi import HTTPException
from database import connect
from dotenv import load_dotenv
from datetime import datetime
import json
import psycopg2
import os

load_dotenv()

def checkin_time(id:int,date:str,entry:str):
    connection = connect()
    cursor = connection.cursor()
    try:
        user = cursor.execute('INSERT INTO timestamps (id, date,entry) VALUES (%s, %s, %s)',
                            (id, date, entry))
        connection.commit()
        return {'msg' : f'{id} checkedin in at {entry} on {date}'}
    except (Exception, psycopg2.Error) as error :
        msg = f'Error while fetching user data: {error}'
        return HTTPException(status_code=500, detail=msg)

def get_ondate(date:str):
    connection = connect()
    cursor = connection.cursor()
    l = []
    try:
        cursor.execute(f"SELECT * FROM timestamps WHERE DATE = '{date}';")
        all = cursor.fetchall()
        
        for user in all:
            id = user[0]
            date = user[1]
            entry = user[2]
            cursor.execute(f'SELECT username FROM users WHERE id={id}')
            username = cursor.fetchone()
            cursor.execute(f'SELECT name FROM users WHERE id={id}')
            name = cursor.fetchone()
            cursor.execute(f'SELECT email FROM users WHERE id={id}')
            email = cursor.fetchone()
            text = {"id": id,
                    "name": name[0],
                    "username": username[0],
                    "checking-time": entry,
                    "email": email[0]}
            l.append(text)
        
        seen = set()
        new_l = []
        for d in l:
            t = d["id"]
            if t not in seen:
                seen.add(t)
                new_l.append(d)
        return new_l
    except (Exception, psycopg2.Error) as error :
        msg = f'Error while fetching user data: {error}'
        return HTTPException(status_code=500, detail=msg)

def whoisin_add(id:int, date:str, entry:str):
    connection = connect()
    cursor = connection.cursor()
    try:
        cursor.execute(f'INSERT INTO whoisin (id, date, entry) VALUES (%s, %s, %s)', (id, date, entry))
        connection.commit()
    except (Exception, psycopg2.Error) as error :
        msg = f'Error while fetching user data: {error}'
        return msg

def whoisin_out(id:int):
    connection = connect()
    cursor = connection.cursor()
    try:
        cursor.execute(f'DELETE FROM whoisin WHERE id = {id};')
        connection.commit()
    except (Exception, psycopg2.Error) as error :
        msg = f'Error while fetching user data: {error}'
        return msg

def who_is_in():
    connection = connect()
    cursor = connection.cursor()
    all_in = []
    try:
        get = cursor.execute(f'SELECT * FROM whoisin')
        all = cursor.fetchall()
        for user in all:
            id    = user[0]
            date  = user[1]
            entry = user[2]
            cursor.execute(f'SELECT name FROM users WHERE id={id}')
            name = cursor.fetchone()
            text = {"id": id,
                    "name": name[0],
                    "checkin time": entry,
                    "date": date}
            all_in.append(text)
        return all_in
    except (Exception, psycopg2.Error) as error :
        msg = f'Error while fetching user data: {error}'
        return HTTPException(status_code=500, detail=msg)


print(get_ondate('12-01-2021'))