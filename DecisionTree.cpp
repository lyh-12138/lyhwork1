//此注释为新插入用于：Github拉取测试
//2020-05-17
#include <iostream>
#include <vector> 
#include <string>
#include <fstream> 
#include <sstream>
#include <map>
#include <set> 
#include <math.h>
#include <stdlib.h>
#include<iomanip>
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */
//数据节点 
class WeatherData{
private:
	int num;//编号 
	string tiankuan;//天况 
	string wendu;//温度 
	string shidu;//湿度 
	string fengkuan;//风况 
	char fenlei;//分类 
public:
	WeatherData(){}//默认构造函数 
	WeatherData(int n, string t, string w, string s, string fk, char fl){//构造函数 
		num = n;
		tiankuan = t;
		wendu = w;
		shidu = s;
		fengkuan = fk;
		fenlei = fl;
	}
	int getNum(){//返回编号
		return this->num;
	}
	string getTiankuan(){//返回天况
		return this->tiankuan;
	}
	string getWendu(){//返回温度
		return this->wendu;
	}
	string getShidu(){//返回湿度
		return this->shidu;
	}
	string getFengkuan(){//返回风况
		return this->fengkuan;
	}
	char getFenlei(){//返回分类
		return this->fenlei;
	}
	WeatherData(const WeatherData &d){//拷贝构造函数 
		num = d.num;
		tiankuan = d.tiankuan;
		wendu = d.wendu;
		shidu = d.shidu;
		fengkuan = d.fengkuan;
		fenlei = d.fenlei;
	}
	~WeatherData(){}//析构函数
};
//决策树节点 
class TreeNode{
private:
	string NodeValue;//记录根节点值 
	vector<string>side;//记录边 
	vector<TreeNode*>childs;//记录子节点 
	int decision;//进行决策 
public:
	TreeNode(){
	NodeValue="NO";
	decision = 0;
	}
	void putNodeValue(string nv){//写入根节点属性值 
		this->NodeValue = nv;
	}
	void putSide(vector<string>s){//写入边 
		this->side = s;
	}
	void putChilds(vector<TreeNode*>c){//写入节点 
		this->childs = c;
	}
	void putDecision(int dec){//写入决策结果 
		this->decision = dec;
	}
	vector<TreeNode*> getChilds(){//返回子节点 
		return this->childs;
	}
	string getNodeValue(){//返回节点值 
		return this->NodeValue;
	}
	vector<string> getSide(){//返回边 
		return this->side;
	}
	TreeNode(TreeNode &n){//拷贝构造函数 
		NodeValue = n.NodeValue;
		side = n.side;
		childs = n.childs;
	}
	~TreeNode(){}
};
typedef vector<WeatherData> Weather;//自定义类对象容器 
Weather obj;//该容器用来存储表示天气的对象 
vector<string> character;//该容器用来存储表示天气属性的字符
TreeNode* root = NULL;//初始化全局树根节点对象 
//初始化obj和character：从文件中读入一条天气数据，将其赋值给一个类对象并存储在类对象容器中 
void setWeatherData(){
	ifstream in;
	int num = 0;
	string tk = "";
	string wd = "";
	string sd = "";
	string fk = "";
	char fl;
	in.open("F:\\vs2017\\天气决策树\\weatherForecast.txt", ios::in);
	if (!in.is_open()){
		cout << "can not open the file!" << endl;
		exit(0);
	}
	else{
		string str;
		stringstream ss;
		getline(in, str);
		while (getline(in, str)){
			ss << str;
			ss >> num >> tk >> wd >> sd >> fk >> fl;
			WeatherData dd(num, tk, wd, sd, fk, fl);
			obj.push_back(dd);
		}
	}
	character.push_back("天况");
	character.push_back("温度");
	character.push_back("湿度");
	character.push_back("风况");
	character.push_back("分类");
}
//计算香浓熵 
double calculateS(Weather &X){
	int n = X.size();//获取容器大小 
	int i = 0;
	map<char, int>NOP;//定义一个映射表，该表有两个值，第一个值为key,第二个为value 
	for (i = 0; i<n; i++){//初始化表，统计分类 
		NOP[X[i].getFenlei()] = 0;
	}
	for (i = 0; i<n; i++){//统计每个分类：记录该容器中分类为N或P的次数 
		NOP[X[i].getFenlei()] += 1;
	}
	double shanno = 0;//计算香浓熵 
	map<char, int>::iterator it;
	for (it = NOP.begin(); it != NOP.end(); it++){
		double prob = (double)(it->second) / (double)n;
		shanno -= prob*(log(prob) / log(2));
	}
	return shanno;//返回结果 
}
//根据属性对整个对象类容器进行拆分，如根据天况，拆分得到天况中属性为晴的对象 
Weather splitDataSet(Weather &X, string bestfeature, string value) {
	Weather result;
	int n = X.size();
	int flag = 0;
	if (bestfeature == "天况"){
		flag = 1;
	}
	else if (bestfeature == "温度") {
		flag = 2;
	}
	else if (bestfeature == "湿度") {
		flag = 3;
	}
	else if (bestfeature == "风况") {
		flag = 4;
	}
	for (int i = 0; i<n; i++){
		switch (flag){
		case 1:
			if (X[i].getTiankuan() == value){
				result.push_back(X[i]);
			}
			break;
		case 2:
			if (X[i].getWendu() == value){
				result.push_back(X[i]);
			}
			break;
		case 3:
			if (X[i].getShidu() == value){
				result.push_back(X[i]);
			}
			break;
		case 4:
			if (X[i].getFengkuan() == value){
				result.push_back(X[i]);
			}
			break;
		}
	}
	return result;
}
//选取属性集中的属性值，比如属性天况，该函数可用于统计其属性值，如晴，阴等 
vector<string> featureValue(Weather &X, string feature){
	vector<string>featureList;//放置属性值的容器 
	set<string>s;//定义一个集合，目的在于消除重复的属性值 
	int n = X.size();
	int i = 0;
	for (i = 0; i<n; i++) {
		if (feature == "天况"){
			s.insert(X[i].getTiankuan());
		}
		else if (feature == "温度") {
			s.insert(X[i].getWendu());
		}
		else if (feature == "湿度") {
			s.insert(X[i].getShidu());
		}
		else if (feature == "风况") {
			s.insert(X[i].getFengkuan());
		}
	}
	set<string>::iterator it;//通过集合消除容器中重复元素的过程 
	for (it = s.begin(); it != s.end(); it++) {
		featureList.push_back(*it);
	}
	return featureList;
}
//选择最好的特征作为根节点 
string chooseBestFeature(Weather &X, vector<string> characters){
	int n = character.size() - 1;
	double bestEntropy = calculateS(X);  //初始香农熵（M(C)) 
	double bestInfoGain = 0;   //最大的信息增益
	string bestFeature;       //最好的特征
	int i;
	vector<string>featureList;
	for (i = 0; i<n; i++){
		double newEntropy = 0;//各分支条件熵(B(C))
		featureList = featureValue(X, characters[i]);//获取某属性的属性值 
		for (int j = 0; j<featureList.size(); j++) {//对属性值进行循环 
			Weather subData = splitDataSet(X, character[i], featureList[j]);//筛选符合当前属性的类对象 
			double prob = (double)subData.size() / (double)X.size();//计算概率 
			newEntropy += prob * calculateS(subData);//计算它的条件熵 
		}
		double gain = bestEntropy - newEntropy;//信息增益 
		if (gain >= bestInfoGain){
			bestInfoGain = gain;//获取最佳信息增益 
			bestFeature = characters[i];//选择最佳属性 
		}
	}
	//每确定一个属性，将从属性集中去掉该属性 
	vector<string>::iterator it;
	for (it = character.begin(); it != character.end(); it++){
		if (*it == bestFeature){
			character.erase(it);
			break;
		}
	}
	return bestFeature;//返回选定的最佳属性集 
}
TreeNode* createDicisionTree(TreeNode*root, Weather &X){
	int i;
	vector<string>characters;//根据该容器进行根节点筛选，每筛选一次，属性减一
	characters = character;
	if (root == NULL){//判断决策树是否存在
		root = new TreeNode();//当决策树不存在时新建一个节点，作为决策树的根节点
	}
	set<char>s;//确定该分类下的数据是否都属于同一类：查重 
	int num = X.size();
	for (i = 0; i<num; i++){
		s.insert(X[i].getFenlei());//s类型为集合，用s记录数据集中的分类，根据集合的无序性可以判断，分类是否唯一
	}
	if (s.size() == 1){//如果分类唯一，划分结束，无需在向下划分
		if (*s.begin() == 'N'){
			int dec = -1;//-1表示该分支下的分类中，所有分类都为N
			root->putDecision(dec);
		}
		else{
			int dec = 1;//1表示该分支下的分类中，所有分类都为P
			root->putDecision(dec);
		}
		return root;
	}
	else{//如果分类不唯一，表示该分支下的数据集还可继续划分
		root->putDecision(0);//记录该分支下数据不唯一
	}
	if (characters.size() == 1 && s.size() != 1){//当所有属性划分完，但是分类仍然不唯一，划分失败
		cout << "决策树构建失败!失败原因：当前树节点无法继续向下分类且当前节点下的分类不唯一" << endl;
		exit(0);
	}
	string bestFeature = chooseBestFeature(X, characters);//选取一个属性作为根节点 
	root->putNodeValue(bestFeature);//更新决策树节点对应的属性 
	vector<string>featureList = featureValue(X, bestFeature);//获取该属性的属性值
	root->putSide(featureList);//更新决策树节点对应的边 
	vector<TreeNode*>childs;//定义一个子节点集，为每个边对应下的子节点
	int n = featureList.size();
	for (i = 0; i<n; i++){//通过属性值向下划分数据集
		Weather subData = splitDataSet(X,bestFeature, featureList[i]);//筛选符合符合属性值的数据集
		TreeNode*newNode = new TreeNode();//为每个属性值创建一个新节点
		childs.push_back(newNode);//新节点添加到容器
		createDicisionTree(newNode, subData);//递归向下划分
	}
	root->putChilds(childs);
	return root;
}
//打印决策树 
void printTree(TreeNode*root,Weather &X,int p){
	int i = 0, j = 0, n = 0, m = 0 , x=0;
	vector<string> side;
	vector<TreeNode*>childs;
	side = root->getSide();
	childs = root->getChilds();
	n = side.size();
	m = childs.size();
	for (x = 0; x < p; x++){
		cout << '\t';
	}
	cout <<"节点："<<root->getNodeValue() << endl;
	if (n == m){
		for (x = 0; x < p; x++){
			cout << '\t';
		}
		cout << "分支：" << '\n' ;
		for (i = 0; i < n; i++){
			for (x = 0; x < p; x++){
				cout << '\t';
			}
			cout << side[i] << '\n';
			Weather subData = splitDataSet(X, root->getNodeValue(), side[i]);
			if (childs[i]->getNodeValue() == "NO"){
				int s = subData.size();
				for (j = 0; j < s; j++){
					for (x = 0; x < p; x++){
						cout << '\t';
					}
					cout << '\t';
					cout << subData[j].getNum() << '\t'
						<< subData[j].getTiankuan() << '\t'
						<< subData[j].getWendu() << '\t'
						<< subData[j].getShidu() << '\t'
						<< subData[j].getFengkuan() << '\t'
						<< subData[j].getFenlei() << '\n' ;
				}
			}
			else{
				printTree(childs[i], subData,p+1);
			}
		}
	}
}
//撤销决策树 
void freeTree(TreeNode*root){
	vector<TreeNode*>c = root->getChilds();
	int n = c.size();
	int i = 0;
	if (n>0){
		for (i = 0; i<n; i++)
			freeTree(c[i]);	
	}
	delete root;
}
int main(int argc, char *argv[]) {
	setWeatherData();
	/*Weather::iterator it;
	for (it = obj.begin(); it != obj.end(); it++){
		cout << it->getNum() << "  " << it->getTiankuan() << "  "
			<< it->getWendu() << "  " << it->getShidu() << "  "
			<< it->getFengkuan() << "  " << it->getFenlei() << endl;
	}*/
	root=createDicisionTree(root, obj);
	cout << "该决策树为：" << endl;
	printTree(root,obj,0);
	freeTree(root);
	root = NULL;
	return 0;
}
