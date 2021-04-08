#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define infinity 65536
typedef struct {
    float data[3];
    int type;
}Data;
typedef struct {
    char CityName[100];
}CityName;
/**
 * @description: 获取比赛数据
 * @param {Data*} MatchData 储存比赛数据的头节点
 */
void getData(Data * MatchData);
/**
 * @description: 获取城市名称
 * @param {CityName*} City 储存城市名称的头节点
 */
void getCityName(CityName* City);
/**
 * @description: 格式化数据
 * @param {Data*} MatchData 储存比赛数据的头节点 {int} order 比赛场次
 */
void initData(Data* MatchData,int order);
/**
 * @description: 管理格式化数据
 * @param {Data*} MatchData 储存比赛数据的头节点
 */
void init(Data* MatchData);
/**
 * @description: 获取两节点之间距离
 * @param {Data} srcNation 源点  {Data} targetNation 终点
 * @return {float} 两点间距离
 */
float getDist(float * srcArray,float * targetArray);
void  showData(Data* MatchData,CityName* City);
void kMeans(Data* MatchData,CityName* City);
int isOk(int* newArray,Data* MatchData);

int main() {
    Data MatchData[15];
    CityName City[15];
    getData(MatchData);
    init(MatchData);
    getCityName(City);
    for (int i = 0; i < 15; ++i) {
        printf(" %6s -->",City[i].CityName);
        for (int j = 0; j < 3; ++j) {
            printf(" %d: [%f]",j+1,MatchData[i].data[j]);
        }
        printf("\n");
    }
    kMeans(MatchData,City);
    return 0;
}
int isOk(int* newArray,Data* MatchData){
    int flag = 1;
    for (int i = 0; i < 15; ++i) {
            if(newArray[i] != MatchData[i].type){
                flag = 0;
            }
    }
    return flag;
}
void showData(Data* MatchData,CityName* City){
    for (int i = 0; i < 3; ++i) {
        printf("%c :[ ",i+65);
        for (int j = 0; j < 15; ++j) {
            if (MatchData[j].type == i + 1 ){
                printf("%s ",City[j].CityName);
            }
        }
        printf("]\n");
    }
}
void init(Data* MatchData){
    for (int i = 0; i < 3; ++i) {
        initData(MatchData,i);
    }
}
void kMeans(Data* MatchData,CityName* City){
    float boss[3][3];
    int historyList[15];
    int A = 1,B = 2,C = 3;
    int select[3] = {1,10,14};
    int count = 0; //迭代次数
    MatchData[select[0]].type = A;
    MatchData[select[1]].type = B;
    MatchData[select[2]].type = C;
    // 初始化源点数组
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            boss[i][j] = MatchData[select[i]].data[j];
        }
    }
    while (!isOk(historyList,MatchData)) {
        for (int i = 0; i < 15; ++i) {
            historyList[i] = MatchData[i].type;
        }
        for (int i = 0; i < 15; ++i) {
            float min = infinity;
            int flag;
            for (int j = 0; j < 3; ++j) {
                if( min > getDist(boss[j],MatchData[i].data) ){
                    min = getDist(boss[j],MatchData[i].data);
                    flag = j;
                }
            }
            printf("距离 %s 最近的城市为 %s 距离为 %f\n",City[i].CityName,City[flag].CityName,min);
            MatchData[i].type = flag+1;
        }
        showData(MatchData,City);
        for (int i = 0; i < 3; ++i) {
            float sum[3] = {0,0,0};
            float number = 0;
            for (int j = 0; j < 15; ++j) {
                if( MatchData[j].type == i + 1 ){
                    for (int k = 0; k < 3; ++k) {
                        sum[k] += MatchData[j].data[k];
                        number++;
                    }
                }
            }
            for (int j = 0; j < 3; ++j) {
                boss[i][j] = sum[j] / number;
            }
        }
    }
}
float getDist(float *  srcArray,float * targetArray){
    float sum = 0;
    for (int i = 0; i < 3; ++i) {
        sum+= pow( srcArray[i] - targetArray[i] , 2 );
    }
    return sqrt(sum);
}
void getData(Data * MatchData){
    FILE *fp = NULL;
    float first;
    float second;
    float third;
    int i = 0;
    if((fp=fopen("/Users/kying-star/Documents/文档/大学/大二下/移动通信/实验课4-大数据挖掘/lab_4/data.txt","r")) == NULL)
    {
        printf("文件data.txt打开失败,请检查路径\n");
        exit(-1);
    }
    while (fscanf(fp, "%f %f %f",&first,&second,&third) != EOF){
        MatchData[i].data[0] = first;
        MatchData[i].data[1] = second;
        MatchData[i].data[2] = third;
        MatchData[i].type = infinity;
        i++;
    }
    fclose(fp);
}
void getCityName(CityName* City){
    FILE *fp = NULL;
    char str[255];
    int i = 0;
    if((fp=fopen("/Users/kying-star/Documents/文档/大学/大二下/移动通信/实验课4-大数据挖掘/lab_4/nation.txt","r")) == NULL)
    {
        printf("文件data.txt打开失败,请检查路径\n");
        exit(-1);
    }
    while (fscanf(fp, "%s",str) != EOF){
        strcpy(City[i].CityName,str);
        i++;
    }
    fclose(fp);
}
void initData(Data* MatchData,int order){
    float min = infinity,max = 0;
    for (int i = 0; i < 15; ++i) {
        if( MatchData[i].data[order] < min){
            min = MatchData[i].data[order];
        }
        if( MatchData[i].data[order] > max ){
            max = MatchData[i].data[order];
        }
    }
    for (int i = 0; i < 15; ++i) {
        float temp = ( MatchData[i].data[order] - min)/( max - min );
        if( temp * 100 - (int)(temp * 100) >= 0.5 ){
            temp = ((int)(temp * 100) + 1.0) / 100;
        } else {
            temp = ((int)(temp * 100)) / 100.0;
        }
        MatchData[i].data[order] = temp;
    }
}
