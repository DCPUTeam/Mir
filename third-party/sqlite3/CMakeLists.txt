cmake_minimum_required(VERSION 2.8.5)
project(sqlite3)

add_library(sqlite3 STATIC
    sqlite3.h
    sqlite3ext.h
    sqlite3.c
)
set_property(TARGET sqlite3 PROPERTY FOLDER "third-party")
cotire(sqlite3)
