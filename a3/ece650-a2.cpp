//a2
// Compile with c++ ece650-a2cpp -std=c++11 -o ece650-a2

#include <stdio.h>
#include <iostream>


bool isnum(char c);

const int  maxv = 999;


bool isnum(char c){
    return (c >= '0' && c <= '9');
}


int main(int argc, char** argv) {
    int num = -1;
    int *e;
    int **edgs;
    
    while(!std::cin.eof()){
        char comma;
        int err = 0;
        static int err1 = 0;
        static int err2 = 0;
        std::cin.clear();
        std::cin.sync();
        std::cin.get(comma);
        
        switch(comma){
            case 'V' :{
                
                // read the amount of vertices
                if (err2==4){
                    std::cerr << "Error : repeative V command" << std::endl;
                    break;
                }
                
                std::cin >> num;
                
                if (num<0){
                    std::cerr << "Error : no vertex input" << std::endl;
                    break;
                }
                
                // create empty matrix
                
                edgs = new int *[num];
                for (int i=0;i<num;i++){
                    edgs[i] = new int[num];
                }
                
                for (int i=0;i<num;i++){
                    for (int j=0;j<num;j++){
                        edgs[i][j] = maxv;
                    }
                }
                
                err1 = 1;       //successful V command
                err2 = 4;       //first successful V comand
                break;
            }
                
                
                
            case 'E' : {
                
                std::string line;
                std::cin >> line;
                int k = line.size();
                e = new int[k];
                int result;
                int n = 0, m = 0;
                
                if (err1==3){
                    std::cerr << "Error : repeative E command" << std::endl;
                    break;
                }
                
                if (err1!=1){
                    std::cerr << "Error : no V command input" << std::endl;
                    break;
                }
                
                while (n<k){
                    result = 0;
                    
                    if (isnum(line[n])){
                        
                        if (line[n-1]=='-'){
                            err = 2;
                            break;
                        }
                        result = line[n] - '0';
                        
                        while (n<k && isnum(line[++n])){
                            result = result * 10 + line[n] - '0';
                        }
                        
                        if (result >= num || result < 0){
                            err = 1;
                            break;
                        }
                        
                        else
                            e[m++] = result;
                    }
                    ++n;
                }
                
                if (err==1){
                    std::cerr << "Error: vertex doesn't exist" << std::endl;
                    err = 0;
                    break;
                }
                
                if (err==2){
                    std::cerr << "Error : negative number" << std::endl;
                    err = 0;
                    break;
                }
                
                // memory edges
                if (m%2){
                    std::cerr << "Error : invalid edges" << std::endl;
                    break;
                }
                
                //std::std::cout << "m=" << m << std::endl;
                for (int j=0;j<m;j=j+2){
                    
                    if (e[j] == e[j+1])
                        continue;
                    edgs[e[j]][e[j+1]] = 1;
                    edgs[e[j+1]][e[j]] = 1;
                }
                err1 = 3;       //first successful E command
                err2 = 0;       //can input V again after successful E command
                break;
            }
                
            case 's' : {
                
                int src = -3;
                int dst = -3;
                
                std::cin >> src;
                std::cin >> dst;
                if (num<0){
                    std::cerr << "Error : the graph doesn't exist" << std::endl;
                    break;
                }
                int dist[num];
                int pre[num];
                int flag = 0;
                bool s[num];
                
                if (err1!=3){
                    std::cerr << "Error : no E command input" << std::endl;
                    break;
                }
                
                if (dst>=num || src>=num){
                    std::cerr << "Error: the vertex doesn't exist" << std::endl;
                    break;
                }
                
                if (src<0 || dst<0){
                    std::cerr << "Error : invalid vertex input" << std::endl;
                    break;
                }
                
                // find shortest path
                
                else if (src==dst){
                    std::cout << src << std::endl;
                    
                }
                
                else{
                    for (int i=0;i<num;i++){
                        dist[i] = edgs[src][i];
                        s[i] = 0;
                        if (dist[i]==maxv)
                            pre[i] = -1;
                        else
                            pre[i] = src;
                    }
                    s[src] = 1;
                    dist[src] = 0;
                    flag++;
                    
                    for (int i=1;i<num;i++){
                        int u = src;
                        for (int j=0;j<num;j++){
                            if (flag==num)
                                break;
                            else{
                                if ((!s[j]) && dist[j]<maxv){
                                    u = j;
                                    for (int t=0;t<num;t++){
                                        int newdist = dist[u] + edgs[u][t];
                                        if (newdist < dist[t]){
                                            dist[t] = newdist;
                                            pre[t] = u;
                                        }
                                    }
                                    s[u] = 1;
                                    flag++;
                                }
                            }
                        }
                    }
                    
                    if (dist[dst]==maxv)
                        std::cerr << "Error: no path between 2 vertices" << std::endl;
                    else{
                        int list[num];
                        int nu = 0;
                        int tmp = pre[dst];
                        while(tmp!=src){
                            list[nu++] = tmp;
                            tmp = pre[tmp];
                        }
                        
                        std::cout << src << '-';
                        for (int i=nu-1;i>=0;i--)
                            std::cout << list[i] << '-';
                        std::cout << dst << std::endl;
                    }
                }
                
                break;
            }
                
                // other command
            default :
                
                break;
                
        }
        
    }
    
    // delete matrix
    
    delete[] e;
    
    for (int i=0;i<num;i++)
        delete[] edgs[i];
    delete[] edgs;
    
    return 0;
}
