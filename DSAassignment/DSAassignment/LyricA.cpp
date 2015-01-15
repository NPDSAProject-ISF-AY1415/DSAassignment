#include "LyricA.h"

using namespace std;

LyricA::LyricA(){}
LyricA::LyricA(string trackid, string mxmatchid){
	this->tid = trackid;
	this->mxmid = mxmatchid;
}
string LyricA::getTrackID(){ return this->tid; }
void LyricA::setTrackID(string trackid){ this->tid = trackid; }
string LyricA::getMusicXMatchID(){ return this->mxmid; }
void LyricA::setMusicXMatchID(string mxmatchid){ this->mxmid = mxmatchid; }
void LyricA::addWordAndCount(string word){
	istringstream wc(word);
	string token;
	bool isWord = true;
	while (getline(wc, token, ':')){
		//First will be word based on word list, second will be count
		if (isWord){
			this->word.add(token);
			isWord = false;
		}
		else {
			this->count.add(token);
			break;
		}
	}
}
string LyricA::getWordCount(){
	return "...";
}