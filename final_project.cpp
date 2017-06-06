/* 
CSIE Probability 2017
Final Project Implementation
Team Member: B04902009 B04902077
Compile with -D SHOWFULL can show all information
./a.out --full 0 0 0 0 0 0 
./a.out --board
*/

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <cstring>
#include <ctime>
#include <utility>

#define MAX_RADIUS 3
#define MAX_ROUND 10
#define TEST_TIME 10000
using namespace std;

inline int maxi(int a, int b){return (a>b)?a:b;}

const double prob[7] = {0.054314335,0.094728608,0.026434729,0.003126288,0.000155284,3.20949e-6,2.73756e-8};
const int space_co[7] = {1,6,12,18,24,30,36};
double dist_prob[7];

void initi(){
	srand(time(NULL));
	for(int h=0;h<7;h++) dist_prob[h] = prob[h]*space_co[h];
	return;
}

class Board{
private:
	int board[14][14];
public:
	void init(int data[14][14]){
		for(int g=0;g<14;g++){
			for(int h=0;h<14;h++){
				board[g][h] = data[g][h];
			}
		}
		return;
	};
	void init(){
		memset(board,0,sizeof(board));
		return;
	};
	int count_dist(int ax,int ay,int bx,int by){
		int dx=abs(ax-bx), dy=abs(ay-by);
		return (dx>=dy)?dx:((dx+dy)/2);
	};
	int count_dist(int dx,int dy){
		dx = abs(dx);
		dy = abs(dy);
		return (dx>=dy)?dx:((dx+dy)/2);
	};
	int query(int x,int y){
		return board[x+6][y+6];
	};
	void assign(int x,int y,int val){
		board[x+6][y+6] = val;
		return;
	};
	void throw_assign(int x,int y,int val){
		rand();
		double r = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
		int ptr = 0;
		double sum = 0.0;
		int listx[50];
		int listy[50];
		int co = 0;
		while(r>=sum && ptr<7){
			sum+=dist_prob[ptr];
			ptr++;
		}
		ptr--;
		for(int cx = -3;cx<=3;cx++){
			for(int cy = (abs(cx)%2==0?-6:-5);cy<=6;cy+=2){
				if(count_dist(cx,cy,x,y)==ptr && count_dist(cx,cy)<=3){
					listx[co] = cx;
					listy[co] = cy;
					co++;
				}
			}
		}
		int e = rand()%(space_co[ptr]);
		if(e<co){
			#ifdef SHOWFULL
			printf("Player %d: Aim (%d,%d), Got (%d,%d)\n",val,x,y,listx[e],listy[e]);
			#endif
			assign(listx[e],listy[e],val);
		}
		else{
			#ifdef SHOWFULL
			printf("Player %d: Aim (%d,%d), Missed\n",val,x,y);
			#endif
		}
	};
	int get_pscore(int val){ // m = 1
		int dx=1,dy=1;
		int score = 0;
		for(int x=-3;x<=3;x++){
			int count = 0;
			for(int c=-1;c<=1;c+=2){
				int ptr = c==-1?0:1;
				while(count_dist(x+ptr*c*dx,(-x)+ptr*c*dy)<=3){
					if(query(x+ptr*c*dx,(-x)+ptr*c*dx) == val) count++;
					ptr++;
				}
			}
			score = maxi(score,count);
		}
		return score;
	};
	int get_nscore(int val){ // m = -1
		int dx=1,dy=-1;
		int score = 0;
		for(int x=-3;x<=3;x++){
			int count = 0;
			for(int c=-1;c<=1;c+=2){
				int ptr = c==-1?0:1;
				while(count_dist(x+ptr*c*dx,x+ptr*c*dy)<=3){
					if(query(x+ptr*c*dx,x+ptr*c*dy) == val) count++;
					ptr++;
				}
			}
			score = maxi(score,count);
		}
		return score;
	};
	int get_vscore(int val){ // better solution with observing abs(x)+number == constant
		int dx=0,dy=2;
		int score = 0;
		for(int x=-3;x<=3;x++){
			
			int count = 0;
			int y = abs(x)%2;
			for(int c=-1;c<=1;c+=2){
				int ptr = c==-1?0:1;
				while(count_dist(x+ptr*c*dx,y+ptr*c*dy)<=3){
					if(query(x+ptr*c*dx,y+ptr*c*dy) == val) count++;
					ptr++;
				}
			}
			score = maxi(score,count);
		}
		return score;
	};
	int get_score(int val){
		return maxi(get_vscore(val),maxi(get_pscore(val),get_nscore(val)));
	};
	void print_board(){
		char printable[14][14];
		memset(printable,' ',sizeof(printable));
		for(int cx = -3;cx<=3;cx++){
			for(int cy = (abs(cx)%2==0?-6:-5);cy<=6;cy+=2){
				if(count_dist(cx,cy)<=3){
					printable[cx+6][cy+6] = (query(cx,cy)==0?'-':query(cx,cy)+'0');
				}
			}
		}
		for(int g=12;g>=0;g--){
			for(int h=3;h<14;h++){
				printf("%c",printable[h][g]);
			}
			printf("\n");
		}
	};
};

Board game;

double find_best(int val,int *best_x,int *best_y,bool future,bool my_score,bool opp_score,bool get_max){
	double ans = (get_max?1.0:-1.0)*(-5000000.0);
	int bx=-1,by=-1;
	int dx=0,dy=2;
	for(int aim_px=-3;aim_px<=3;aim_px++){
		int aim_py = abs(aim_px)%2;
		for(int aim_c=-1;aim_c<=1;aim_c+=2){
			int aim_ptr = aim_c==-1?0:1;
			int aim_x = aim_px + aim_ptr*aim_c*dx;
			int aim_y = aim_py + aim_ptr*aim_c*dy;
			while(game.count_dist(aim_x,aim_y)<=MAX_RADIUS){
				double expe = 0.0;
				double cum_prob = 0.0;
				for(int x=-3;x<=3;x++){
					int y = abs(x)%2;
					for(int c=-1;c<=1;c+=2){
						int ptr = c==-1?0:1;
						int cx = x+ptr*c*dx, cy = y+ptr*c*dy;
						while(game.count_dist(cx,cy)<=3){
							int ori = game.query(cx,cy);
							game.assign(cx,cy,val);
							double tempp = (prob[game.count_dist(cx,cy,aim_x,aim_y)] * (future?(opp_score?(-find_best(val==1?2:1,NULL,NULL,false,opp_score,false,true)+(my_score?game.get_score(val):(game.get_score(val)-game.get_score(val==1?2:1)))):(-find_best(val==1?2:1,NULL,NULL,false,opp_score,false,true))):(my_score?game.get_score(val):(game.get_score(val)-game.get_score(val==1?2:1)))));
							expe+=tempp;
							cum_prob += prob[game.count_dist(cx,cy,aim_x,aim_y)];
							game.assign(cx,cy,ori);
							ptr++;
							cx = x+ptr*c*dx;
							cy = y+ptr*c*dy;
						}
					}
				}
				double tempp = ((1.0-cum_prob) * (future?(opp_score?(-find_best(val==1?2:1,NULL,NULL,false,opp_score,false,true)+(my_score?game.get_score(val):(game.get_score(val)-game.get_score(val==1?2:1)))):(-find_best(val==1?2:1,NULL,NULL,false,opp_score,false,true))):(my_score?game.get_score(val):(game.get_score(val)-game.get_score(val==1?2:1)))));
				expe += tempp;
				if((ans<expe && get_max) || (ans>expe && !get_max)){
					ans=expe;
					bx = aim_x;
					by = aim_y;
				}
				aim_ptr++;
				aim_x = aim_px + aim_ptr*aim_c*dx;
				aim_y = aim_py + aim_ptr*aim_c*dy;
			}
		}
	}
	if(best_x!=NULL && best_y!=NULL){
		*best_x = bx;
		*best_y = by;
	}
	return ans;
}

int main(int argc, char **argv){
	initi();
	game.init();
	bool full_future[3],full_my_score[3],full_opp_score[3];
	if(strcmp(argv[1],"--full")==0){
		for(int g=3;g<=6;g+=3){
			full_future[g/3] = (strcmp(argv[-1+g],"1")==0);
			full_my_score[g/3] = (strcmp(argv[g],"1")==0);
			full_opp_score[g/3] = (strcmp(argv[1+g],"1")==0);
		}
	}
	if(strcmp(argv[1],"--full")==0){
		int count_win[3];
		int sum_point[3];
		memset(count_win,0,sizeof(count_win));
		memset(sum_point,0,sizeof(sum_point));
		for(int i=0;i<TEST_TIME;i++){
			game.init();
			for(int g=0;g<MAX_ROUND;g++){
				int val = (g%2)+1;
				int best_x,best_y;
				find_best(val,&best_x,&best_y,full_future[val],full_my_score[val],full_opp_score[val],true);
				game.throw_assign(best_x,best_y,val);
			}
			int score1 = game.get_score(1);
			int score2 = game.get_score(2);
			#ifdef SHOWFULL
				game.print_board();
			#endif
			int winner = (score1>score2)?(1):((score2>score1)?(2):(0));
			printf("%d %d %d\n",score1,score2,winner);
			fflush(stdout);
			sum_point[1]+=score1;
			sum_point[2]+=score2;
			count_win[winner]++;
		}
		printf("-------END SIMULATION-------\n");
		printf("Player 1 avg points: ");
		cout<<(double)sum_point[1]/(double)TEST_TIME<<endl;
		printf("Player 2 avg points: ");
		cout<<(double)sum_point[2]/(double)TEST_TIME<<endl;
		printf("Player 1 won: %d\nPlayer 2 won: %d\n,Draw: %d\n",count_win[1],count_win[2],count_win[0]);
	}
	else if(strcmp(argv[1],"--board")==0){
		printf("Please input all data until EOF\n");
		int x,y,val;
		while(scanf("%d %d %d",&x,&y,&val)!=EOF){
			if(val == -1) break;
			game.assign(x,y,val);
		}
		int best_x,best_y;
		for(int player=1;player<=2;player++){
			printf("=========Player %d=========\n",player);
			//no future
			cout<<find_best(player,&best_x,&best_y,false,true,false,true);
			cout<<" "<<best_x<<" "<<best_y<<endl;
			cout<<find_best(player,&best_x,&best_y,false,false,false,true);
			cout<<" "<<best_x<<" "<<best_y<<endl;
			//future
			//cout<<find_best(player,&best_x,&best_y,true,true,true,true);
			//cout<<" "<<best_x<<" "<<best_y<<endl;
			//cout<<find_best(player,&best_x,&best_y,true,true,false,true);
			//cout<<" "<<best_x<<" "<<best_y<<endl;
			cout<<find_best(player,&best_x,&best_y,true,false,true,true);
			cout<<" "<<best_x<<" "<<best_y<<endl;
			cout<<find_best(player,&best_x,&best_y,true,false,false,true);
			cout<<" "<<best_x<<" "<<best_y<<endl;

		}
	}
	return 0;
}