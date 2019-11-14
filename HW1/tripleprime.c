#include <stdio.h>
#include <stdlib.h>
#include <math.h>



typedef struct _seg {
    int bits[256];
    struct _seg *next;
}   seg;

typedef struct {
    seg *segpt;
    int intnum;
    int bitnum;
}   coordinate;

// Created a struct to store the previous seg and neccessary info for the which seg function
typedef struct {
    seg * lastSeg;
    int prevJ;
    int index;
} lastPointer; 


seg * head;
lastPointer * lastJ;

void setBits(){
    seg * temp;

    temp = head;
    //goes through all the segs and sets the bits to 0
    while( temp != NULL){
        for (int i = 0; i < 256; i++){
            temp->bits[i] = 0;
        }
        temp = temp->next;
    }
}

seg * whichseg(int j){
    seg * temp;
    int startPos = 0;

    int numSegs =  ( (j/2) ) / (256 * sizeof(int) * 8);

    //function to find specific segment 
    //also handles storin previous segment
    if(lastJ != NULL){
        if ( lastJ->prevJ < j ){
            temp = lastJ->lastSeg;
            startPos = lastJ->index;
        }
        else temp = head;
    }
    else temp = head;
    for (int i = startPos; i < numSegs; i++ ) temp = temp->next;
    
    if (lastJ == NULL) lastJ = (lastPointer *) malloc(sizeof(lastPointer));
    
    lastJ->lastSeg = temp;
    lastJ->prevJ = j;
    lastJ->index = numSegs;


    return temp;

}

int whichint(int j){
    //return array index
    return ( (j / (sizeof(int) * 8 * 2)) ) % 256;
}

int whichbit(int j){
    //returns bit index
    return ( (j % (sizeof(int) * 8 * 2) ) )/ 2;
}

coordinate getcoord(int j){
    
    //returns coordinate based on j
    coordinate temp = {whichseg(j),whichint(j),whichbit(j)};


    return temp;
}

void markcoord(coordinate c){
    //set bit pos to 1
    c.segpt->bits[c.intnum] |= (1 << c.bitnum);
}

int testcoord(coordinate c){
    //If bit position is 0 return 1
    if ((c.segpt->bits[c.intnum] & (1 << c.bitnum)) == 0){
        return 1;
    }
    else{
        return 0;
    }
    
}

void marknonprime(int j){
    //marks a number not prime
    markcoord(getcoord(j));
}

int testprime(int j){
    //test if a number if prime
    return testcoord(getcoord(j));
}

int whichnum(coordinate c){
    seg* temp = head;
    int segNum = 0;

    while (temp != NULL){
        if(temp == c.segpt) break;
        
        else{
            temp = temp->next;
            segNum++;
        }
    }
    //Finds segment Number and return specific number based on coordinate info
    return (16384 * segNum) + (c.intnum * 64) + ( (c.bitnum * 2) + 1 );

}

coordinate incrcoord(coordinate c, int inc){

    int carry = 0;
    c.bitnum += (inc/2);

    //keep tracks of a carry variable  and performs operations to handle overflow

    if(c.bitnum >= 32){
        carry = c.bitnum / 32;
        c.bitnum %= 32;
    }

    c.intnum += carry;
    if(c.intnum >= 256){
        c.segpt = c.segpt->next;
        c.intnum %= 256;
    }
    return c;
}

void sieveOfE(int N){
    setBits();

    int t = 3;
    //starts off by only going through all the odd numbers 
    //only goes up to square root of N
    while(t <= sqrt(N) + 1){

        int mult = t;
        
        //checks if the number is prime thens go through the mults and marks the nonprime
        if(testprime(t) == 1){    
            for(mult = t * t; mult <= N; mult+=t){
                if(mult % 2 != 0) marknonprime(mult);
            }
        }

        t += 2;

    }


    //Counts the number of prime
    int count = 0;
    for (t = 3; t <= N; t += 2){
        if (testprime(t) == 1) count++;
    }
    printf("Found %d odd primes \n",count);
}

void triplePrime(int m, int n, int N){
    int numSol = 0;
    int firstLarge, secondLarge, thirdLarge;
    for (int i = 3; i <= N; i += 2){
        
        //Goes through all the odd numbers and checks all the increments
        //if all three are prime store solutions 
        if(testcoord(getcoord(i)) == 1 && (i + m) < N && (i + n) < N){
            int mTest = testcoord(incrcoord(getcoord(i),m));
            int nTest = testcoord(incrcoord(getcoord(i),n));
            if((mTest) == 1 && (nTest) == 1){
                numSol ++;
                firstLarge = i;
                secondLarge = (i + m);
                thirdLarge = (i + n);

            }
        }
    }
    if( numSol == 0){
        printf("No solutions \n");
    }else{
        printf("%d soultions, largest (%d,%d,%d) \n",numSol, firstLarge, secondLarge, thirdLarge);
    }
    
}


void main(int argc, char *argv[]){

    int N;
    int numSegs;
    seg * tempSeg;
    seg * holder;
   
   //Grabs N parameter
    N = strtol(argv[1], NULL, 10);

    numSegs = ( (N/2) ) / (256 * sizeof(int) * 8);


    head = (seg *) malloc(sizeof(seg));

    // creates neccessary amount of Segs
    holder = head;
    for (int i = 1; i < numSegs + 1 ; i++){
        tempSeg = (seg *) malloc(sizeof(seg));
        holder->next = tempSeg;
        holder = holder->next;
        
    }
    

    sieveOfE(N);

    int m,n;
    int loop = 1;

    
    printf("Enter Two Even Numbers for Triple Prime Differential \n");

    int r;
    while(loop){
        
        r = scanf("%d %d",&m,&n);
    
        if (r == EOF) break;
        if (r == 2) triplePrime(m,n,N);
    }


}