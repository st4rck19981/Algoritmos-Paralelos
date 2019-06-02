//g++ -g -Wall -fopenmp -o code codeMP.cpp

/*

    cout<<"---SIN PARALELISMO---\n";
    start = omp_get_wtime();
    countingPattern(texto,pattern,n,m);
    end = omp_get_wtime();
    cout<<"Tiempo sin paralelismo: "<<end-start<<endl;

//------CON PARALELISMO
    cout<<"---CON PARALELISMO---\n";
    thread_count=strtol(argv[1], NULL, 10);

    start = omp_get_wtime();
    countingPatternAP(texto,pattern,n,m);
    end = omp_get_wtime();
    cout<<"Tiempo con paralelismo: "<<end-start<<endl;

*/


#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <fstream>
#include <time.h>
#include <omp.h>
using namespace std;

void countingPatternAP(char* texto,char* pattern,long long int n,long int m);
void countingPattern(char* texto,char* pattern,long long int n,long int m);


int thread_count;

int main(int argc, char* argv[]){

    double start,end;
    double NP=0,P=0;

    int nTimes=100;

    long int tamN=60000000;
    long int tamM=1;

	long int n=0,m=0;
    char *pattern=new char[tamM];
    char *texto=new char[tamN];

	fstream fichero, fichero0;
    fichero.open("proteins.txt",ios::in);
    fichero0.open("pattern.txt",ios::in);
    string linea;

//------ENTRANDO TEXTO
    while(getline(fichero,linea)){
        for(unsigned int k=0;k<linea.size();k++){
        	if (n>=tamN)    break;
            texto[n]=linea[k];    n++;
        }
    }
//------CREANDO PATTERN
    while(getline(fichero0,linea)){
        for(unsigned int k=0;k<linea.size();k++){
            if (m>=tamM)    break;
            pattern[m]=linea[k];    m++;
        }
    }


    thread_count=strtol(argv[1], NULL, 10);

    for (int i=0;i<nTimes;i++){
        start = omp_get_wtime();
        countingPattern(texto,pattern,n,m);
        end = omp_get_wtime();
        NP+=(end-start);

        start = omp_get_wtime();
        countingPatternAP(texto,pattern,n,m);
        end = omp_get_wtime();
        P+=(end-start);

    }

    cout<<"Tiempo sin paralelismo: "<<NP/nTimes<<endl;
    cout<<"Tiempo con paralelismo: "<<P/nTimes<<endl;



//------ BORRAR MEMORIA
    delete[] texto;
    delete[] pattern;
    fichero.close();
    fichero0.close();
    return 0;
}



//------CON PARALELISMO
void countingPatternAP(char* texto,char* pattern,long long int n,long int m){
    long int countPattern=0;
    long long int i;
    # pragma omp parallel for num_threads(thread_count) \
        reduction(+: countPattern)    private(i)
    for(i=0;i<(n-m+1);i++){
        long int t0=0;
        for(long int j=0;j<m;j++){
            if (texto[i+j]==pattern[j])    t0++;
            else    break;
        }
        if (t0==m){    
            countPattern+=1;
        }
    }
    //if (countPattern==0)    cout<<"Sin coincidencias.\n";
    //else    cout<<"countPattern="<<countPattern<<endl;
}


//-----SIN PARALELISMO
void countingPattern(char* texto,char* pattern,long long int n,long int m){
    long int countPattern=0;
    for(long long int i=0;i<(n-m+1);i++){
        long int t0=0;
        for(long int j=0;j<m;j++){
            if (texto[i+j]==pattern[j])    t0++;
            else    break;
        }
        if (t0==m){    
            countPattern+=1;
        }
    }
    //if (countPattern==0)    cout<<"Sin coincidencias.\n";
    //else    cout<<"countPattern="<<countPattern<<endl;

}