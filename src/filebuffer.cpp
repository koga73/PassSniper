#include <iostream>
#include <fstream>

#include "filebuffer.h"

using namespace std;

const unsigned long FileBuffer::BUFFER_SIZE;

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
bool FileBuffer::addData(const string& newData){
	bool flushed = false;

	pthread_mutex_lock(&mutex);
		unsigned long newDataLen = newData.length();
		if (dataLen + newDataLen >= BUFFER_SIZE){
			pthread_mutex_unlock(&mutex);
			flush();
			pthread_mutex_lock(&mutex);
			flushed = true;
		}
		data += newData;
		dataLen += newDataLen;
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
			if (dataLen){
				ofs << data;
			}
		} else {
			throw "There was an error opening the output file.";
		}
		if (!keepOpen){
			pthread_mutex_unlock(&mutex);
			close();
			pthread_mutex_lock(&mutex);
		}
		data = "";
		dataLen = 0;
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