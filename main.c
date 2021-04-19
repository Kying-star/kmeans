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
    char NationName[100];
}NationName;
/**
 * @description: 获取比赛数据
 * @param {Data*} MatchData 储存比赛数据的头节点
 */
void getData(Data * MatchData);
/**
 * @description: 获取城市名称
 * @param {NationName*} Nation 储存城市名称的头节点
 */
void getNationName(NationName* Nation);
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
void  showData(Data* MatchData,NationName* Nation);
/**
 * @description: kmeans算法
 * @param {Data*} MatchData 储存比赛数据的头节点 {NationName*} Nation 储存城市名称的头节点
 */
void kMeans(Data* MatchData,NationName* Nation);
/**
 * @description: 判断数组类型是否被刷新
 * @param {int*} newArray 存储更新的类型数组 {Data*} MatchData 储存比赛数据的头节点
 */
int isOk(int* newArray,Data* MatchData);
/**
 * @description: 保留小数点后几位
 * @param {float} temp 原数 {float} pos 保留位数
 * @return {float} 保留小数后的数
 */
float RetainDecimalPoints(float temp,float pos);
/**
 * @description: 显示比赛数据
 * @param {Data*} MatchData 储存比赛数据的头节点 {NationName*} Nation 储存城市名称的头节点
 */
void showMatchData(NationName * Nation,Data * MatchData);
int main() {
    Data MatchData[15];
    NationName Nation[15];
    getData(MatchData);
    init(MatchData);
    getNationName(Nation);
    showMatchData(Nation,MatchData);
    kMeans(MatchData,Nation);
    return 0;
}
void showMatchData(NationName * Nation,Data * MatchData){
    printf("----------------------初始化城市数据集----------------------\n");
    for (int i = 0; i < 15; ++i) {
        printf(" %6s -->",Nation[i].NationName);
        for (int j = 0; j < 3; ++j) {
            printf(" %d: [%.2f]",j+1,MatchData[i].data[j]);
        }
        printf("\n");
    }
    printf("----------------------               ----------------------\n");
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
void showData(Data* MatchData,NationName* Nation){
    for (int i = 0; i < 3; ++i) {
        printf("%c :[ ",i+65);
        for (int j = 0; j < 15; ++j) {
            if (MatchData[j].type == i + 1 ){
                printf("%s ",Nation[j].NationName);
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
void kMeans(Data* MatchData,NationName* Nation){
    float boss[3][3];
    int historyList[15];
    int A = 1,B = 2,C = 3;
    int select[3];
    for (int i = 0; i < 3; ++i) {
        printf("输入第%d个聚类中心",i+1);
        scanf("%d",&select[i]);
    }
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
        count++;
        printf("------------------------------------------------------------------------------------------------------------------\n");
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
            printf("距离 %s 最近的城市为 %s 距离为 %.4f\n",Nation[i].NationName,Nation[flag].NationName,min);
            MatchData[i].type = flag+1;
        }
        showData(MatchData,Nation);
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
        printf("重新选择的聚类中心为:\n");
        for (int i = 0; i < 3; ++i) {
            printf("[%c]:(%.4f , %.4f , %.4f)\n",i+65,boss[i][0],boss[i][1],boss[i][2]);
        }
    }
    printf("迭代次数为%d",count);
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
void getNationName(NationName* Nation){
    FILE *fp = NULL;
    char str[255];
    int i = 0;
    if((fp=fopen("/Users/kying-star/Documents/文档/大学/大二下/移动通信/实验课4-大数据挖掘/lab_4/nation.txt","r")) == NULL)
    {
        printf("文件data.txt打开失败,请检查路径\n");
        exit(-1);
    }
    while (fscanf(fp, "%s",str) != EOF){
        strcpy(Nation[i].NationName,str);
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
        MatchData[i].data[order] = RetainDecimalPoints(temp,100);
    }
}
float RetainDecimalPoints(float temp,float pos){
    if( temp * pos - (int)(temp * pos) >= 0.5 ){
        temp = ((int)(temp * pos) + 1.0) / 100;
    } else {
        temp = ((int)(temp * pos)) / pos;
    }
    return temp;
}

