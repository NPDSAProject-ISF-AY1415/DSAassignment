#include"Music.h"
#include<iostream>
#include<string>
using namespace std;
//constructor
Music::Music(string id, string art, string title, string mid, string mart, string mtitl)
{
	this->tid = id;
	this->tArtist = art;
	this->tTitle = title;
	this->mxmid = mid;
	this->mArtist = mart;
	this->mTitle = mtitl;
}
//implement the class functions e.g get, set
void Music::setTid(string id){ tid = id; }
string Music::getTid(){ return tid; }
void Music::setTartist(string art){ tArtist = art; }
string Music::getTartist(){ return tArtist; }
void Music::setTtitle(string title){ tTitle = title; }
string Music::getTtitle(){ return tTitle; }
void Music::setMxmid(string mart){ mxmid = mart; }
string Music::getMxmid(){ return mxmid; }
void Music::setMArtist(string mart){ mArtist = mart; }
string Music::getMartist(){ return mArtist; }
void Music::setMTitle(string mtitl){ mTitle = mtitl; }
string Music::getMTitle(){ return mTitle; }
void Music::printMusicInfo(){
	cout <<  "Million Song Track DB ID: "  << this->tid << endl;
	cout <<  "Million Song Track Artist: " << this->tArtist << endl;
	cout <<  "Million Song Track Title: "  << this->tTitle << endl;
	cout <<  "musicXmatch Track DB ID: "   << this->mxmid << endl;
	cout <<  "musicXmatch Track Artist: "  << this->mArtist << endl;
	cout <<  "musicXmatch Track Title: "   << this->mTitle << endl;
}


