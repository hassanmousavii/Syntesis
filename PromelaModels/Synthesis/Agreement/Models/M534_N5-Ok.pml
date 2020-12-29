#define N 5 // ring Size
#define dec(i) ((i+N-1)%N)
#define inc(i) ((i+1)%N)
#define r(i)  (x[i] != x[dec(i)] && y[i] != y[dec(i)] && z[i] != z[dec(i)])
#define q(i) (x[i] ==x[dec(i)] && y[i] == y[dec(i)] && z[i] == z[dec(i)])
#define R (r(0) && r(1) && r(2) && r(3) && r(4))
#define Q (q(0) && q(1) && q(2) && q(3) && q(4))
byte x[N]={1,3,1,3,2};
byte y[N]={1,0,2,0,2};
byte z[N]={3,0,1,2,1};
active [N] proctype p(){
short i = _pid;
 do
:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 0) && (z[dec(i)] == 0)) &&  (   (( z[i] !=0 )) ||   (( y[i] !=0 )) ||   (( x[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 0) && (z[dec(i)] == 0)) &&  (   (( x[i] !=1) &&  ( y[i] !=0) &&  ( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 0) && (z[dec(i)] == 0)) &&  (   (( x[i] !=2 && x[i] !=0 )) ||   (( z[i] !=0 )) ||   (( y[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 0) && (z[dec(i)] == 0)) &&  (   (( x[i] !=3 && x[i] !=0 )) ||   (( z[i] !=0 )) ||   (( y[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 0) && (z[dec(i)] == 0)) &&  (   (( x[i] !=4) &&  ( y[i] !=0) &&  ( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 1) && (z[dec(i)] == 0)) &&  (   (( y[i] ==2 )) ||   (( z[i] !=0 )) ||   (( x[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 1) && (z[dec(i)] == 0)) &&  (   (( x[i] !=1) &&  ( y[i] !=0 )) ||   (( x[i] !=0) &&  ( y[i] !=1 )) ||   (( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 1) && (z[dec(i)] == 0)) &&  (   (( x[i] !=2) &&  ( y[i] !=1) &&  ( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 1) && (z[dec(i)] == 0)) &&  (   (( x[i] !=3) &&  ( y[i] !=1) &&  ( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 1) && (z[dec(i)] == 0)) &&  (   (( x[i] !=4) &&  ( y[i] !=0 )) ||   (( x[i] !=0) &&  ( y[i] !=1 )) ||   (( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 2) && (z[dec(i)] == 0)) &&  (   (( y[i] ==1 )) ||   (( z[i] !=0 )) ||   (( x[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 2) && (z[dec(i)] == 0)) &&  (   (( x[i] !=1) &&  ( y[i] !=0 )) ||   (( x[i] !=0) &&  ( y[i] !=2 )) ||   (( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 2) && (z[dec(i)] == 0)) &&  (   (( x[i] !=2) &&  ( y[i] !=0 )) ||   (( x[i] !=0) &&  ( y[i] !=2 )) ||   (( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 2) && (z[dec(i)] == 0)) &&  (   (( x[i] !=3) &&  ( y[i] !=0 )) ||   (( x[i] !=0) &&  ( y[i] !=2 )) ||   (( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 2) && (z[dec(i)] == 0)) &&  (   (( x[i] !=4) &&  ( y[i] !=0 )) ||   (( x[i] !=0) &&  ( y[i] !=2 )) ||   (( z[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 0) && (z[dec(i)] == 1)) &&  (   (( x[i] !=0) &&  ( y[i] !=0) &&  ( z[i] !=1) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 0) && (z[dec(i)] == 1)) &&  (   (( x[i] !=1) &&  ( y[i] !=0) &&  ( z[i] !=1) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 0) && (z[dec(i)] == 1)) &&  (   (( x[i] !=2) &&  ( z[i] !=0 )) ||   (( x[i] !=0) &&  ( z[i] !=1 )) ||   (( y[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 0) && (z[dec(i)] == 1)) &&  (   (( x[i] !=3) &&  ( y[i] !=0) &&  ( z[i] !=1) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 0) && (z[dec(i)] == 1)) &&  (   (( x[i] !=4) &&  ( y[i] !=0) &&  ( z[i] !=1) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 1) && (z[dec(i)] == 1)) &&  (   (( x[i] !=0) &&  ( y[i] !=1) &&  ( z[i] !=1) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 1) && (z[dec(i)] == 1)) &&  (   (( x[i] !=1) &&  ( y[i] !=1) &&  ( z[i] !=1) )) -> x[i] = 4 ; y[i] = 1 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 1) && (z[dec(i)] == 1)) &&  (   (( x[i] !=2) &&  ( y[i] !=1) &&  ( z[i] !=1) )) -> x[i] = 2 ; y[i] = 2 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 1) && (z[dec(i)] == 1)) &&  (   (( x[i] !=3) &&  ( y[i] !=1) &&  ( z[i] !=1) )) -> x[i] = 3 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 1) && (z[dec(i)] == 1)) &&  (   (( x[i] !=4) &&  ( y[i] !=1) &&  ( z[i] !=1) )) -> x[i] = 1 ; y[i] = 1 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 2) && (z[dec(i)] == 1)) &&  (   (( y[i] !=2) &&  ( z[i] !=0 )) ||   (( y[i] !=0) &&  ( z[i] !=1 )) ||   (( x[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 2) && (z[dec(i)] == 1)) &&  (   (( x[i] ==4) &&  ( y[i] ==1) &&  ( z[i] ==3 || z[i] ==2 )) ||   (( x[i] ==4) &&  ( y[i] ==0) &&  ( z[i] !=1 )) ||   (( x[i] !=4 && x[i] !=1) &&  ( y[i] !=2) &&  ( z[i] !=1) )) -> x[i] = 4 ; y[i] = 1 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 2) && (z[dec(i)] == 1)) &&  (   (( x[i] !=2) &&  ( y[i] !=2) &&  ( z[i] !=1) )) -> x[i] = 1 ; y[i] = 2 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 2) && (z[dec(i)] == 1)) &&  (   (( x[i] !=3) &&  ( y[i] !=2) &&  ( z[i] !=1) )) -> x[i] = 3 ; y[i] = 0 ; z[i] = 3 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 2) && (z[dec(i)] == 1)) &&  (   (( x[i] ==3) &&  ( y[i] ==0) &&  ( z[i] ==3 || z[i] ==2 )) ||   (( x[i] ==3) &&  ( y[i] ==1) &&  ( z[i] !=1 )) ||   (( x[i] !=4 && x[i] !=3) &&  ( y[i] !=2) &&  ( z[i] !=1) )) -> x[i] = 3 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 0) && (z[dec(i)] == 2)) &&  (   (( z[i] ==3 || z[i] ==1 )) ||   (( y[i] !=0 )) ||   (( x[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 0) && (z[dec(i)] == 2)) &&  (   (( x[i] !=1) &&  ( y[i] !=0) &&  ( z[i] !=2) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 0) && (z[dec(i)] == 2)) &&  (   (( x[i] !=2) &&  ( y[i] !=0) &&  ( z[i] !=2) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 0) && (z[dec(i)] == 2)) &&  (   (( x[i] !=3) &&  ( y[i] !=0) &&  ( z[i] !=2) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 0) && (z[dec(i)] == 2)) &&  (   (( x[i] !=4) &&  ( y[i] !=0) &&  ( z[i] !=2) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 1) && (z[dec(i)] == 2)) &&  (   (( x[i] !=0) &&  ( y[i] !=1) &&  ( z[i] !=2) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 1) && (z[dec(i)] == 2)) &&  (   (( x[i] !=1) &&  ( y[i] !=1) &&  ( z[i] !=2) )) -> x[i] = 1 ; y[i] = 0 ; z[i] = 3 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 1) && (z[dec(i)] == 2)) &&  (   (( x[i] ==3) &&  ( y[i] ==0) &&  ( z[i] ==1 || z[i] ==0 )) ||   (( x[i] ==3) &&  ( y[i] ==2) &&  ( z[i] !=2 )) ||   (( x[i] !=3 && x[i] !=2) &&  ( y[i] !=1) &&  ( z[i] !=2) )) -> x[i] = 3 ; y[i] = 0 ; z[i] = 3 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 1) && (z[dec(i)] == 2)) &&  (   (( x[i] !=3) &&  ( y[i] !=1) &&  ( z[i] !=2) )) -> x[i] = 3 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 1) && (z[dec(i)] == 2)) &&  (   (( x[i] ==3) &&  ( y[i] ==2) &&  ( z[i] ==3 || z[i] ==1 )) ||   (( x[i] ==3) &&  ( y[i] ==0) &&  ( z[i] !=2 )) ||   (( x[i] !=4 && x[i] !=3) &&  ( y[i] !=1) &&  ( z[i] !=2) )) -> x[i] = 3 ; y[i] = 2 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 2) && (z[dec(i)] == 2)) &&  (   (( x[i] !=0) &&  ( y[i] !=2) &&  ( z[i] !=2) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 2) && (z[dec(i)] == 2)) &&  (   (( x[i] !=1) &&  ( y[i] !=2) &&  ( z[i] !=2) )) -> x[i] = 0 ; y[i] = 2 ; z[i] = 1 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 2) && (z[dec(i)] == 2)) &&  (   (( x[i] ==0) &&  ( y[i] ==1) &&  ( z[i] ==3 || z[i] ==1 )) ||   (( x[i] ==0) &&  ( y[i] ==0) &&  ( z[i] !=2 )) ||   (( x[i] !=2 && x[i] !=0) &&  ( y[i] !=2) &&  ( z[i] !=2) )) -> x[i] = 0 ; y[i] = 1 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 2) && (z[dec(i)] == 2)) &&  (   (( x[i] !=3) &&  ( y[i] !=2) &&  ( z[i] !=2) )) -> x[i] = 4 ; y[i] = 2 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 2) && (z[dec(i)] == 2)) &&  (   (( x[i] !=4) &&  ( y[i] !=2) &&  ( z[i] !=2) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 2 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 0) && (z[dec(i)] == 3)) &&  (   (( z[i] ==2 || z[i] ==1 )) ||   (( y[i] !=0 )) ||   (( x[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 0) && (z[dec(i)] == 3)) &&  (   (( x[i] !=1) &&  ( z[i] !=0 )) ||   (( x[i] !=0) &&  ( z[i] !=3 )) ||   (( y[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 0) && (z[dec(i)] == 3)) &&  (   (( x[i] !=2) &&  ( y[i] !=0) &&  ( z[i] !=3) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 0) && (z[dec(i)] == 3)) &&  (   (( x[i] !=3) &&  ( z[i] !=0 )) ||   (( x[i] !=0) &&  ( z[i] !=3 )) ||   (( y[i] !=0) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 0) && (z[dec(i)] == 3)) &&  (   (( x[i] !=4) &&  ( y[i] !=0) &&  ( z[i] !=3) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 1) && (z[dec(i)] == 3)) &&  (   (( x[i] !=0) &&  ( y[i] !=1) &&  ( z[i] !=3) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 1) && (z[dec(i)] == 3)) &&  (   (( x[i] !=1) &&  ( y[i] !=1) &&  ( z[i] !=3) )) -> x[i] = 1 ; y[i] = 2 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 1) && (z[dec(i)] == 3)) &&  (   (( x[i] !=2) &&  ( y[i] !=1) &&  ( z[i] !=3) )) -> x[i] = 2 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 1) && (z[dec(i)] == 3)) &&  (   (( x[i] !=3) &&  ( y[i] !=1) &&  ( z[i] !=3) )) -> x[i] = 3 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 1) && (z[dec(i)] == 3)) &&  (   (( x[i] ==2) &&  ( y[i] ==0) &&  ( z[i] ==2 || z[i] ==0 )) ||   (( x[i] ==2) &&  ( y[i] ==2) &&  ( z[i] !=3 )) ||   (( x[i] !=4 && x[i] !=2) &&  ( y[i] !=1) &&  ( z[i] !=3) )) -> x[i] = 2 ; y[i] = 0 ; z[i] = 1 ;}

:: atomic{( (x[dec(i)] == 0) && (y[dec(i)] == 2) && (z[dec(i)] == 3)) &&  (   (( x[i] !=0) &&  ( y[i] !=2) &&  ( z[i] !=3) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 1) && (y[dec(i)] == 2) && (z[dec(i)] == 3)) &&  (   (( x[i] !=1) &&  ( y[i] !=2) &&  ( z[i] !=3) )) -> x[i] = 3 ; y[i] = 0 ; z[i] = 3 ;}

:: atomic{( (x[dec(i)] == 2) && (y[dec(i)] == 2) && (z[dec(i)] == 3)) &&  (   (( x[i] !=2) &&  ( y[i] !=2) &&  ( z[i] !=3) )) -> x[i] = 0 ; y[i] = 0 ; z[i] = 3 ;}

:: atomic{( (x[dec(i)] == 3) && (y[dec(i)] == 2) && (z[dec(i)] == 3)) &&  (   (( x[i] !=3) &&  ( y[i] !=2) &&  ( z[i] !=3) )) -> x[i] = 3 ; y[i] = 2 ; z[i] = 0 ;}

:: atomic{( (x[dec(i)] == 4) && (y[dec(i)] == 2) && (z[dec(i)] == 3)) &&  (   (( x[i] !=4) &&  ( y[i] !=2) &&  ( z[i] !=3) )) -> x[i] = 0 ; y[i] = 2 ; z[i] = 0 ;}

od;
}
ltl p1 {[] (R -> <>Q)}