#ifndef MUSIC
#define MUSIC

#include<iostream>
#include<string> 
using namespace std;

class Music
{
private:
	//initialize private variables for the music
	string tid;
	string tArtist;
	string tTitle;
	string mxmid;
	string mArtist;
	string mTitle;
public:
	Music::Music(string id, string art, string title, string mid, string mart, string mtitl);
	void setTid(string id);
	string getTid();
	void setTArtist(string art);
	string getTArtist();
	void setTTitle(string title);
	string getTTitle();
	void setMid(string mid);
	string getMid();
	void setMArtist(string mart);
	string getMArtist();
	void setMTitle(string mtitl);
	string getMTitle();
	void printMusicInfo();
};

#endif