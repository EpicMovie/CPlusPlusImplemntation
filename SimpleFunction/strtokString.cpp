#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> strSplit(string origin, string token){
    int cutAt;  // 자르는 위치
    int index = 0;  // 문자열 인덱스
    vector<string> result;
    
    while ( (cutAt = origin.find_first_of(token)) != origin.npos){
        if (cutAt > 0)
            result.push_back(origin.substr(0, cutAt));
        
        origin = origin.substr(cutAt + 1);
    }
    
    if (origin.length() > 0)
        result.push_back(origin.substr(0, cutAt));
    
    return result;
}

int main() { 
    string s; 
    int num = 1;  
    
    while(getline(cin, s)){
        vector<string> temp = strSplit(s, " ");
        cout << "#" << num++ << " : ";
        
        for(int i=0; i<temp.size(); i++)
            cout << temp[i] << " ";     cout << endl; 
    }
    
    return 0;
}
