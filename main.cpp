#include <stdio.h>
#include <iostream>
#include <stack>
#include <string>

using namespace std;

union{
    char character;
    struct{
        char a1:1;char a2:1;char a3:1;char a4:1;char a5:1;char a6:1;char a7:1;char a8:1;
    } bit;
} counter;
int flag=1;

struct node{
    int value;
    char info;
    bool bit;
    node *r,*t1,*t2;
};


void hufftree(int* num, node* tree, int x, int t_bit);
int freq_count(int* num, int t_bit, string fileName);
void zip(int* num, node* tree, int t_bit, string fileName);
void photo_conv(FILE* output, int bi);
void unzip(node* tree, int x, FILE* input, string fileName, int stop);
char bit_conv(FILE* input);

int main(){
    while(1){
        
        int selection;
        cout << "What do you want to do? \nPress 1 for making zip file \n  2 for unzipping\n";
        cout << "3 for exit \nSelection- ";
        cin >> selection;
        
        if(selection!=1 && selection!=2){
            break;
        }
        
        string fileName;
        cout << "Enter name of the file: ";
        cin >> fileName;
        
        if(selection==1){
            int t_bit=256;
            
            int num[t_bit];
            if(freq_count(num, t_bit, fileName)==0);
            
            else {
                int x=(t_bit)*2-1;
                node tree[x];
                hufftree(num, tree, x, t_bit);
                zip(num, tree, t_bit, fileName);
            }
        }
        
        else{
            FILE *input;
            
            char fName[fileName.size()+1];
            for(int i=0; i<fileName.size(); i++){
                fName[i]=fileName[i];
            }
            fName[fileName.size()]='\0';
            
            if((input=fopen(fName, "rb"))==NULL){
                printf("File is not found!!\n");
            }
            
            else{
                int t_bit;
                fscanf(input, "%d ", &t_bit);
                
                int num[t_bit];
                for(int i=0; i<t_bit; i++){
                    fscanf(input, "%d ", &num[i]);
                }
                
                int x=(t_bit)*2-1;
                node tree[x];
                hufftree(num, tree, x, t_bit);
                unzip(tree, x, input, fileName, num[18]);
                fclose(input);
                cout << "\n# File Extracted.\n\n";
            }
        }
        getchar();
        cout << "\n\n";
    }
    
    return 0;
}

void hufftree(int* num, node* tree, int x, int t_bit){
    for(int i=0; i<x; i++){
        tree[i].value = -1;
        tree[i].info = (char)0;
        tree[i].bit = false;
        tree[i].r = NULL;
        tree[i].t1 = NULL;
        tree[i].t2 = NULL;
    }
    
    int position;
    
    for(position=0; position<t_bit; position++){
        tree[position].value = num[position];
        tree[position].info = (char)(position);
    }
    
    int lt = 0,rt = 0;
    while(position<x){
        for(int i=0; i<x; i++)
            if(tree[i].r==NULL && tree[i].value>=0){
                lt=i;
                break;
            }
        
        for(int i=0; i<x; i++){
            if(tree[i].r==NULL && tree[i].value>=0)
                if(tree[i].value < tree[lt].value) lt=i;
        }
        tree[lt].r = &tree[position];
        
        
        for(int i=0; i<x; i++)
            if(tree[i].r==NULL && tree[i].value>=0){
                rt=i;
                break;
            }
        
        for(int i=0; i<x; i++){
            if(tree[i].r==NULL && tree[i].value>=0)
                if(tree[i].value < tree[rt].value) rt=i;
        }
        tree[rt].r = &tree[position];
        
        
        tree[lt].bit = false;
        tree[rt].bit = true;
        
        tree[position].value = (tree[lt].value + tree[rt].value);
        tree[position].t1 = &tree[lt];
        tree[position].t2 = &tree[rt];
        
        position++;
    }
    return ;
}

int freq_count(int* num, int t_bit, string fileName){
    FILE *input;
    
    char fName[fileName.size()+1];
    for(int i=0; i<fileName.size(); i++)
        fName[i]=fileName[i];
    fName[fileName.size()]='\0';
    if((input=fopen(fName, "rb"))==NULL){
        printf("File is not found!!\n");
        return 0;
    }
    
    for(int i=0; i<t_bit; i++)
        num[i]=0;
    char ch;
    ch=fgetc(input);
    while(!feof(input)){
        if(ch>=0 && ch<t_bit) num[ch]++;
        ch=fgetc(input);
    }
    
    num[18]++;    //end character to add all the remaining entries
    fclose(input);
    
    return 1;
}

void zip(int* num, node* tree,int t_bit, string fileName){
    FILE *input;
    char fName[fileName.size()+1];
    for(int i=0; i<fileName.size(); i++)
        fName[i]=fileName[i];
    fName[fileName.size()]='\0';
    
    if((input=fopen(fName, "rb"))==NULL){
        printf("File is not found!!\n");
        return ;
    }
    
    FILE *output;
    char oFileName[fileName.size()+4+1];
    
    oFileName[0]='H';oFileName[1]='u';oFileName[2]='f';oFileName[3]='f';
    for(int i=4; i<fileName.size()+4; i++)
        oFileName[i]=fileName[i-4];
    oFileName[fileName.size()+4]='\0';
    
    output=fopen(oFileName, "wb");
    
    fprintf(output, "%d", t_bit);
    fprintf(output, "\n");
    for(int i=0; i<t_bit; i++)
        fprintf(output, "%d ", num[i]);
    fprintf(output, "\n");
    
    int flag=1;
    stack<bool> mystack;
    char ch;
    ch=fgetc(input);
    while(1){
        int i;
        for(i=0; i<t_bit; i++){
            if(tree[i].info==ch) break;
        }
        node* nod;
        nod=&tree[i];
        while(nod->r!=NULL){
            mystack.push(nod->bit);
            nod=nod->r;
        }
        
        while(!mystack.empty()){
            
            photo_conv(output, mystack.top());
            
            mystack.pop();
        }
        ch=fgetc(input);
        
        if(flag==0) break;
        
        else if(flag && feof(input)){
            flag = 0;
            ch=18;
        }
    }
    
    if(flag>1){
        fprintf(output, "%c", counter.character);
    }
    fclose(output);
    fclose(input);
    
    cout << "\n# Zip file is made.\n";
    
    return ;
}

void photo_conv(FILE* output, int bi){
    
    if(flag==1) counter.bit.a1=bi;
    else if(flag==2) counter.bit.a2=bi;
    else if(flag==3) counter.bit.a3=bi;
    else if(flag==4) counter.bit.a4=bi;
    else if(flag==5) counter.bit.a5=bi;
    else if(flag==6) counter.bit.a6=bi;
    else if(flag==7) counter.bit.a7=bi;
    else if(flag==8) counter.bit.a8=bi;
    
    flag++;
    
    if(flag>8){
        fprintf(output, "%c", counter.character);
        flag=1;
    }
    return;
}

void unzip(node* tree, int x, FILE *input, string fileName, int stop){
    int st=0;
    
    FILE *output;
    char oFileName[fileName.size()];
    
    oFileName[0]='R';oFileName[1]='e';oFileName[2]='s';oFileName[3]='s';
    for(int i=4; i<fileName.size(); i++)
        oFileName[i]=fileName[i];
    oFileName[fileName.size()]='\0';
    
    output=fopen(oFileName, "wb");
    flag=9;
    
    char ch;
    ch=bit_conv(input);
    while(!feof(input)){
        node* nod;
        nod=&tree[x-1];
        
        while(nod->t1!=NULL && nod->t2!=NULL){
            
            if(ch==0) nod = nod->t1;
            else nod = nod->t2;
            
            ch=bit_conv(input);
        }
        if(nod->info == 18){
            st++;
            
            if(st>=stop) break;
        }
        fprintf(output, "%c", nod->info);
    }
    
    fclose(output);
    
    return ;
}


char bit_conv(FILE* input){
    char ch;
    
    if(flag>8){
        counter.character = fgetc(input);
        flag=1;
    }
    
    if(flag==1) ch=counter.bit.a1;
    else if(flag==2) ch=counter.bit.a2;
    else if(flag==3) ch=counter.bit.a3;
    else if(flag==4) ch=counter.bit.a4;
    else if(flag==5) ch=counter.bit.a5;
    else if(flag==6) ch=counter.bit.a6;
    else if(flag==7) ch=counter.bit.a7;
    else ch=counter.bit.a8;
    
    flag++;
    
    return ch;
}
