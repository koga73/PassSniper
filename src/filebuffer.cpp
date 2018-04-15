#include <iostream>
#include <fstream>

#include "filebuffer.h"

using namespace std;

const int FileBuffer::BUFFER_SIZE;

FileBuffer::FileBuffer(string fileName){
	this->fileName = fileName;
}
FileBuffer::FileBuffer(string fileName, bool keepOpen){
	this->fileName = fileName;
	this->keepOpen = keepOpen;
}
FileBuffer::~FileBuffer(){
	close();
}

bool FileBuffer::test(){
	try {
        flush();
		return true;
    } catch (...){}
    return false;
}

//Returns true if flushed
bool FileBuffer::addLine(string line){
	bool flushed = false;

	pthread_mutex_lock(&mutex);
		line += "\n";
		int lineLen = line.length();
		linesLen = lines.length();
		if (linesLen + lineLen >= BUFFER_SIZE){
			pthread_mutex_unlock(&mutex);
			flush();
			pthread_mutex_lock(&mutex);
			flushed = true;
		}
		lines += line;
		linesLen += lineLen;
	pthread_mutex_unlock(&mutex);

	return flushed;
}

void FileBuffer::flush(){
	pthread_mutex_lock(&mutex);
		if (!isOpen){
			isOpen = true;
			ofs.exceptions(ios::failbit | ios::badbit);
			ofs.open(fileName, ios::out | ios::app); //Open file for output and append to eof
			if (ofs && ofs.is_open()){
				isOpen = true;
			}
		}
		if (isOpen){
			if (linesLen >= 0){
				ofs << lines;
			}
		} else {
			throw "There was an error opening the output file.";
		}
		if (!keepOpen){
			pthread_mutex_unlock(&mutex);
			close();
			pthread_mutex_lock(&mutex);
		}
		lines = "";
		linesLen = 0;
	pthread_mutex_unlock(&mutex);
}

void FileBuffer::close(){
	if (!isOpen){
		return;
	}
	pthread_mutex_lock(&mutex);
		isOpen = false;
		ofs.close();
	pthread_mutex_unlock(&mutex);
}