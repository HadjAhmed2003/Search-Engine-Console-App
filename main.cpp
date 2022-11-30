#include<bits/stdc++.h>
using namespace std;
unordered_map<string,vector<string>> graph; 
unordered_map<string,vector<string>> incoming_links; 
// unordered_map<string,vector<string>> keywords; 
unordered_map<string,int> impressions; 
unordered_map<string,int> CTR; 
unordered_map<string,double> pagerank; 
unordered_map<string,vector<string>> search_table; 
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
                incoming_links[colname].push_back(start);
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
        // vector<string> adj;  
        while(std::getline(ss, colname, ',')){
            if(flag){
                start = colname;
                flag = false;
                
            }else{
                // adj.push_back(colname);
                search_table[colname].push_back(start);
            }
        }
        // keywords[start]=adj; 
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
void getCTR(){
    fstream fin;
    string colname, line;
    fin.open("CTR.csv", ios::in);
    while(std::getline(fin, line))
    {
        std::stringstream ss(line);
        string website, ctr; 
        getline(ss, website, ','); 
        getline(ss, ctr, ',');
        CTR[website] = stoi(ctr);
    }
}
void page_rank(){
    double size = graph.size();
    for(auto it=graph.begin(); it!=graph.end(); it++){
        pagerank[it->first]=1/size;
    }
    for(int j=0; j<2; j++){
        for(auto it=pagerank.begin(); it!=pagerank.end();it++){
            double temp_rank = 0; 
            for(int i=0; i<incoming_links[it->first].size(); i++){
                temp_rank+=(pagerank[incoming_links[it->first][i]]/graph[incoming_links[it->first][i]].size());
            }
            it->second = temp_rank; 
        }
    }
}
bool cmp_score(pair<string,double> const& p1, pair<string,double> const& p2){
    return p1.second>p2.second; 
}
vector<pair<string, double>> get_websites(string query){
    vector<pair<string, double>> final_result;
    vector<string> result=search_table[query]; 
    // for(auto it=keywords.begin(); it!= keywords.end(); it++){
    //     for(auto itr:it->second){
    //         if(itr == query){
    //             result.push_back({it->first, impressions[it->first]}); 
    //             break;
    //         }
    //     } 
    // }
    for(auto i:result){
        double score = 0.4*pagerank[i] + (((1-(0.1*impressions[i]/(1+0.1*impressions[i])))*pagerank[i])+ ((1-(0.1*impressions[i]/(1+0.1*impressions[i])))*CTR[i]))*0.6;
        final_result.push_back({i, score});
    }
    return final_result;
} 
void print_results(vector<pair<string, double>> result){
    sort(result.begin(), result.end(), cmp_score);
    int i = 1; 
    for(auto it=result.begin(); it!=result.end(); it++){
        cout << i <<"- " << it->first <<endl;
        impressions[it->first]++; 
        i++;
    }
    int clicked; 
    cout << "Enter the page you want to click on: ";
    cin >> clicked; 
    CTR[result[clicked-1].first]++;
}
void update_registry(){
    fstream fout;
    fout.open("CTR.csv", ios::out);
    for(auto it=CTR.begin(); it!=CTR.end(); it++){
        fout << it->first<<','<<it->second<<'\n';
    }
    fout.close();
    fout.open("impression.csv", ios::out);
    for(auto it=impressions.begin(); it!=impressions.end(); it++){
        fout << it->first<<','<<it->second<<'\n';
    }
    fout.close();
}
void process_query(string query){
    if(query[0]=='"'){
        string new_query=""; 
        for(int i=1; i<query.length()-1;i++){
            new_query+=query[i];
        }
        print_results(get_websites(new_query));
        return;
    }else if(query.find("OR")!=string::npos){
        string temp;
        stringstream s(query); 
        vector<pair<string, double>> result1; 
        vector<pair<string, double>> result2;
        getline(s, temp, ' ');
        result1 = get_websites(temp);
        getline(s,temp,' ');  
        getline(s,temp,' ');
        result2 = get_websites(temp);
        for(auto it: result1){
            if(find(result2.begin(), result2.end(), it)==result2.end()){
                result2.push_back({it.first, it.second}); 
            }
        }
        print_results(result2);
    }else if(query.find("AND")!=string::npos){
        string temp;
        stringstream s(query); 
        vector<pair<string, double>> result1; 
        vector<pair<string, double>> result2;
        vector<pair<string, double>> result;
        getline(s, temp, ' ');
        result1 = get_websites(temp);
        getline(s,temp,' ');  
        getline(s,temp,' ');
        result2 = get_websites(temp);
        for(auto it: result1){
            if(find(result2.begin(), result2.end(), it)!=result2.end()){
                result.push_back({it.first, it.second}); 
            }
        }
        print_results(result);
    }else{
        string temp;
        stringstream s(query); 
        vector<pair<string, double>> result1; 
        vector<pair<string, double>> result2;
        getline(s, temp, ' ');
        result1 = get_websites(temp);
        getline(s,temp,' ');
        result2 = get_websites(temp);
        for(auto it: result1){
            if(find(result2.begin(), result2.end(), it)==result2.end()){
                result2.push_back({it.first, it.second}); 
            }
        }
        print_results(result2);
    }
}
int main(){
    getImpressions();
    getGraph();
    getKeywords();
    getCTR();
    page_rank();
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
            cout << "Enter your search "; 
            cin.ignore();
            getline(cin,query); 
            process_query(query);
        }else{
            cout << "Enter a valid choice\n"; 
        }
    }
    update_registry();
    return 0; 
}