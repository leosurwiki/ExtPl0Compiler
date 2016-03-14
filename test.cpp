#include <iostream>
#include <fstream>
#include <string>
using namespace std;
int main()
{
    char str[100],c;
    ifstream fin("input.txt");
    cout<<fin.getline(str,100)<<endl;
    cout<<fin.getline(str,100)<<endl;
    cout<<fin.getline(str,100)<<endl;
    cout<<fin.getline(str,100)<<endl;
}
