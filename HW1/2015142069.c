/* 
 * CS:APP Data Lab 
 * 
 * <안유빈 2015142069>
 * 
 * bits.c - Source file with your solutions to the Lab.
 *          This is the file you will hand in to your instructor.
 *
 * WARNING: Do not include the <stdio.h> header; it confuses the dlc
 * compiler. You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.  
 */

#if 0
/*
 * Instructions to Students:
 *
 * STEP 1: Read the following instructions carefully.
 */

You will provide your solution to the Data Lab by
editing the collection of functions in this source file.

INTEGER CODING RULES:
 
  Replace the "return" statement in each function with one
  or more lines of C code that implements the function. Your code 
  must conform to the following style:
 
  int Funct(arg1, arg2, ...) {
      /* brief description of how your implementation works */
      int var1 = Expr1;
      ...
      int varM = ExprM;

      varJ = ExprJ;
      ...
      varN = ExprN;
      return ExprR;
  }

  Each "Expr" is an expression using ONLY the following:
  1. Integer constants 0 through 255 (0xFF), inclusive. You are
      not allowed to use big constants such as 0xffffffff.
  2. Function arguments and local variables (no global variables).
  3. Unary integer operations ! ~
  4. Binary integer operations & ^ | + << >>
    
  Some of the problems restrict the set of allowed operators even further.
  Each "Expr" may consist of multiple operators. You are not restricted to
  one operator per line.

  You are expressly forbidden to:
  1. Use any control constructs such as if, do, while, for, switch, etc.
  2. Define or use any macros.
  3. Define any additional functions in this file.
  4. Call any functions.
  5. Use any other operations, such as &&, ||, -, or ?:
  6. Use any form of casting.
  7. Use any data type other than int.  This implies that you
     cannot use arrays, structs, or unions.

 
  You may assume that your machine:
  1. Uses 2s complement, 32-bit representations of integers.
  2. Performs right shifts arithmetically.
  3. Has unpredictable behavior when shifting if the shift amount
     is less than 0 or greater than 31.


EXAMPLES OF ACCEPTABLE CODING STYLE:
  /*
   * pow2plus1 - returns 2^x + 1, where 0 <= x <= 31
   */
  int pow2plus1(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     return (1 << x) + 1;
  }

  /*
   * pow2plus4 - returns 2^x + 4, where 0 <= x <= 31
   */
  int pow2plus4(int x) {
     /* exploit ability of shifts to compute powers of 2 */
     int result = (1 << x);
     result += 4;
     return result;
  }

FLOATING POINT CODING RULES

For the problems that require you to implement floating-point operations,
the coding rules are less strict.  You are allowed to use looping and
conditional control.  You are allowed to use both ints and unsigneds.
You can use arbitrary integer and unsigned constants. You can use any arithmetic,
logical, or comparison operations on int or unsigned data.

You are expressly forbidden to:
  1. Define or use any macros.
  2. Define any additional functions in this file.
  3. Call any functions.
  4. Use any form of casting.
  5. Use any data type other than int or unsigned.  This means that you
     cannot use arrays, structs, or unions.
  6. Use any floating point data types, operations, or constants.


NOTES:
  1. Use the dlc (data lab checker) compiler (described in the handout) to 
     check the legality of your solutions.
  2. Each function has a maximum number of operations (integer, logical,
     or comparison) that you are allowed to use for your implementation
     of the function.  The max operator count is checked by dlc.
     Note that assignment ('=') is not counted; you may use as many of
     these as you want without penalty.
  3. Use the btest test harness to check your functions for correctness.
  4. Use the BDD checker to formally verify your functions
  5. The maximum number of ops for each function is given in the
     header comment for each function. If there are any inconsistencies 
     between the maximum ops in the writeup and in this file, consider
     this file the authoritative source.

/*
 * STEP 2: Modify the following functions according the coding rules.
 * 
 *   IMPORTANT. TO AVOID GRADING SURPRISES:
 *   1. Use the dlc compiler to check that your solutions conform
 *      to the coding rules.
 *   2. Use the BDD checker to formally verify that your solutions produce 
 *      the correct answers.
 */


#endif
/* Copyright (C) 1991-2018 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */
/* This header is separate from features.h so that the compiler can
   include it implicitly at the start of every compilation.  It must
   not itself include <features.h> or any other header that includes
   <features.h> because the implicit include comes before any feature
   test macros that may be defined in a source file before it first
   explicitly includes a system header.  GCC knows the name of this
   header in order to preinclude it.  */
/* glibc's intent is to support the IEC 559 math functionality, real
   and complex.  If the GCC (4.9 and later) predefined macros
   specifying compiler intent are available, use them to determine
   whether the overall intent is to support these features; otherwise,
   presume an older compiler has intent to support these features and
   define these macros by default.  */
/* wchar_t uses Unicode 10.0.0.  Version 10.0 of the Unicode Standard is
   synchronized with ISO/IEC 10646:2017, fifth edition, plus
   the following additions from Amendment 1 to the fifth edition:
   - 56 emoji characters
   - 285 hentaigana
   - 3 additional Zanabazar Square characters */
/* We do not support C11 <threads.h>.  */
/* 
 * bitXor - x^y using only ~ and & 
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
  return ~(x&y)&~(~x&~y);
}
/* 
 * anyEvenBit - return 1 if any even-numbered bit in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples anyEvenBit(0xA) = 0, anyEvenBit(0xE) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int anyEvenBit(int x) {
	int test = (0x55)|(0x55<<8)|(0x55<<16)|(0x55<<24); //set 10101010...
	int result = !!(x&test); // no even : 0 -> 1 -> 0 / else -> something -> 0 -> 1
  return result;
}
/* 
 * rotateLeft - Rotate x to the left by n
 *   Can assume that 0 <= n <= 31
 *   Examples: rotateLeft(0x87654321,4) = 0x76543218
 *   Legal ops: ~ & ^ | + << >> !
 *   Max ops: 25
 *   Rating: 3 
 */
int rotateLeft(int x, int n) {
	int ifsign=(0x01)&(x>>31); // 1 or 0
	int minusn = ~n+1;
	int y = x&(~(0x80<<24)); // else
	int xleft = (y<<n); // 0xdefeh000
	int xright = (y>>(32+minusn)) | (ifsign<<(n+(~0x01)+1)); // 0x00000abc
	int result = xleft|xright; // 0xdefegabc
  return result;
}
/*
 * bitReverse - Reverse bits in a 32-bit word
 *   Examples: bitReverse(0x80000002) = 0x40000001
 *             bitReverse(0x89ABCDEF) = 0xF7D3D591
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 45
 *   Rating: 4
 */
int bitReverse(int x) {
	int x1 = (x>>24)&0xFF;
	int x2 = (x>>16)&0xFF;
	int x3 = (x>>8)&0xFF;
	int x4 = x&0xFF;
	
	int mask1 = 0xF0; // 11110000
	int mask2 = 0xCC; // 11001100
	int mask3 = 0xAA; // 10101010

	x1 = ((x1&mask1)>>4)|((x1&~mask1)<<4);
	x1 = ((x1&mask2)>>2)|((x1&~mask2)<<2);
	x1 = ((x1&mask3)>>1)|((x1&~mask3)<<1);

	x2 = ((x2&mask1)>>4)|((x2&~mask1)<<4);
	x2 = ((x2&mask2)>>2)|((x2&~mask2)<<2);
	x2 = ((x2&mask3)>>1)|((x2&~mask3)<<1);

	x3 = ((x3&mask1)>>4)|((x3&~mask1)<<4);
	x3 = ((x3&mask2)>>2)|((x3&~mask2)<<2);
	x3 = ((x3&mask3)>>1)|((x3&~mask3)<<1);

	x4 = ((x4&mask1)>>4)|((x4&~mask1)<<4);
	x4 = ((x4&mask2)>>2)|((x4&~mask2)<<2);
	x4 = ((x4&mask3)>>1)|((x4&~mask3)<<1);

    return (x4<<24)+(x3<<16)+(x2<<8)+x1;
}
/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
/*
	int ifsign = (x>>31); // pos : 000..., neg : 111...
	x=(ifsign&(~x+1)) | ((~ifsign)&x);	// to unsigned
	int if0x80 = (x+(0x80<<24)); // if0x80 = 0 only when input is 0x80000000 , elwe  something 
	int gt16 = !!(x>>16); // if x>=2^16, 
	x = x>>(gt16<<4);	// devide 2^16 else nothing
	int gt8 = !!(x>>8); // if x>=2^8
	x = x>>(gt8<<3);	// devide 2^8 else nothing
	int gt4 = !!(x>>4);
	x = x>>(gt4<<2);
	int gt2 = !!(x>>2);
	x = x>>(gt2<<1);
	int gt1 = !!(x>>1); // if x>=2^1, 
	x = x>>gt1;		//devide 2 else nothing
	int getsum = (gt16<<4)+(gt8<<3)+(gt4<<2)+(gt2<<1)+gt1+x+1;
	int result = ((!if0x80)<<5)| (((!!if0x80<<31)>>31)&(getsum));
*/
// if neg(ifsign==1) : result=32, else (ifsign==0) : result = x+16(1)+8(2)+4(3)+2(4)+1(5)
  return 2;
}
/* 
 * tmin - return minimum two's complement integer 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin(void) {
  return 1<<0x1F;
}
/* 
 * implication - return x -> y in propositional logic - 0 for false, 1
 * for true
 *   Example: implication(1,1) = 1
 *            implication(1,0) = 0
 *   Legal ops: ! ~ ^ |
 *   Max ops: 5
 *   Rating: 2
 */
int implication(int x, int y) {
    return (!x)|(!!y);
}
/* 
 * conditional - same as x ? y : z 
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
	
  return ((((!!x)<<31)>>31)&y) | ((((!x)<<31)>>31)&z);
}
/* 
 * subtractionOK - Determine if can compute x-y without overflow
 *   Example: subtractionOK(0x80000000,0x80000000) = 1,
 *            subtractionOK(0x80000000,0x70000000) = 0, 
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Rating: 3
 */
int subtractionOK(int x, int y) {
	int xsign = x>>31;
	int ysign = y>>31;
	int signcheck = 0x80<<24;
	int ifsame = !(xsign^ysign); // same sign(safe) : 1, different sign : 0
	// x<0, y>0 : x=~x+1, x+y>2^31 <-> x=~x, x+y>=2^31 overflow else fine
	// x>0, y<0 : y=~y+1, x+y>=2^31 overflow else fine
	int isxoverflow = ((xsign)&(y+~x))&(signcheck); // overflow : 100.., else: 0	
	int isyoverflow = ((ysign)&(x+(~y)+1))&(signcheck); // overflow : 100.., else: 0
	
	int result = (ifsame) | ((!ifsame) & ((!isxoverflow)&(!isyoverflow)));
  return result;
}
/* 
 * isLessOrEqual - if x <= y  then return 1, else return 0 
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
	// x>=0 y>=0 : y+(~x)+1<0 (&0x80<<24) is 0, else 1
	// x>=0 y<0 : always 0
	// x<0 y>=0 : always 1
	// x<0 y<0 : y+(~x)<0 (&0x80<<24) is 0, else 1
	int xsign = x>>31;
	int ysign = y>>31;	
	
	int safe = (!ysign)&(!!xsign);
	int unsafe = (!xsign)&(!!ysign);
	
	int ifpos = (!xsign)&(!ysign);
	int ifneg = (!!xsign)&(!!ysign);
	int posminus = !!((y+(~x)+1)&(0x80<<24));	// if impossible, return 1
	int negminus = !!((y+(~x))&(0x80<<24));		// if impossible, return 1

	int result = (safe) | ((!unsafe) & ((ifpos&(!ifneg)&(!posminus)) | ((!ifpos)&ifneg&(negminus))));
/*
	int xsign = x>>31;
	int ysign = y>>31;
	int ifdiff = x^y; // same sign : 00...0, different sign : 11...1
	int ifsafe = !!((ifdiff)&(xsign)); // x<0, y>0 => 1, else 0
	int ifunsafe = !!((ifdiff)&(ysign)); // x>0, y<0 => 1, else 0
	int ifminus = !!((y+(~x)+1)&(0x80<<24)); // if y-x<0 return 1, else 0 
	int result = (ifsafe) | ((!ifsafe)&(!ifunsafe)&(!ifminus));
*/
  return result;
}
/* 
 * absVal - absolute value of x
 *   Example: absVal(-1) = 1.
 *   You may assume -TMax <= x <= TMax
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 10
 *   Rating: 4
 */
int absVal(int x) {
	int ifsign = x>>31;
	int result = ((~ifsign)&(x)) | ((ifsign)&(~x+1));
  return result;
}
/* 
 * logicalNeg - implement the ! operator, using all of 
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4 
 */
int logicalNeg(int x) {
	x = (x)|(x>>1);
	x = (x)|(x>>2);
	x = (x)|(x>>4);
	x = (x)|(x>>8);
	x = (x)|(x>>16);
	x = x^1;
	x = x&1;
  return x;
}
/* 
 * floatIsEqual - Compute f == g for floating point arguments f and g.
 *   Both the arguments are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representations of
 *   single-precision floating point values.
 *   If either argument is NaN, return 0.
 *   +0 and -0 are considered equal.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 25
 *   Rating: 2
 */
int floatIsEqual(unsigned uf, unsigned ug) {
	int frac = (0x7F<<16)+(0xFF<<8)+0xFF;
	int exp = 0xFF<<23;
	int s = 0x80<<24;

	//consider 3 : 1) totally same : 1  2) +0 & -0 : 1  3) NaN : 0 
	int totalsame = uf^ug; // return 0 only if totally same
	
	int pmzero = (!((uf^s)<<1)) & (!((ug^s)<<1)); // return 1 if both pmzero(X000...00), else 0
	int ufnan = (!(~(uf&exp))) & (!!(uf&frac));
	int ugnan =  (!(~(ug&exp))) & (!!(ug&frac));
	int result = (!(ufnan&ugnan))&((!totalsame) | (pmzero));
    return result;
}
/* 
 * floatUnsigned2Float - Return bit-level equivalent of expression (float) u
 *   Result is returned as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point values.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatUnsigned2Float(unsigned u) {
    return u;
}
/* 
 * floatScale64 - Return bit-level equivalent of expression 64*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 35
 *   Rating: 4
 */
unsigned floatScale64(unsigned uf) {
	int frac = 0;
	int exp = 0xFF<<23;
	int s = 0x80<<24;
	if (!(uf&exp)){
		if(!(uf&(0x	
	}
  return 2;
}
