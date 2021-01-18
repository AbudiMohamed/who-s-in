import psycopg2
from dotenv import load_dotenv
import os

load_dotenv()
password = os.environ.get('PASSWORD')
user = os.environ.get('USER')
db = os.environ.get('DB_NAME')
host = os.environ.get('HOST')
port = os.environ.get('PORT')

def setup_db() -> psycopg2.connect:
    try:
        connection = psycopg2.connect(user = user,
                                    password = password,
                                    host = host,
                                    port = port,
                                    database = db)
                                    
    except (Exception, psycopg2.Error) as error :
        print ("Error while connecting to PostgreSQL", error)
    
    try:
        cursor = connection.cursor()
        create_user = '''CREATE TABLE IF NOT EXISTS users \
            (ID BIGINT  PRIMARY KEY     NOT NULL, \
            USERNAME           TEXT    NOT NULL UNIQUE, \
            NAME TEXT NOT NULL , \
            EMAIL         TEXT); '''
        
        create_timetable = '''CREATE TABLE IF NOT EXISTS timestamps \
            (ID BIGINT   NOT NULL, \
            DATE  TEXT    NOT NULL, \
            ENTRY TEXT  NULL); '''
        
        create_who_is_in = '''CREATE TABLE IF NOT EXISTS whoisin \
            (ID BIGINT   NOT NULL, \
            DATE  TEXT    NOT NULL, \
            ENTRY TEXT  NULL); '''
        
        cursor.execute(create_user)
        cursor.execute(create_timetable)
        cursor.execute(create_who_is_in)
        connection.commit()
        print("PostgreSQL is Setup.")
    except (Exception, psycopg2.DatabaseError) as error :
        print ("Error while creating PostgreSQL table", error)

def connect():
    try:
        connection = psycopg2.connect(user = user,
                                    password = password,
                                    host = host,
                                    port = port,
                                    database = db)
        return connection
    except (Exception, psycopg2.Error) as error :
        print ("Error while connecting to PostgreSQL", error)

    
