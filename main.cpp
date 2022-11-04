#include<bits/stdc++.h>
using namespace std;
unordered_map<string,vector<string>> graph; 
unordered_map<string,vector<string>> keywords; 
unordered_map<string,int> impressions; 
void getGraph(){
    fstream fin;
    string colname, line;
    fin.open("webGraph.csv", ios::in);
    while(std::getline(fin, line))
    {
        std::stringstream ss(line);
        bool flag = true;
        string start;
        vector<string> adj;  
        while(std::getline(ss, colname, ',')){
            if(flag){
                start = colname;
                flag = false;
                
            }else{
                adj.push_back(colname);
            }
        }
        graph[start]=adj; 
    }
    // for(auto it=graph.begin(); it!= graph.end(); it++){
    //     cout << "source " << it->first <<endl; 
    //     for(auto itr:it->second){
    //         cout << itr << " ";
    //     } 
    //     cout << endl;
    // }
} 
void getKeywords(){
    fstream fin;
    string colname, line;
    fin.open("keyword.csv", ios::in);
    while(std::getline(fin, line))
    {
        std::stringstream ss(line);
        bool flag = true;
        string start;
        vector<string> adj;  
        while(std::getline(ss, colname, ',')){
            if(flag){
                start = colname;
                flag = false;
                
            }else{
                adj.push_back(colname);
            }
        }
        keywords[start]=adj; 
    }
}
void getImpressions(){
    fstream fin;
    string colname, line;
    fin.open("impression.csv", ios::in);
    while(std::getline(fin, line))
    {
        std::stringstream ss(line);
        string website, impression; 
        getline(ss, website, ','); 
        getline(ss, impression, ',');
        impressions[website] = stoi(impression);
    }
}
bool cmp_impression(pair<string,int> const& p1, pair<string,int> const& p2){
    return p1.second>p2.second; 
}
vector<pair<string, int>> get_websites(string query){
    vector<pair<string, int>> result;
    for(auto it=keywords.begin(); it!= keywords.end(); it++){
        for(auto itr:it->second){
            if(itr == query){
                result.push_back({it->first, impressions[it->first]}); 
                break;
            }
        } 
    }
    return result;
} 
void print_results(vector<pair<string, int>> result){
    sort(result.begin(), result.end(), cmp_impression); 
    for(auto it=result.begin(); it!=result.end(); it++){
        cout << "- " << it->first << endl;
    }
}
int main(){
    getImpressions();
    getGraph();
    getKeywords();
    cout << "Welcome!\n"; 
    while(true){
        int choice; 
        cout << "What would you like to do?\n"; 
        cout << "1. New search\n"; 
        cout << "2. End\n"; 
        cout << "Type in your choice "; 
        cin >> choice; 
        if(choice == 2){
            break;
        }else if(choice == 1){
            string query;
            cout << "Enter your search "; cin >> query; 
            print_results(get_websites(query));
        }else{
            cout << "Enter a valid choice\n"; 
        }
    }
    return 0; 
}


