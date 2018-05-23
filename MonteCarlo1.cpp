#include<iostream>
#include<cmath>
#include "Random64.h"

const int L=8;
const int L2 = L*L;

class SpinSystem{
private:
  int s[L][L], E, M; //La caja como un arreglo, la energía y 
public:
  void InicieTodosAbajo(void);
  double GetE(void){return E;};
  double GetM(void){return std::fabs(M);};
};

void SpinSystem::InicieTodosAbajo(void){
  for(int i=0; i<L;i++)
    for(int j=0; j<L;j++)
      s[i][j]=-1;
  M=-L2; E = -2*L2;
}

int main(void){
  SpinSystem Ising;
  int t;

  Ising.InicieTodosAbajo();
  std::cout<<"E="<<Ising.GetE()<<" , M="<<Ising.GetM()<<std::endl;
  
  return 0;
}
