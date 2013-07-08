#ifndef LOADMATRIX_H
#define LOADMATRIX_H
#include<iostream>
#include<fstream>
#include<string>
#include"qSort.h"
using namespace std;

bool File_Has_Decimal(string fileName){
  const char* chtempInput=fileName.c_str();
  ifstream fin(chtempInput);
  string s;
  getline(fin,s);
  while (s!="")
  {
      for (int i=0;i<s.length();i++)
          if (s[i]=='.')
             {
              return true;
              fin.close();
             }
      getline(fin,s);
  }
  fin.close();
  return false;
}

void generate01matrixbycutoffval(string fileName)
{
  const char* chtempInput=fileName.c_str();
  ifstream fin(chtempInput);
  ofstream fout("temp01matrix");
  int i,j,temp,position,col=0;
  double x1,x2,cutoffvalue,minWeight,Weight1,Weight2,sumline,sumlineforward,line[maxMatrixColumn],linebackup[maxMatrixColumn];
  string s;
  getline(fin,s);
  fout<<s<<endl;
  for (i=0;i<s.length();i++)
      if ((s[i]=='0') || (s[i]=='1'))
         col++;
  while (fin>>line[0])
        {
         sumline=linebackup[0]=line[0];
         for (i=1;i<col;i++)
             {
              fin>>line[i];
              linebackup[i]=line[i];
              sumline+=line[i];
             }
         qSort(line,0,col-1);
         x2=(sumline-line[0])/(col-1);
         minWeight=0;position=0;
         for (i=1;i<col;i++)
             minWeight+=(line[i]-x2)*(line[i]-x2);
         minWeight=minWeight*(col-1);
         sumlineforward=line[0];
         for (i=1;i<col-1;i++)
             {
              sumlineforward+=line[i];
              x1=sumlineforward/(i+1);
              x2=(sumline-sumlineforward)/(col-i-1);
              Weight1=Weight2=0;
              for (j=0;j<=i;j++)
                  Weight1+=(line[j]-x1)*(line[j]-x1);
              for (j=i+1;j<col;j++)
                  Weight2+=(line[j]-x2)*(line[j]-x2);
              if ((Weight1*(i+1)+Weight2*(col-i-1))<minWeight)
                 {
                  minWeight=Weight1*(i+1)+Weight2*(col-i-1);
                  position=i;
                 }
             }
         cutoffvalue=(line[position]+line[position+1])/2;
         if (linebackup[0]>=cutoffvalue)
            fout<<1;
            else fout<<0;
         for (i=1;i<col;i++)
             if (linebackup[i]>=cutoffvalue)
                fout<<" "<<1;
                else fout<<" "<<0;
         fout<<endl;
        }
  fin.close();
  fout.close();
}


void getMatrixInfo(string inputFileName,short matrix[][maxMatrixColumn],short class_type[],int& row,int& column,double& p0,double& p1)
{
 const char *chtemp=inputFileName.c_str();
 ifstream fin(chtemp);
 p0=0;column=0;
 string s;
 getline(fin,s);
 for (int i=0;i<s.length();i++)
     if (s[i]=='0')
        {
         class_type[column++]=0;
         p0=p0+1;
        }
        else if (s[i]=='1')
                class_type[column++]=1;
 p0=p0/column;p1=1-p0;row=0;
 while (fin>>matrix[row][0])
       {
        for (int i=1;i<column;i++)
            fin>>matrix[row][i];
        row++;
       }
 fin.close();
}

void Load_TainVali_Matrix(string fileName,short matrix[][maxMatrixColumn],short classType[],int& row,int& column,double& p0,double& p1)
{
    if (File_Has_Decimal(fileName))
       {
        generate01matrixbycutoffval(fileName);//LoadMatrix.h
        getMatrixInfo("temp01matrix",matrix,classType,row,column,p0,p1);//LoadMatrix.h
        system("del temp01matrix");
       }
        else
        {
         getMatrixInfo(fileName,matrix,classType,row,column,p0,p1);
        }
}




#endif // LOADMATRIX_H
