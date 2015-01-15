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
	void setTartist(string art);
	string getTartist();
	void setTtitle(string title);
	string getTtitle();
	void setMxmid(string mid);
	string getMxmid();
	void setMArtist(string mart);
	string getMartist();
	void setMTitle(string mtitl);
	string getMTitle();
	void printMusicInfo();
};
