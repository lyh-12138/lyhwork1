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
//���ݽڵ� 
class WeatherData{
private:
	int num;//��� 
	string tiankuan;//��� 
	string wendu;//�¶� 
	string shidu;//ʪ�� 
	string fengkuan;//��� 
	char fenlei;//���� 
public:
	WeatherData(){}//Ĭ�Ϲ��캯�� 
	WeatherData(int n, string t, string w, string s, string fk, char fl){//���캯�� 
		num = n;
		tiankuan = t;
		wendu = w;
		shidu = s;
		fengkuan = fk;
		fenlei = fl;
	}
	int getNum(){//���ر��
		return this->num;
	}
	string getTiankuan(){//�������
		return this->tiankuan;
	}
	string getWendu(){//�����¶�
		return this->wendu;
	}
	string getShidu(){//����ʪ��
		return this->shidu;
	}
	string getFengkuan(){//���ط��
		return this->fengkuan;
	}
	char getFenlei(){//���ط���
		return this->fenlei;
	}
	WeatherData(const WeatherData &d){//�������캯�� 
		num = d.num;
		tiankuan = d.tiankuan;
		wendu = d.wendu;
		shidu = d.shidu;
		fengkuan = d.fengkuan;
		fenlei = d.fenlei;
	}
	~WeatherData(){}//��������
};
//�������ڵ� 
class TreeNode{
private:
	string NodeValue;//��¼���ڵ�ֵ 
	vector<string>side;//��¼�� 
	vector<TreeNode*>childs;//��¼�ӽڵ� 
	int decision;//���о��� 
public:
	TreeNode(){
	NodeValue="NO";
	decision = 0;
	}
	void putNodeValue(string nv){//д����ڵ�����ֵ 
		this->NodeValue = nv;
	}
	void putSide(vector<string>s){//д��� 
		this->side = s;
	}
	void putChilds(vector<TreeNode*>c){//д��ڵ� 
		this->childs = c;
	}
	void putDecision(int dec){//д����߽�� 
		this->decision = dec;
	}
	vector<TreeNode*> getChilds(){//�����ӽڵ� 
		return this->childs;
	}
	string getNodeValue(){//���ؽڵ�ֵ 
		return this->NodeValue;
	}
	vector<string> getSide(){//���ر� 
		return this->side;
	}
	TreeNode(TreeNode &n){//�������캯�� 
		NodeValue = n.NodeValue;
		side = n.side;
		childs = n.childs;
	}
	~TreeNode(){}
};
typedef vector<WeatherData> Weather;//�Զ������������ 
Weather obj;//�����������洢��ʾ�����Ķ��� 
vector<string> character;//�����������洢��ʾ�������Ե��ַ�
TreeNode* root = NULL;//��ʼ��ȫ�������ڵ���� 
//��ʼ��obj��character�����ļ��ж���һ���������ݣ����丳ֵ��һ������󲢴洢������������� 
void setWeatherData(){
	ifstream in;
	int num = 0;
	string tk = "";
	string wd = "";
	string sd = "";
	string fk = "";
	char fl;
	in.open("F:\\vs2017\\����������\\weatherForecast.txt", ios::in);
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
	character.push_back("���");
	character.push_back("�¶�");
	character.push_back("ʪ��");
	character.push_back("���");
	character.push_back("����");
}
//������Ũ�� 
double calculateS(Weather &X){
	int n = X.size();//��ȡ������С 
	int i = 0;
	map<char, int>NOP;//����һ��ӳ����ñ�������ֵ����һ��ֵΪkey,�ڶ���Ϊvalue 
	for (i = 0; i<n; i++){//��ʼ����ͳ�Ʒ��� 
		NOP[X[i].getFenlei()] = 0;
	}
	for (i = 0; i<n; i++){//ͳ��ÿ�����ࣺ��¼�������з���ΪN��P�Ĵ��� 
		NOP[X[i].getFenlei()] += 1;
	}
	double shanno = 0;//������Ũ�� 
	map<char, int>::iterator it;
	for (it = NOP.begin(); it != NOP.end(); it++){
		double prob = (double)(it->second) / (double)n;
		shanno -= prob*(log(prob) / log(2));
	}
	return shanno;//���ؽ�� 
}
//�������Զ������������������в�֣�������������ֵõ����������Ϊ��Ķ��� 
Weather splitDataSet(Weather &X, string bestfeature, string value) {
	Weather result;
	int n = X.size();
	int flag = 0;
	if (bestfeature == "���"){
		flag = 1;
	}
	else if (bestfeature == "�¶�") {
		flag = 2;
	}
	else if (bestfeature == "ʪ��") {
		flag = 3;
	}
	else if (bestfeature == "���") {
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
//ѡȡ���Լ��е�����ֵ����������������ú���������ͳ��������ֵ�����磬���� 
vector<string> featureValue(Weather &X, string feature){
	vector<string>featureList;//��������ֵ������ 
	set<string>s;//����һ�����ϣ�Ŀ�����������ظ�������ֵ 
	int n = X.size();
	int i = 0;
	for (i = 0; i<n; i++) {
		if (feature == "���"){
			s.insert(X[i].getTiankuan());
		}
		else if (feature == "�¶�") {
			s.insert(X[i].getWendu());
		}
		else if (feature == "ʪ��") {
			s.insert(X[i].getShidu());
		}
		else if (feature == "���") {
			s.insert(X[i].getFengkuan());
		}
	}
	set<string>::iterator it;//ͨ�����������������ظ�Ԫ�صĹ��� 
	for (it = s.begin(); it != s.end(); it++) {
		featureList.push_back(*it);
	}
	return featureList;
}
//ѡ����õ�������Ϊ���ڵ� 
string chooseBestFeature(Weather &X, vector<string> characters){
	int n = character.size() - 1;
	double bestEntropy = calculateS(X);  //��ʼ��ũ�أ�M(C)) 
	double bestInfoGain = 0;   //������Ϣ����
	string bestFeature;       //��õ�����
	int i;
	vector<string>featureList;
	for (i = 0; i<n; i++){
		double newEntropy = 0;//����֧������(B(C))
		featureList = featureValue(X, characters[i]);//��ȡĳ���Ե�����ֵ 
		for (int j = 0; j<featureList.size(); j++) {//������ֵ����ѭ�� 
			Weather subData = splitDataSet(X, character[i], featureList[j]);//ɸѡ���ϵ�ǰ���Ե������ 
			double prob = (double)subData.size() / (double)X.size();//������� 
			newEntropy += prob * calculateS(subData);//�������������� 
		}
		double gain = bestEntropy - newEntropy;//��Ϣ���� 
		if (gain >= bestInfoGain){
			bestInfoGain = gain;//��ȡ�����Ϣ���� 
			bestFeature = characters[i];//ѡ��������� 
		}
	}
	//ÿȷ��һ�����ԣ��������Լ���ȥ�������� 
	vector<string>::iterator it;
	for (it = character.begin(); it != character.end(); it++){
		if (*it == bestFeature){
			character.erase(it);
			break;
		}
	}
	return bestFeature;//����ѡ����������Լ� 
}
TreeNode* createDicisionTree(TreeNode*root, Weather &X){
	int i;
	vector<string>characters;//���ݸ��������и��ڵ�ɸѡ��ÿɸѡһ�Σ����Լ�һ
	characters = character;
	if (root == NULL){//�жϾ������Ƿ����
		root = new TreeNode();//��������������ʱ�½�һ���ڵ㣬��Ϊ�������ĸ��ڵ�
	}
	set<char>s;//ȷ���÷����µ������Ƿ�����ͬһ�ࣺ���� 
	int num = X.size();
	for (i = 0; i<num; i++){
		s.insert(X[i].getFenlei());//s����Ϊ���ϣ���s��¼���ݼ��еķ��࣬���ݼ��ϵ������Կ����жϣ������Ƿ�Ψһ
	}
	if (s.size() == 1){//�������Ψһ�����ֽ��������������»���
		if (*s.begin() == 'N'){
			int dec = -1;//-1��ʾ�÷�֧�µķ����У����з��඼ΪN
			root->putDecision(dec);
		}
		else{
			int dec = 1;//1��ʾ�÷�֧�µķ����У����з��඼ΪP
			root->putDecision(dec);
		}
		return root;
	}
	else{//������಻Ψһ����ʾ�÷�֧�µ����ݼ����ɼ�������
		root->putDecision(0);//��¼�÷�֧�����ݲ�Ψһ
	}
	if (characters.size() == 1 && s.size() != 1){//���������Ի����꣬���Ƿ�����Ȼ��Ψһ������ʧ��
		cout << "����������ʧ��!ʧ��ԭ�򣺵�ǰ���ڵ��޷��������·����ҵ�ǰ�ڵ��µķ��಻Ψһ" << endl;
		exit(0);
	}
	string bestFeature = chooseBestFeature(X, characters);//ѡȡһ��������Ϊ���ڵ� 
	root->putNodeValue(bestFeature);//���¾������ڵ��Ӧ������ 
	vector<string>featureList = featureValue(X, bestFeature);//��ȡ�����Ե�����ֵ
	root->putSide(featureList);//���¾������ڵ��Ӧ�ı� 
	vector<TreeNode*>childs;//����һ���ӽڵ㼯��Ϊÿ���߶�Ӧ�µ��ӽڵ�
	int n = featureList.size();
	for (i = 0; i<n; i++){//ͨ������ֵ���»������ݼ�
		Weather subData = splitDataSet(X,bestFeature, featureList[i]);//ɸѡ���Ϸ�������ֵ�����ݼ�
		TreeNode*newNode = new TreeNode();//Ϊÿ������ֵ����һ���½ڵ�
		childs.push_back(newNode);//�½ڵ���ӵ�����
		createDicisionTree(newNode, subData);//�ݹ����»���
	}
	root->putChilds(childs);
	return root;
}
//��ӡ������ 
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
	cout <<"�ڵ㣺"<<root->getNodeValue() << endl;
	if (n == m){
		for (x = 0; x < p; x++){
			cout << '\t';
		}
		cout << "��֧��" << '\n' ;
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
//���������� 
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
	cout << "�þ�����Ϊ��" << endl;
	printTree(root,obj,0);
	freeTree(root);
	root = NULL;
	return 0;
}
