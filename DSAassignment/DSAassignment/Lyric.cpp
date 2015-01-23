#include "Lyric.h"

using namespace std;

Lyric::Lyric(){
	this->word.clear();
	this->count.clear();
}

Lyric::Lyric(string trackid, string mxmatchid){
	this->tid = trackid;
	this->mxmid = mxmatchid;
}
string Lyric::getTrackID(){ return this->tid; }
void Lyric::setTrackID(string trackid){ this->tid = trackid; }
string Lyric::getMusicXMatchID(){ return this->mxmid; }
void Lyric::setMusicXMatchID(string mxmatchid){ this->mxmid = mxmatchid; }
void Lyric::addWordAndCount(string word){
	istringstream wc(word);
	string token;
	bool isWord = true;
	while (getline(wc, token, ':')){
		//First will be word based on word list, second will be count
		if (isWord){
			this->word.push_back(token);
			isWord = false;
		}
		else {
			this->count.push_back(token);
			break;
		}
	}
}
vector<string> Lyric::getWords(){
	if (this->word.size() != 0)
		return this->word;
}

vector<string> Lyric::getCounts(){
	if (this->count.size() != 0)
		return this->count;
}