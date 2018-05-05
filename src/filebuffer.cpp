#include <iostream>
#include <fstream>

#include "filebuffer.h"

using namespace std;

const unsigned long FileBuffer::BUFFER_SIZE;

FileBuffer::FileBuffer(string fileName){
	this->fileName = fileName;
	pthread_mutex_init(&mutex, NULL);
}

bool FileBuffer::test(){
	try {
        flush();
		return true;
    } catch (...){}
    return false;
}

//Returns true if flushed
bool FileBuffer::addLine(const string& line){
	bool flushed = false;

	unsigned long newDataLen = line.length() + 1; //'\n'
	if (dataLen + newDataLen >= BUFFER_SIZE){
		flush();
		flushed = true;
	}
	pthread_mutex_lock(&mutex);
		data += line + '\n';
		dataLen += newDataLen;
	pthread_mutex_unlock(&mutex);

	return flushed;
}

//Returns true if flushed
bool FileBuffer::addLines(const vector<string>& lines){
	bool flushed = false;

	int linesLen = lines.size();
	for (int i = 0; i < linesLen; i++){
		flushed |= addLine(lines.at(i));
	}

	return flushed;
}

void FileBuffer::flush(){
	pthread_mutex_lock(&mutex);
		ofs.exceptions(ios::failbit | ios::badbit);
		ofs.open(fileName, ios::out | ios::app); //Open file for output and append to eof
		if (ofs && ofs.is_open()){
			if (dataLen){
				ofs << data;
			}
		} else {
			throw "There was an error opening the output file.";
		}
		ofs.close();
		data = "";
		dataLen = 0;
	pthread_mutex_unlock(&mutex);
}