#include<iostream>
#include<cmath>
#include "Random64.h"

const int L=8;
const int L2 = L*L;

class SpinSystem{
private:
  int s[L][L], E, M; //La caja como un arreglo, la energ�a y la magnetizaci�n.
public:
  void InicieTodosAbajo(void);
  void UnPasoDeMetropolis(double Beta, Crandom & Ran64);//Beta en lugar de beta porque las variables reservadas de c++ empiezan con min�scula.
  double GetE(void){return E;};
  double GetM(void){return std::fabs(M);};
};

void SpinSystem::InicieTodosAbajo(void){
  for(int i=0; i<L;i++)
    for(int j=0; j<L;j++)
      s[i][j]=-1;
  M=-L2; E = -2*L2;
}

void SpinSystem::UnPasoDeMetropolis(double Beta, Crandom & Ran64){ // Miro la estructura de metr�polis y dependiendo del cambio de energ�a volteo.
  int n,i,j; double dE;
  //Escojo un spin al azar;
  n = (int) 16*Ran64.r();  i = n%L; j = n/L;
  //s[(n%L][n/L]; las columnas son lo que sobra y las filas es la divisi�n.
  //Calculo el dE que se producir�a se lo volteo.
  //La energ�a entre spines es menos su producto punto e_12 = -s1.s2; si volteo el spin, el cambio en energ�a es 2e
  dE = 2*s[i][j]*(s[(i+1)%L][j]+s[(i+L-1)%L][j]+s[i][(j+1)%L]+s[i][(j+L-1)%L]);//Pues tengo 4 vecinos. Condiciones peri�dicas.
  if(dE<=0){
    //lo volteo;
    s[i][j]*=-1;
    E+=dE;
    M+=2*s[i][j]; //La magnetizaci�n en ese punto pasa de s a -s, luego despu�s de voltearlo dM = -2s_antes = 2s_despu�s.
  }  else if (Ran64.r()<std::exp(-Beta*dE)){
    //lo volteo;
    s[i][j]*=-1;
    E+=dE;
    M+=2*s[i][j];
  }
}


const int teq = (int)(600*pow(L/8.0,2.125));//El triple del tiempo de equilibrio, para que equilibre bien.
const int tcorr = (int)(50*pow(L/8.0,2.125));//El tiempo que pasa para que las muestras esten descorrelacionadas, i.e. que no se parezcan (pues una se construye a partir de las otras.)
const int Nmuestras = 10000;


int main(void){
  SpinSystem Ising;
  Crandom Ran64(1902); //Una semilla diferente a 0
  double kT, Beta;
  int mcs, mcss; //MonteCarlo step and Monte Carlo Step per Site.
  int teq; //Tiempo de equilibrio, realmente no es un tiempo sino un n�mero de pasos para que la distribuci�n llegue a equilibrio.
  double E,M, sumM, sumM2, sumM4, sumE, sumE2; //Acumuladores, aqu� van los valores de energ�a y magnetizaci�n.
  
  //Para cada temperatura
  kT = 2.2;
  Beta = 1.0/kT;
  //Inicio el sistema
  Ising.InicieTodosAbajo();
  //Dejo que el sistema llegue a equilibrio
  //Hago pasos de montecarlo por sitio hasta que el sistema no cambie
  for(mcss = 0; mcss<teq; mcss++)
    for(mcs=0; mcs<L2;mcs++)
      Ising.UnPasoDeMetropolis(Beta, Ran64);
  //Tomar N muestras
  //Inicio los acumuladores en cero
  sumM=sumM2=sumM4=sumE=sumE2 = 0.0;
  for(int cmuestras=0;cmuestras<Nmuestras;cmuestras++){
  //Tomo 1 Muestra
    E = Ising.GetE();
    M = Ising.GetM();
    sumM+=M; sumM2 +=M*M; sumM4+=M*M*M*M; sumE+=E; sumE2+=E*E;

  //Avanzar hasta la siguiente muestra, uso el tiempo de correlaci�n
    for(mcss = 0; mcss<tcorr; mcss++)
      for(mcs=0; mcs<L2;mcs++)
	Ising.UnPasoDeMetropolis(Beta, Ran64);
  }
  //Imprimir los resultados
  std::cout<<"E="<<Ising.GetE()<<" , M="<<Ising.GetM()<<std::endl;
  
  return 0;
}
