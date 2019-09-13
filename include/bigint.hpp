//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                                                                          //
//              _____                    _____                    _____                    _____                    _____                _____              //
//             /\    \                  /\    \                  /\    \                  /\    \                  /\    \              /\    \             //
//            /::\    \                /::\    \                /::\    \                /::\    \                /::\____\            /::\    \            //
//           /::::\    \               \:::\    \              /::::\    \               \:::\    \              /::::|   |            \:::\    \           //
//          /::::::\    \               \:::\    \            /::::::\    \               \:::\    \            /:::::|   |             \:::\    \          //
//         /:::/\:::\    \               \:::\    \          /:::/\:::\    \               \:::\    \          /::::::|   |              \:::\    \         //
//        /:::/__\:::\    \               \:::\    \        /:::/  \:::\    \               \:::\    \        /:::/|::|   |               \:::\    \        //
//       /::::\   \:::\    \              /::::\    \      /:::/    \:::\    \              /::::\    \      /:::/ |::|   |               /::::\    \       //
//      /::::::\   \:::\    \    ____    /::::::\    \    /:::/    / \:::\    \    ____    /::::::\    \    /:::/  |::|   | _____        /::::::\    \      //
//     /:::/\:::\   \:::\ ___\  /\   \  /:::/\:::\    \  /:::/    /   \:::\ ___\  /\   \  /:::/\:::\    \  /:::/   |::|   |/\    \      /:::/\:::\    \     //
//    /:::/__\:::\   \:::|    |/::\   \/:::/  \:::\____\/:::/____/  ___\:::|    |/::\   \/:::/  \:::\____\/:: /    |::|   /::\____\    /:::/  \:::\____\    //
//    \:::\   \:::\  /:::|____|\:::\  /:::/    \::/    /\:::\    \ /\  /:::|____|\:::\  /:::/    \::/    /\::/    /|::|  /:::/    /   /:::/    \::/    /    //
//     \:::\   \:::\/:::/    /  \:::\/:::/    / \/____/  \:::\    /::\ \::/    /  \:::\/:::/    / \/____/  \/____/ |::| /:::/    /   /:::/    / \/____/     //
//      \:::\   \::::::/    /    \::::::/    /            \:::\   \:::\ \/____/    \::::::/    /                   |::|/:::/    /   /:::/    /              // 
//       \:::\   \::::/    /      \::::/____/              \:::\   \:::\____\       \::::/____/                    |::::::/    /   /:::/    /               // 
//        \:::\  /:::/    /        \:::\    \               \:::\  /:::/    /        \:::\    \                    |:::::/    /    \::/    /                // 
//         \:::\/:::/    /          \:::\    \               \:::\/:::/    /          \:::\    \                   |::::/    /      \/____/                 // 
//          \::::::/    /            \:::\    \               \::::::/    /            \:::\    \                  /:::/    /                               // 
//           \::::/    /              \:::\____\               \::::/    /              \:::\____\                /:::/    /                                // 
//            \::/____/                \::/    /                \::/____/                \::/    /                \::/    /                                 // 
//             ~~                       \/____/                                           \/____/                  \/____/                                  // 
//                                                                                                                                                          //
//                                                                                                                                                          //
//		BigInt                                                                                                                                              //
//		@ Vlad Salnikov (XXXRef), 2016                                                                                                                      //
//		xxxref.com                                                                                                                                          //
//                                                                                                                                                          //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//BIGINT_VERSION 1.0

#ifndef HEADER_BIGINT
#define HEADER_BIGINT

#include <string>
#include <iostream>
#include <exception>

namespace BigInt {

	/**
		Class implementing number of arbitrary length
	*/
	class CBigInt {
		bool flagIsPositive;

		/**
			Adds 2 non-negative BigInt numbers
		*/
		static CBigInt addAtomic(const CBigInt& num1, const CBigInt& num2);

		/**
			Subtracts 2 non-negative BigInt numbers
		*/
		static CBigInt subAtomic(const CBigInt& num1, const CBigInt& num2);//num1-num2

		/**
			Multiplies BigInt number by single-digit number
		*/
		static CBigInt multiplyByDigit(const CBigInt& num1, unsigned char digitChar);
		friend bool divide(const CBigInt& num1, const CBigInt& num2, CBigInt& quotient, CBigInt& remainder);

	public:
		std::string storage;

		CBigInt(const std::string &str = "");

		CBigInt abs() const;

		//arythmetic operators
		friend CBigInt operator+(const CBigInt& num1, const CBigInt& num2);
		friend CBigInt operator-(const CBigInt& num1, const CBigInt& num2);
		friend CBigInt operator*(const CBigInt& num1, const CBigInt& num2);
		friend CBigInt operator/(const CBigInt& num1, const CBigInt& num2);
		friend CBigInt operator%(const CBigInt& num1, const CBigInt& num2);
		friend CBigInt operator^(const CBigInt& num1, const CBigInt& num2); //power
		friend CBigInt& operator++(CBigInt& num);
		friend CBigInt operator++(CBigInt& num, int i); //postfix
		friend CBigInt& operator--(CBigInt& num);
		friend CBigInt operator--(CBigInt& num, int i); //postfix

		CBigInt& operator+=(const CBigInt&num);
		CBigInt& operator-=(const CBigInt&num);
		CBigInt& operator*=(const CBigInt&num);
		CBigInt& operator/=(const CBigInt&num);
		CBigInt& operator%=(const CBigInt&num);
		CBigInt& operator^=(const CBigInt&num);

		//comparison operators
		friend bool operator<(const CBigInt& num1, const CBigInt& num2);
		friend bool operator>(const CBigInt& num1, const CBigInt& num2);
		friend bool operator==(const CBigInt& num1, const CBigInt& num2);
		friend bool operator!=(const CBigInt& num1, const CBigInt& num2);

		friend CBigInt operator-(const CBigInt& num2);

		friend CBigInt multiplyKaratsuba(const CBigInt& n1, const CBigInt& n2);
		friend CBigInt powMod(const CBigInt& base, const CBigInt& pow, const CBigInt& mod);

		//bool operators?

		//output
		friend std::ostream& operator<<(std::ostream& outStream, CBigInt n);

		bool isOdd() const;

		//Exceptions
		class ExZeroDivision : public std::exception {};
	};

	CBigInt getGCD(const CBigInt& n1Par, const CBigInt& n2Par); //Greatest common divisor
	bool getDiophantParams(const CBigInt& n1Par, const CBigInt& n2Par, CBigInt& k1, CBigInt& k2);

#define BIGINT_LEN_BOUND_KARATSUBA 80
	CBigInt multiplyKaratsuba(const CBigInt& n1, const CBigInt& n2);

	CBigInt powMod(const CBigInt& base, const CBigInt& pow, const CBigInt& mod);
}
#endif