#include<iostream>
#include<cstring>
#include<windows.h>
#include<iomanip>
using namespace std;

int main()
{
    char **text;
    int height, width;
    cout<<"Height: ";cin>>height;
    text = new char*[height];
    cout<<"Width: ";cin>>width;
    for(int i=0;i<height;i++) text[i] = new char[width+1];
    system("cls");
    cin.get();
    for(int i=0;i<height;i++)
    {
        cin.getline(text[i], width);
    }
    system("cls");
    for(int i=0;i<height;i++)
    {
        for(int j=0;j<width - strlen(text[i]);j++)cout<<' ';
        for(int j=strlen(text[i])-1;j>=0;j--)
        {
            if(text[i][j]==')')cout<<'(';
            else if(text[i][j]=='(')cout<<')';
            else if(text[i][j]=='/')cout<<'\\';
            else if(text[i][j]=='\\')cout<<'/';
            else if(text[i][j]=='`')cout<<'\'';
            else if(text[i][j]=='\'')cout<<'`';
            else if(text[i][j]=='<')cout<<'>';
            else if(text[i][j]=='>')cout<<'<';
            //else if(text[i][j]=='}')cout<<'}';
            //else if(text[i][j]=='{')cout<<'{';
            else cout<<text[i][j];
        }
        cin.clear();
        cout<<endl;
    }
    return 0;
}
