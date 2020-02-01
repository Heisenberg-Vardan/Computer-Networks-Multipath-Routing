#include<cstdlib>
#include<cstdio>
#include<iostream>
#include<math.h>
#include<vector>
#include<fstream>
#include<time.h>
#include<stdlib.h>


#define ROUTER_RANGE 20
#define AREA_LENGTH 200
#define AREA_BREADTH 200
#define DEFAULT_SERVICE_RATE 320
#define MAX_ROUTERS 200
#define PI 2*acos(0.0)
#define SINK 0


using namespace std;

ofstream newfile;


class node{
	public:

	int ID; 
	int servRate;
	int inpRate;
	int posX;
	int posY;
	int TR;
	int enrg;
	float depth;
	float ratio;
	vector<int> neighSet;

	public:

	node(){

		ID=0;
		inpRate=0;
		posX=0;
		posY=0;
		servRate=DEFAULT_SERVICE_RATE;
		TR=ROUTER_RANGE;
		enrg=0;
		depth=SINK;
		ratio=enrg/depth;
	}

	node(int id, int ir, int x, int y,int e, float d){
		ID=id;
		inpRate=ir;
		posX=x;
		posY=y;
		servRate=DEFAULT_SERVICE_RATE;
		TR=ROUTER_RANGE;
		enrg=e;	
		depth=d;
		ratio=enrg/depth;

	}
	bool isNeighbor(node j){
		double dist= pow((pow(posX-j.posX,2.0)+pow(posY-j.posY,2.0)),0.5);
		if(dist<=TR)
			return true;
		else return false;
	}	

};
node routers[MAX_ROUTERS];
int visited[MAX_ROUTERS];

void findNeighbors(int nR){

	int i=1;
	for(;i<=nR;i++){
		cout<<"\n Neighbour Of "<<i<<": ";

		int j=i+1;
		for(;j<=nR;j++){

			if(routers[i].isNeighbor(routers[j])){
				cout<<" "<<j;
				routers[i].neighSet.push_back(j);
				routers[j].neighSet.push_back(i);

			}

		}
	}

}

int leastCost(int i, int d){

	visited[i]=1;
	int x=0;
	if(routers[i].depth!=SINK&&routers[i].enrg>=0)
	
	{
		int minC=100000;
		int slctdF,slctd;

		double did= pow((pow(routers[i].posX-routers[d].posX,2)+pow(routers[i].posY-routers[d].posY,2)),0.5);

		double theta=acos(routers[i].TR/(2*did));


		vector<int>::iterator it;
		it= (routers[i].neighSet).begin();

		double costj,r=0;



		for(;it!=routers[i].neighSet.end();it++){

			int j =*it;
		
		if(visited[j])continue;
			double dij= pow((pow(routers[i].posX-routers[j].posX,2)+pow(routers[i].posY-routers[j].posY,2)),0.5);
			double djd= pow((pow(routers[j].posX-routers[d].posX,2)+pow(routers[j].posY-routers[d].posY,2)),0.5);

			double Eij=pow(dij,2)/pow(routers[i].TR,2);

			int ax, ay, bx, by;

			ax=routers[j].posX-routers[i].posX;
			ay=routers[j].posY-routers[i].posY;
			bx=routers[d].posX-routers[i].posX;
			by=routers[d].posY-routers[i].posY;

			double alpha=acos((ax*bx+ay*by)/(dij*did));

			double Aijd=alpha/PI;

			double rhoj=(double)(routers[j].inpRate)/(double)(routers[j].servRate);

			double Pij= (routers[i].TR+(did-djd))/(2*routers[j].TR);

			vector<int>::iterator it1;

			double posP=0.1;

			int k,posN=0,N=0;
			for(it1=routers[j].neighSet.begin(); it1!=routers[j].neighSet.end();it1++){
				k=*it1;
				int cx, cy, dx, dy;
				cx=routers[k].posX-routers[j].posX;
				cy=routers[k].posY-routers[j].posY;
				dx=routers[d].posX-routers[j].posX;
				dy=routers[d].posY-routers[j].posY;

				double djk= pow((pow(routers[j].posX-routers[k].posX,2)+pow(routers[j].posY-routers[k].posY,2)),0.5);

				double beta=acos((cx*dx)+(cy*dy))/(djk*djd);
				if(beta*100<(PI/2)*100){
					posN+=1;
				}
	
				N++;
			}
			posP=(posP+posN)/N;


			costj
			
			
			=(Eij*Aijd*rhoj)/(Pij);
			if(r<routers[j].ratio)
        		r=routers[j].ratio;
        	
			costj*=1000;
			cout<<"\nID : "<<routers[j].ID<<"   POS : ("<<routers[j].posX<<","<<routers[j].posY<<")    COST: "<<costj<<"   E/R:  "<<routers[j].ratio<<"\n";

			if(costj<=minC ){
				minC=costj;
				slctdF=j;
				if(r>routers[j].ratio&&slctdF==j)
        				r+=routers[j].ratio;
        				slctd=j;
   			 }

	}
	return slctd;
	}
	else 
		return i;

}

void transmitPacket(int sID, int dID){
	int k=sID;
	int j=MAX_ROUTERS;

	while(k!=dID){
		
		j=k;

	label:	cout<<"\nNODE:"<<k<<"\nPOS : ("<<routers[k].posX<<","<<routers[k].posY<<") \t Depth : "<<routers[k].depth<<"\t E/D Ratio :" <<routers[k].ratio;



		k=leastCost(k,dID);
		visited[k]=1;
			cout<<"\n\nBroadcasts To :\n\nNODE :"<<k<<"\nPOS : ("<<routers[k].posX<<","<<routers[k].posY<<") \t Depth :"<<routers[k].depth<<"\t E/D Ratio :" <<routers[k].ratio<<"\n\n";


	}

	if(k==dID)
		cout<<"\n"<<"Packet transmition form node "<<sID<<" to node "<<k<<" is successfull "<<"\n";
	else
		cout<<"\n"<<"Packet Lost!!!\n";

}




int main(){

	int noRtrs=MAX_ROUTERS;
	cout<<"No of Nodes: "<<noRtrs<<"\n";

	int i;

	for(i=1;i<=noRtrs;i++){
		int tempIR, tempX, tempY, tempE;
		float tempD;
		tempIR=rand()%212+32;
		tempX=rand()%200+1;
		tempY=rand()%200+1;
		tempE=rand() %101;
		tempD=rand()%21-SINK;
		node temp(i,tempIR,tempX,tempY,tempE,tempD);
		routers[i]=temp;
		routers[i].inpRate=tempIR;
	}
	findNeighbors(noRtrs);

	int srcID=rand()%MAX_ROUTERS;

	cout<<"\n Source Node: "<<srcID;

	int destID=srcID;



	while(srcID==destID)
		destID=rand()%MAX_ROUTERS-50;

	cout<<"\n\nSource Node: "<<srcID<<"\n";
	cout<<"Destination Node: "<<destID<<"\n";
	transmitPacket(srcID, destID);

   
	return 0;
} 




