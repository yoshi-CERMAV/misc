
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

FILE *gp;
using namespace std;
class coord
{
public:
    coord(float x, float y, float z){
        c[0] = x;c[1] = y; c[2] = z;
    }
    coord(coord &a, coord &b){
        c[0] = b[0] - a[0];
        c[1] = b[1] - a[1];
        c[2] = b[2] - a[2];
    }
    void normalize(){
        float x = sqrt(c[0]*c[0] + c[1] *c[1] + c[2]*c[2]);
        c[0] /=x;
        c[1] /=x;
        c[2] /=x;
    }
    void dump(){
        cout << c[0] <<" "<< c[1]<<" "<<c[2]<<endl;
    }
    inline float & operator[](size_t i){return c[i];}
    float c[3];
};

vector<coord> xgo, cell;
vector<float> elapse;


void diff(float *a, float *b, float *c)
{
   c[0] = b[0] - a[0];
   c[1] = b[1] - a[1];
   c[2] = b[2] - a[2];
}

void read(const char filename[])
{
    xgo.clear();
    cell.clear();
    char buffer[256];
    ifstream fi(filename);
    while(fi.getline(buffer, 256)){
        istringstream istr(buffer);
        float t, x, y, z, t1, x1, y1, z1;
        istr >> t >> x>> y >> z>>t1>>x1 >> y1 >> z1;
        xgo.push_back(coord(x, y, z));
        cell.push_back(coord(x1, y1, z1));
    }
}

void movement(vector<coord> &in, vector<coord> &mv, int step)
{
    mv.clear();
    cout << "cleared"<<endl;
    for(int i = 0; i < in.size()-step; i+=step){
        mv.push_back(coord(in[i], in[i+step]));
  //      cout <<i<< " "<<(*(mv.rbegin()))[0]<<endl;
    }
    cout <<"calculated"<<endl;
}
void normalize(vector<coord> &a){
    vector<coord>::iterator itr;
    for(itr = a.begin(); itr!=a.end(); itr++) itr->normalize();
}
void dump(vector<coord> &a)
{
    cout << a.size()<<endl;
    vector<coord>::iterator itr;
    for(itr = a.begin(); itr!=a.end(); itr++) itr->dump();
}
int main(int argc, char *argv[])
{
    gp = popen("/usr/local/bin/gnuplot --persist", "w");
    read(argv[1]);
    fputs("plot x\n", gp);
    fflush(gp);
    vector<coord> mv;
    int step;
//    dump(xgo);
    while(cin >> step){
        movement(xgo, mv, step);
        normalize(mv);
        fputs("splot \"-\" binary format =\"%float%float%float\" record = ",gp);
        fprintf(gp, "%d u 1:2:3 \n", mv.size());
        fwrite(&(mv[0]), mv.size(), 12, gp);
        
        fflush(gp);
    }
}
