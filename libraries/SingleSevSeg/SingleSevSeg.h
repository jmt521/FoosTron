// #include "Arduino.h"

#ifndef SingleSevSeg_h
#define SingleSevSeg_h

class SingleSevSeg
{
    public:
        SingleSevSeg();

    private:

        const bool symbolMap[31][7] //this array maps the state of each segment for each simbol
		{
			{1,1,1,1,1,1,0}, // zero
			{0,1,1,0,0,0,0}, // one
			{1,1,0,1,1,0,1}, // two
			{1,1,1,1,0,0,1}, // three
			{0,1,1,0,0,1,1}, // four
			{1,0,1,1,0,1,1}, // five
			{1,0,1,1,1,1,1}, // six
			{1,1,1,0,0,0,0}, // seven
			{1,1,1,1,1,1,1}, // eight
			{1,1,1,1,0,1,1}, // nine
			
			{1,1,1,0,1,1,1}, // A
			{0,0,1,1,1,1,1}, // b
			{1,0,0,1,1,1,0}, // C
			{0,0,0,1,1,0,1}, // c
			{0,1,1,1,1,0,1}, // d
			{1,0,0,1,1,1,1}, // E
			{1,0,0,0,1,1,1}, // F
			//G not allowed
			{0,1,1,0,1,1,1}, // H
			{0,0,1,0,1,1,1}, // h
			{0,0,0,0,1,1,0}, // I
			{0,1,1,1,0,0,0}, // J
			//K not allowed
			{0,0,0,1,1,1,0}, // L
			//M not allowed
			{0,0,1,0,1,0,1}, // n
			{0,0,1,1,1,0,1}, // o
			{1,1,0,0,1,1,1}, // P
			{1,1,1,0,0,1,1}, // q
			{0,0,0,0,1,0,1}, // r
			//S is equal to five
			{0,0,0,1,1,1,1}, // t
			{0,1,1,1,1,1,0}, // U
			{0,0,1,1,1,0,0}, // u
			//V not allowed
			//W not allowed
			//X not allowed
			{0,1,1,1,0,1,1}, // y
			//Z not allowed
		};

};

#endif