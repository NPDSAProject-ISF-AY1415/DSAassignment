#include "LyricDLL.h"

using namespace std;

LyricDLL::LyricDLL(){}
LyricDLL::LyricDLL(string trackid, string mxmatchid){
	this->tid = trackid;
	this->mxmid = mxmatchid;
}
string LyricDLL::getTrackID(){ return this->tid; }
void LyricDLL::setTrackID(string trackid){ this->tid = trackid; }
string LyricDLL::getMusicXMatchID(){ return this->mxmid; }
void LyricDLL::setMusicXMatchID(string mxmatchid){ this->mxmid = mxmatchid; }
void LyricDLL::addWordAndCount(string word){
	istringstream wc(word);
	string token;
	bool isWord = true;
	while (getline(wc, token, ':')){
		//First will be word based on word list, second will be count
		if (isWord){
			this->word.addBack(token);
			isWord = false;
		}
		else {
			this->count.addBack(token);
			break;
		}
	}
}
string LyricDLL::getWordCount(){
	return "...";
}

DoubleLinkedList LyricDLL::getWords(){
	if (this->word.getLength() != 0)
		return this->word;
}

DoubleLinkedList LyricDLL::getCounts(){
	if (this->count.getLength() != 0)
		return this->count;
}