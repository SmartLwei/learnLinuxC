all: printDatabase sqlite3_study
	

printDatabase: printDatabase.cpp
	g++ $^ -L./ -lsqlite3 -lpthread -ldl -o $@

sqlite3_study: sqlite3_study.cpp	
	g++ $^ -L./ -lsqlite3 -lpthread -ldl -o $@

clean:
	-rm printDatabase sqlite3_study
