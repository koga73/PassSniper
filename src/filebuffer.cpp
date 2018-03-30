#include <iostream>
#include <fstream>

#include "filebuffer.h"

using namespace std;

const int FileBuffer::MAX_LINES;

FileBuffer::FileBuffer(string fileName){
	this->fileName = fileName;
}
FileBuffer::FileBuffer(string fileName, bool keepOpen){
	this->fileName = fileName;
	this->keepOpen = keepOpen;
}

bool FileBuffer::test(){
	try {
        flush();
		return true;
    } catch (...){}
    return false;
}

void FileBuffer::addLine(string line){
	if (lineIndex < 0){ //Empty
		lineIndex = 0;
	}
	lines[lineIndex] = line;
	lineIndex++;
	if (lineIndex == MAX_LINES){
		flush();
	}
}

void FileBuffer::flush(){
	if (!isOpen){
		isOpen = true;
		ofs.exceptions(ios::failbit | ios::badbit);
		ofs.open(fileName, ios::out | ios::app); //Open file for output and append to eof
		if (ofs && ofs.is_open()){
			isOpen = true;
		}
	}
	if (isOpen){
		if (lineIndex >= 0){ //-1 means we are empty
			int maxLine = min(lineIndex, MAX_LINES);
			for (int i = 0; i < maxLine; i++){
				ofs << lines[i] << endl;
			}
		}
	} else {
        throw "There was an error opening the output file.";
	}
	if (!keepOpen){
		close();
	}
	lineIndex = -1;
}

void FileBuffer::close(){
	isOpen = false;
	ofs.close();
}