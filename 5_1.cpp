#include<stdio.h>
#include<assert.h>
#include<deque>
#include<iostream>
#include <algorithm>
typedef struct tagACTION{//倒水动作模型 
	int from;
	int to;
	int water;
}ACTION;
static const int BUCKETS_COUNT=3;
struct BucketState{//水桶状态
	int bucket_s[3]; 
	ACTION curAction;//记录倒水动作 
	bool CanTakeDumpAction(int from,int to); 
	bool IsFinalState();
	bool IsBucketEmpty(int from);
	bool IsBucketFull(int to);
	bool DumpWater(int from,int to,BucketState& next);
	void SetBuckets(int *bucket_s);
	void SetAction(int dump_water,int from,int to);
};
bool BucketState::IsFinalState(){
	if(bucket_s[0]==4&&bucket_s[1]==4&&bucket_s[2]==0){
		return true;
	}
	return false;
}
void BucketState::SetBuckets(int *bucket_s_other){
	bucket_s[0]=bucket_s_other[0];
	bucket_s[1]=bucket_s_other[1];
	bucket_s[2]=bucket_s_other[2];
}
void BucketState::SetAction(int dump_water,int from,int to){
	curAction.from =from;
	curAction.to=to;
	curAction.water=dump_water;
}  
bool BucketState::DumpWater(int from,int to,BucketState& next){//倒水动作,结构参数next有取地址符，可直接在该函数中操作（*结构方法可直接使用自身的属性，而其他结构的属性则需通过参数传递。注意区别！！！*） 
	const int bucket_capicity[3]={8,5,3}; 
	next.SetBuckets(bucket_s);
	int dump_water=bucket_capicity[to]-next.bucket_s[to];
	if(next.bucket_s[from]>=dump_water){
		next.bucket_s[to]+=dump_water;
		next.bucket_s[from]-=dump_water;
	}
	else{
		next.bucket_s[to]+=next.bucket_s[from];
		dump_water=next.bucket_s[from];
		next.bucket_s[from]=0;
	}
	if(dump_water>0){
		next.SetAction(dump_water,from,to);
		return true;
	}
	return false;
} 
bool BucketState::IsBucketEmpty(int from){
	if(bucket_s[from]!=0){
		return false;
	}
	return true;
}
bool BucketState::IsBucketFull(int to){
	if(to==0&&bucket_s[to]>=8){
		return true;
	}
	if(to==1&&bucket_s[to]>=5){
		return true;
	}
	if(to==2&&bucket_s[to]>=3){
		return true;
	}
	return false;
}
bool BucketState::CanTakeDumpAction(int from,int to){//判断倒水动作的合理性，以搜索符合题意的状态树 
	assert((from>=0)&&(from<BUCKETS_COUNT));
	assert((to>=0)&&(to<BUCKETS_COUNT));
	if((from!=to)&&!IsBucketEmpty(from)&&!IsBucketFull(to)){
		return true;
	}
	return false; 
}
bool IsProcessedState(std::deque<BucketState>& states,const BucketState& newState){//检查重复状态，避免状态循环 
	int i;
	for(i=0;i<states.size();i++){
		if(states[i].bucket_s[0]==newState.bucket_s[0]&&states[i].bucket_s[1]==newState.bucket_s[1]&&states[i].bucket_s[2]==newState.bucket_s[2]){
			return true;
		}
	}
	return false;
}
void SearchState(std::deque<BucketState>& states){//搜素算法第一部分，遍历状态树节点的所有子节点 
	void SearchStateOnAction(std::deque<BucketState>& states,BucketState& current,int from,int to);
	void PrintResult(std::deque<BucketState>& states);
	int i,j;
	BucketState current=states.back();
	if(current.IsFinalState()){
		PrintResult(states);
		return;
	}
	for(j=0;j<BUCKETS_COUNT;j++){
		for(i=0;i<BUCKETS_COUNT;i++){
			SearchStateOnAction(states,current,i,j);
		}
	}
}
void SearchStateOnAction(std::deque<BucketState>& states,BucketState& current,int from,int to){//搜索算法第二部分，进行倒水操作并递归遍历更深层节点。 
	if(current.CanTakeDumpAction(from,to)){
		BucketState next;
		bool bDump=current.DumpWater(from,to,next);
		if(bDump&&!IsProcessedState(states,next)){
			states.push_back(next);
			SearchState(states);
			states.pop_back();//回溯处理 
		}
	}
}
void PrintResult(std::deque<BucketState>& states){//将搜索到的状态树的分支打印出来即可得到一组解 
	int i;
	static int measureNum=1;
	std::cout<<"方法"<<measureNum<<":\n初始水桶状态为：8 0 0(-1表示初始化用水桶)\n"<<std::endl;
	for(i=0;i<states.size();i++){
		std::cout<<"步骤"<<i<<"：从"<<states[i].curAction.from<<"倒"<<states[i].curAction.water<<"升水到"<<states[i].curAction.to<<
		" 水桶状态："<<states[i].bucket_s[0]<<" "<<states[i].bucket_s[1]<<" "<<states[i].bucket_s[2]<<"\n"<<std::endl; 
	}
	measureNum++;
}
int main(){
	std::deque<BucketState> states;
	BucketState firstBucket;
	printf("问题的解：\n");
	firstBucket.bucket_s[0]=8;
	firstBucket.bucket_s[1]=0;
	firstBucket.bucket_s[2]=0;
	firstBucket.curAction.from=-1;
	firstBucket.curAction.to=1;
	firstBucket.curAction.water=8;
	states.push_back(firstBucket); 
	SearchState(states);
} 
