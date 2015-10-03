#include "ofxSQLite.h"
#include "ofxSQLiteInsert.h"
#include "ofxSQLiteUpdate.h"
#include "ofxSQLiteDelete.h"
#include "ofxSQLiteSelect.h"
#include "ofUtils.h"
//#include "ofxSQLiteSimpler.h"

ofxSQLite::ofxSQLite()
:db_name("")
{
}


void ofxSQLite::setup(std::string sDB) {
	db_name = sDB;
	db_file  = ofToDataPath(sDB); 
	if (SQLITE_OK != sqlite3_open(db_file.c_str(), &db)) {
		cout << sqlite3_errmsg(db);
		exit(1);
	}
	cout << "opened:" << db_file << endl;
}	

int ofxSQLite::simpleQuery(const char* pSQL) {
	sqlite3_stmt* statement;
	if (SQLITE_OK != sqlite3_prepare_v2(db, pSQL,-1, &statement, 0)) {
		return sqlite3_errcode(db);
	}
	while(SQLITE_ROW == sqlite3_step(statement));
	return sqlite3_finalize(statement);
}

ofxSQLiteTable ofxSQLite::createTableIfNotExists(std::string sTable){
	ofxSQLiteTable table = ofxSQLiteTable(db, sTable);
	return table;
}

ofxSQLiteInsert ofxSQLite::insert(std::string sTable) {
	ofxSQLiteInsert insert = ofxSQLiteInsert(db, sTable);
	return insert;
}
ofxSQLiteUpdate ofxSQLite::update(std::string sTable) {
	return ofxSQLiteUpdate(db, sTable);
}

ofxSQLiteDelete ofxSQLite::remove(std::string sTable) {
	return ofxSQLiteDelete(db, sTable);
}

ofxSQLiteSelect	ofxSQLite::select(std::string sFields) {
	ofxSQLiteSelect select = ofxSQLiteSelect(db);
	select.select(sFields);
	return select;
}

const char* ofxSQLite::getError() {
	return sqlite3_errmsg(db);
}

int ofxSQLite::lastInsertID() {
	return sqlite3_last_insert_rowid(db);
}

ofxSQLiteSimpler ofxSQLite::operator[](const std::string sKeyValue)  {
	ofxSQLiteSimpler simple(*this, sKeyValue);
	return simple;
}

void ofxSQLite::printTable(std::string sTable) {
	cout << select("*").from(sTable).execute().getResultAsAsciiTable();
}
