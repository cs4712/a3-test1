//generator random numbers

#include <iostream>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <time.h>


bool zero_line(int x1,int y1,int x2,int y2){
    
    if (x1 == x2 && y1 == y2)
        return true;    //  empty segment
    else
        return false;
    
}


bool get_parallel(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    
    int xden = 1;
    xden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    
    if (xden == 0)
        return true;
    else
        return false;
}


bool Intersection(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4){
    //    static int mu = 0;
    //    mu++;
    //    std::cout << "mu = " << mu << std::endl;
    double xnum, xden, ynum, yden;
    xnum = (x1*y2-y1*x2)*(x3-x4) - (x1-x2)*(x3*y4-y3*x4);
    xden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    ynum = (x1*y2 - y1*x2)*(y3-y4) - (y1-y2)*(x3*y4-y3*x4);
    yden = (x1-x2)*(y3-y4) - (y1-y2)*(x3-x4);
    
    
    if (xden == 0 && xnum == 0 && ynum == 0){     // overlap
        return true;
    }
    
    else if (xden == 0 && (xnum != 0 || ynum != 0)){   // parallel but no intersection
        return false;
    }
    
    else{
        double xcoor, ycoor;
        xcoor = (double) xnum / (double) xden;
        ycoor = (double) ynum / (double) yden;
        //std::cout << "xcoor=" << xcoor << "ycoor=" << ycoor << std::endl;
        
        
        if ((xcoor <= (double)std::max(x1,x2)) && (xcoor >= (double)std::min(x1,x2)) && (xcoor <= (double)std::max(x3,x4)) && (xcoor >= (double)std::min(x3,x4))){
            // maybe 1 intersection
        }
        else{                   // no intersection
            return false;
        }
        
        if ((ycoor <= (double)std::max(y1,y2)) && (ycoor >= (double)std::min(y1,y2)) && (ycoor <= (double)std::max(y3,y4)) && (ycoor >= (double)std::min(y3,y4))){
            //maybe 1 intersection
        }
        else{
            return false;       // no intersection
        }
        
        return true;
    }
    
    return true;
}



int main(int argc, char **argv){
    
    //getopt
    int sn = 10;
    int nn = 5;
    int ln = 5;
    int cn = 20;
    std::string svalue, nvalue, lvalue, cvalue;
    int a;
    
    //////////////// expected options are '-s value', '-n value', '-l value' and '-c value' //////////////
    
    while ((a = getopt(argc, argv, "s:n:l:c:")) != -1)
        switch (a)
    {
        case 's':
            svalue = optarg;
            sn = atoi(svalue.c_str());      // sn : street number
            if (sn < 2){
                std::cerr << "Error: invalid number of streets" << std::endl;
                return 0;
            }
            break;
        case 'n':
            nvalue = optarg;
            nn = atoi(nvalue.c_str());      // nn : segment number
            if (nn < 1){
                std::cerr << "Error: invalid number of line-segments" << std::endl;
                return 0;
            }
            break;
        case 'l':
            lvalue = optarg;
            ln = atoi(lvalue.c_str());      // ln : waiting time
            if (ln < 5){
                std::cerr << "Error: invalid waiting time" << std::endl;
                return 0;
            }
            break;
        case 'c':
            cvalue = optarg;
            cn = atoi(cvalue.c_str());      // cn : coordinate range
            if (cn < 1){
                std::cerr << "Error: invalid range of coodinates" << std::endl;
                return 0;
            }
            break;
        default:
            std::cerr << "Error: invalid command" << std::endl;
            return 0;
    }
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    
    /////////////////////////////////// generate streets //////////////////////////////////////////////
    // open /dev/urandom
    std::ifstream urandom("/dev/urandom");
    
    // check that it did not fail
    if (urandom.fail())
    {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    
    while (!std::cin.eof()) {
        
        int st = -1;        // street number
        int sg = -1;        // segment number
        int wait_t = -1;     // waiting time
        
        int err_num = 0;
        
        ///////////////waiting time//////////////////////
        
        while((wait_t>ln || wait_t<5)){
            
            unsigned int ran0 = 1;
            urandom.read((char*)&ran0, sizeof(unsigned int));
            
            wait_t = ran0 % (ln+1);
        }
        
        /////////////////////////////////////////////////
        
        ///////////////numbers of streets////////////////
        
        while((st>sn || st<2)){
            
            unsigned int ran1 = 1;
            urandom.read((char*)&ran1, sizeof(unsigned int));
            
            st = ran1 % (sn+1);
        }

        /////////////////////////////////////////////////
        
        ///////////////numbers of segments////////////////

        while((sg>nn || sg<1)){

            unsigned int ran2 = 1;
            urandom.read((char*)&ran2, sizeof(unsigned int));
            
            sg = ran2 % (nn+1);
        }

        ////////////////////////////////////////////////////
        
        //////////////////////////////////////// coordinate ///////////////////////////////////////
        
        // create (st * (sg+1) ) array
        std::vector <std::vector <int> > cr(st);
        for (int i = 0; i < cr.size(); i++)
            cr[i].resize((sg+1)*2);
        
        
        int seg_num = 0;
        seg_num = st * sg;
        
        for (int i=0;i<st;i++){
            for (int j=0;j<sg+1;j++){
                
                bool success = false;
                
                while(!success && err_num < 25){    //  try at most 25 attenpts
                    
                    for (int k=0;k<2;k++){
                        int ran3 = 1;
                        urandom.read((char*)&ran3, sizeof(int));
                        cr[i][2*j+k] = ran3 % (cn+1);
                       
                    }
                    
                    // no zero-length segment
                    if (j>0){
                        if (zero_line(cr[i][2*(j-1)], cr[i][2*j-1], cr[i][2*j], cr[i][2*j+1]) == 1){
                            err_num++;
                            continue;
                        }
                    }
                    
                    // adjacent segment shouldn't be parallel
                    if (j>1){
                        if (get_parallel(cr[i][2*(j-2)], cr[i][2*j-3], cr[i][2*(j-1)], cr[i][2*j-1], cr[i][2*(j-1)], cr[i][2*j-1], cr[i][2*j], cr[i][2*j+1]) == 1){
                            err_num++;
                            continue;
                        }
                    }
                    
                    // street should not be cyclic
                    int err_flag1 = 0;
                    if (j>2){
                        for (int t=1;t<j-1;t++){
                            if(Intersection(cr[i][2*(t-1)], cr[i][2*t-1], cr[i][2*t], cr[i][2*t+1], cr[i][2*(j-1)], cr[i][2*j-1], cr[i][2*j], cr[i][2*j+1]) == 1){
                                err_num++;
                                err_flag1 = 1;
                                break;
                            }
                        }
                        if (err_flag1 == 1)
                            continue;
                    }
                    
                    // it should be no overlap segment in different streets
                    int err_flag2 = 0;
                    if (j>0 && i>0){
                        for (int x=0; x<i; x++){
                            for (int y=0; y<sg+1; y++){
                                if ((cr[x][2*y] == cr[i][2*j] && cr[x][2*y+1] == cr[i][2*j+1]) || (cr[x][2*y] == cr[i][2*j+1] && cr[x][2*y+1] == cr[i][2*j])){
                                    err_num++;
                                    err_flag2 = 1;
                                    break;
                                }
                            }
                            if (err_flag2 == 1)
                                break;
                        }
                        if (err_flag2 == 1)
                            continue;
                    }
                    
                    success = true;
                    
                }

                if (err_num >= 25){
                    std::cerr << "Error : failed to generate valid input for 25 simultaneous attempts" << std::endl;
                    urandom.close();
                    return 0;
                }
            }
        }
        
        ////////////////////////////////////////////////////////////////////////////////////////////////////////

        
        std::vector <std::vector <std::string> > street(st);
        for (int i = 0; i < st; i++)
            cr[i].resize(200);
        
        std::string street_name[26] = {"a street", "b street","c street","d street","e street","f street","g street","h street","i street","j street","k street","l street","m street","n street","o street","p street","q street","r street","s street","t street","u street","v street","w street","x street","y street","z street"};
        
        for (int i=0;i<st;i++){
            
            street[i].push_back("a ");
            street[i].push_back("\"");
            street[i].push_back(street_name[i]);
            street[i].push_back("\"");
            
            for(int j=0;j<sg+1;j++){
                
                street[i].push_back(" ");
                street[i].push_back("(");
                street[i].push_back(std::to_string(cr[i][2*j]));
                street[i].push_back(",");
                street[i].push_back(std::to_string(cr[i][2*j+1]));
                street[i].push_back(")");
            }
            
        }
        
        for (int i=0;i<st;i++){
            
            std::string line = "";
            for (auto&& j:street[i])
                line = line + j;
            line = line + "\n";
            write(STDOUT_FILENO, line.c_str(), line.size());
            
        }
        
        write(STDOUT_FILENO, "g\n", 2);
        
        sleep(wait_t);
        
        for (int i=0;i<st;i++){
            std::string line1 = "";
            line1 = line1 + "r " "\"";
            line1 = line1 + street_name[i];
            line1 = line1 + "\"";
            line1 = line1 + "\n";
            write(STDOUT_FILENO, line1.c_str(), line1.size());
            
        }
        
    }
    
    // close random stream
    urandom.close();
    return 0;
}
