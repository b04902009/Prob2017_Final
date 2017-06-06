#include <bits/stdc++.h>

using namespace std;

int sum[3];
int co[3];

int main(){
	int a,b,c;
	int counter = 0;
	while(scanf("%d%d%d",&a,&b,&c)!=EOF){
		sum[1]+=a;
		sum[2]+=b;
		co[c]++;
		counter++;
	}
	cout<<"N = "<<counter<<endl;
	cout<<"Player 1 avg points: "<<(double)sum[1]/(double)counter<<endl;

	cout<<"Player 2 avg points: "<<(double)sum[2]/(double)counter<<endl;
	cout<<"Player 1 won: "<<co[1]<<endl;
	cout<<"Player 2 won: "<<co[2]<<endl;
	cout<<",Draw: "<<co[0]<<endl;
}
