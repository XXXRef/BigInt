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

#include "bigint.hpp"

#include <cstdint>
#include <vector>
#include <string>
#include <list>

namespace BigInt {

	//====================================================================================================
	CBigInt::CBigInt(const std::string &str) : flagIsPositive(true) {
		if (str.empty()) {
			this->storage = "0";
			return;
		}
		if (str[0] == '-') {
			this->flagIsPositive = false;
			this->storage = str.substr(1, str.size());
			return;
		}
		this->storage = str;
	}

	//Only dec numbers
	inline unsigned Char2Int(unsigned char c) {
		return c - '0';
	}

	//Only dec numbers
	inline unsigned char Int2Char(unsigned i) {
		return i + '0';
	}

	//====================================================================================================
	CBigInt CBigInt::addAtomic(const CBigInt& bigNum1, const CBigInt& bigNum2) {
		//1.Align numbers
		//Align only one number, use 2nd number without copying
		const std::string* pNum1Storage;
		const std::string* pNum2Storage;
		std::string alignedString;
		int sizeDelta = bigNum1.storage.size() - bigNum2.storage.size();
		if (!sizeDelta) {

			pNum1Storage = &bigNum1.storage;
			pNum2Storage = &bigNum2.storage;
		}
		if (sizeDelta < 0) {
			alignedString = std::string(-sizeDelta, '0') + bigNum1.storage;
			pNum1Storage = &alignedString;
			pNum2Storage = &bigNum2.storage;
		}
		else {
			alignedString = std::string(sizeDelta, '0') + bigNum2.storage;
			pNum2Storage = &alignedString;
			pNum1Storage = &bigNum1.storage;
		}
		//2.Perform operation
		unsigned counter = pNum1Storage->size();
		std::uint8_t cf = 0;
		int iterationResult;
		CBigInt resultNum;
		resultNum.storage = std::string(pNum1Storage->size(), '.');
		while (counter) {
			--counter;
			iterationResult = Char2Int((*pNum1Storage)[counter]) + Char2Int((*pNum2Storage)[counter]) + cf;
			cf = (iterationResult > 9) ? (iterationResult -= 10, 1) : (0); //Wow, calm down
			resultNum.storage[counter] = Int2Char(iterationResult);
		}
		//add remainder
		if (cf) {
			resultNum.storage = "1" + resultNum.storage;
		}
		return resultNum;
	}

	//====================================================================================================
	CBigInt CBigInt::subAtomic(const CBigInt& bigNum1, const CBigInt& bigNum2) {
		//corner cases: '0-n','n-0'
		CBigInt resultNum;
		const CBigInt* pBiggerInt;
		const CBigInt* pLesserInt;
		//Order operands, determine result number sign
		if (bigNum1 < bigNum2) {
			resultNum.flagIsPositive = false;
			pBiggerInt = &bigNum2;
			pLesserInt = &bigNum1;
		}
		else {
			pBiggerInt = &bigNum1;
			pLesserInt = &bigNum2;
		}
		// Perform subtraction
		resultNum.storage = std::string(pBiggerInt->storage.size(), '.');
		CBigInt ovenNum = *pBiggerInt;
		size_t sizeDelta = pBiggerInt->storage.size() - pLesserInt->storage.size();
		size_t i = pLesserInt->storage.size();
		while (i) {
			--i;
			int delta = ovenNum.storage[sizeDelta + i] - pLesserInt->storage[i];
			if (delta < 0) {
				//acquire more from previous digits
				size_t j = sizeDelta + i - 1;//here j can be 0
				while (true) {
					if (ovenNum.storage[j] != '0') {
						//adjusting ovenNum
						--ovenNum.storage[j++];
						while (true) {
							if (j == sizeDelta + i) {
								break;
							}
							ovenNum.storage[j++] = '9';
						}
						delta += 10;
						break;
					}
					--j;
				}
			}
			resultNum.storage[sizeDelta + i] = Int2Char(delta);
		}
		//copy the rest of ovenNum into resultNum
		i = 0;
		while (true) {
			if (ovenNum.storage[i] != '0') {
				break;
			}
			++i;
		}
		for (size_t counter = i; counter < sizeDelta; ++counter) {
			resultNum.storage[counter] = ovenNum.storage[counter];
		}
		// Keep only digits, clear '.'s
		i = 0;
		while (true) {
			if (resultNum.storage[i] != '.') {
				break;
			}
			++i;
		}
		if (i) { //if i==0 no need to substr which copies data
			resultNum.storage = resultNum.storage.substr(i, resultNum.storage.size());
		}
		// Delete 0s from beginning if any
		i = 0;
		while (i != resultNum.storage.size() - 1) {
			if (resultNum.storage[i] != '0') {
				break;
			}
			++i;
		}
		if (i) { //if i==0 no need to substr which copies data
			resultNum.storage = resultNum.storage.substr(i, resultNum.storage.size());
		}
		return resultNum;
	}

	//====================================================================================================
	CBigInt CBigInt::multiplyByDigit(const CBigInt& bigNum, unsigned char digitChar) {
		//only positive digit realization
		if (digitChar == '0') {
			return CBigInt("0");
		}
		CBigInt resultNum;
		resultNum.flagIsPositive = bigNum.flagIsPositive;
		resultNum.storage = std::string(bigNum.storage.size(), '.');
		std::uint8_t iterationResult;
		std::uint8_t cf = 0;
		int counter = bigNum.storage.size();
		while (counter) {
			--counter;
			iterationResult = Char2Int(bigNum.storage[counter])*Char2Int(digitChar) + cf;
			if (iterationResult > 9) {
				cf = iterationResult / 10;
				iterationResult %= 10;
			}
			else {
				cf = 0;
			}
			resultNum.storage[counter] = Int2Char(iterationResult);
		}
		//process last digit if needed
		if (cf) {
			resultNum.storage = std::string(1, Int2Char(cf)) + resultNum.storage;
		}
		return resultNum;
	}

	//====================================================================================================
	CBigInt operator+(const CBigInt& num1, const CBigInt& num2) {
		//2 non negative numbers addition
		//2 non negative numbers substraction

		/*
		(+1)+(-2) = (+1)-(+2)
		(-1)+(+2) = (+2)-(+1)
		(+1)+(+2) = (+1)+(+2)
		(-1)+(-2) = -((+1)+(+2))
		*/

		if (num1.flagIsPositive == num2.flagIsPositive) {
			return (num1.flagIsPositive) ? (CBigInt::addAtomic(num1, num2)) : (-CBigInt::addAtomic(num1, num2));
		}

		//signs unequal
		return (num1.flagIsPositive) ? (CBigInt::subAtomic(num1, -num2)) : (CBigInt::subAtomic(num2, -num1));
	}

	//====================================================================================================
	CBigInt operator-(const CBigInt& num1, const CBigInt& num2) {
		/*
		(+1)-(-2) = (+1)+(+2)
		(-1)-(+2) = -((+1)+(+2))

		(-1)-(-2) = (+2)-(+1)
		(+1)-(+2) = (+1)-(+2)
		*/
		if (num1.flagIsPositive == num2.flagIsPositive) {
			return (num1.flagIsPositive) ? (CBigInt::subAtomic(num1, num2)) : (CBigInt::subAtomic(-num2, -num1));
		}

		//signs unequal
		return (num1.flagIsPositive) ? (CBigInt::addAtomic(num1, -num2)) : (-CBigInt::addAtomic(-num1, num2));

		/*
		if (num1.flagIsPositive){
			return CBigInt::addAtomic(num1, -num2);
		}
		else{
			auto tmpNum = -num1;
			return -CBigInt::addAtomic(tmpNum, num2);
		}
		*/

	}

	//====================================================================================================
	CBigInt operator-(const CBigInt& num1) {
		CBigInt resultNum = num1;
		resultNum.flagIsPositive = !resultNum.flagIsPositive;
		return resultNum;
	}

	//====================================================================================================
	CBigInt operator*(const CBigInt& num1, const CBigInt& num2) {
		//1.Determine sign
		//2.Calc mul
		//	operator+
		//  multByDigit

		CBigInt resultNum;

		if ((num1.storage == "0") || (num2.storage == "0")) {
			return CBigInt();
		}

		unsigned counter = num2.storage.size();
		while (counter) {
			--counter;
			if (num2.storage[counter] == '0') {
				continue;
			}
			CBigInt tmpNum = CBigInt::multiplyByDigit(num1, num2.storage[counter]);
			tmpNum.storage = tmpNum.storage + std::string(num2.storage.size() - counter - 1, '0');
			resultNum = resultNum + (tmpNum.flagIsPositive ? tmpNum : -tmpNum);
		}

		//Adjust sign
		if (num1.flagIsPositive != num2.flagIsPositive) {
			resultNum.flagIsPositive = false;
		}

		return resultNum;
	}

	//====================================================================================================
	bool divide(const CBigInt& dividend, const CBigInt& divisor, CBigInt& quotient, CBigInt& remainder) {
		//check if division by zero
		if (divisor.storage == "0") {
			throw CBigInt::ExZeroDivision();
		}
		//hard to work with '0' :->(3)
		if (dividend.storage == "0") {
			remainder.storage = quotient.storage = "0";
			return true;
		}
		std::size_t dividendMarker = 0;
		//cleaning storages
		quotient.storage = remainder.storage = "";
		while (true) {
			//acquire appropriate amount of digits from dividend into remainder
			std::size_t digitsAmountToAdd = divisor.storage.size() - remainder.storage.size();
			//if no more digits in dividend, we have to exit
			if (dividend.storage.size() < (dividendMarker + digitsAmountToAdd)) {
				if (quotient.storage != "") {
					quotient.storage += std::string(dividend.storage.size() - dividendMarker, '0');
				}

				remainder.storage += std::string(dividend.storage.cbegin() + dividendMarker, dividend.storage.cend());//!!!WARNING!!! Remainder can be "00...0"
				break;
			}

			if (!quotient.storage.empty() && digitsAmountToAdd) {
				quotient.storage += std::string(digitsAmountToAdd - 1, '0');
			}

			remainder.storage += std::string(dividend.storage.cbegin() + dividendMarker, dividend.storage.cbegin() + dividendMarker + digitsAmountToAdd);

			dividendMarker += digitsAmountToAdd;

			//Its possible that we need one more digit
			//Depends on divisor size
			if ((remainder != CBigInt("0")) && (remainder < divisor.abs())) {
				//Try add one more digit
				//If no more digits in dividend we have to exit
				if (dividend.storage.size() == dividendMarker) {
					//remainder is already set
					quotient.storage += std::string(dividend.storage.size() - dividendMarker, '0');
					//Quotient is already set too
					break;
				}

				//There is one more digit - we can use it
				remainder.storage += dividend.storage[dividendMarker++];

				if (!quotient.storage.empty() && digitsAmountToAdd) {
					quotient.storage += '0';
				}

			}
			//quotient and remainder is set
			//Calc how many divisors in reaminder
			unsigned numDivisors = 0;
			while (!(remainder < divisor.abs())) {
				remainder = remainder - divisor.abs();
				++numDivisors;
			}
			quotient.storage += std::to_string(numDivisors);
			if (remainder.storage == "0") {
				remainder.storage = "";
			}
		}
		if (quotient.storage == "") {
			quotient.storage = "0";
		}
		if (remainder.storage == "") {
			remainder.storage = "0";
		}

		//Care about signs/adjusting values

		//Set quotient sign
		quotient.flagIsPositive = !(dividend.flagIsPositive^divisor.flagIsPositive);
		//set remainder size?

		if (remainder != CBigInt("0")) {
			//Adjust quotient value
			if (false == dividend.flagIsPositive) {
				quotient += ((true == quotient.flagIsPositive) ? (CBigInt("1")) : (CBigInt("-1")));
			}
			//Adjust remainder value
			if (false == dividend.flagIsPositive) {
				remainder = divisor.abs() - remainder.abs();
			}
		}
		return true;
	}

	//====================================================================================================
	CBigInt operator/(const CBigInt& num1, const CBigInt& num2) {
		CBigInt quotient,remainder;
		divide(num1, num2, quotient, remainder);
		return quotient;
	}

	//====================================================================================================
	CBigInt operator%(const CBigInt& num1, const CBigInt& num2) {
		CBigInt quotient,remainder;
		divide(num1, num2, quotient, remainder); // No need in quotient -> CBigInt() as a out quotient reference stub
		return remainder;
	}

	//====================================================================================================
	//TODO make member function
	CBigInt& operator++(CBigInt& num) {
		num = num + CBigInt("1");
		return num;
	}

	//====================================================================================================
	//TODO make member function
	CBigInt operator++(CBigInt& num, int i) {
		CBigInt resultNum = num;
		num = num + CBigInt("1");
		return resultNum;
	}

	//====================================================================================================
	//TODO make member function
	CBigInt& operator--(CBigInt& num) {
		num = num - CBigInt("1");
		return num;
	}

	//====================================================================================================
	//TODO make member function
	CBigInt operator--(CBigInt& num, int i) {
		CBigInt resultNum = num;
		num = num - CBigInt("1");
		return resultNum;
	}



	//====================================================================================================
	/*
	O(n)-slow
	CBigInt operator^(const CBigInt& num1, const CBigInt& num2){
		//Существуют алгоритмы быстрого возведения в степень
		CBigInt bigCounter=("1");
		CBigInt resultNum = num1;
		while (bigCounter != num2){
			resultNum=resultNum*resultNum;
			++bigCounter;
		}

		return resultNum;
	}
	*/

	//Fast powering
	CBigInt operator^(const CBigInt& base, const CBigInt& power) {
		CBigInt result = base;
		//Corner cases
		if (power == CBigInt("0")) {
			return CBigInt("1");
		}
		if (power == CBigInt("1")) {
			return base;
		}
		// Collect odd flags
		std::list<bool> oddFlags;
		CBigInt newPow = power;
		while (true) {
			if (newPow == CBigInt("1")) {
				break;
			}
			oddFlags.push_front((newPow.isOdd() ? (true) : (false)));
			newPow = ((newPow.isOdd()) ? (newPow - CBigInt("1")) : (newPow)) / CBigInt("2");
		}
		//Perform loop powering
		for (const auto& oddFlag : oddFlags) {
			result *= result;
			if (oddFlag) {
				result = result*base;
			}
		}
		return result;
	}

	/*
	CBigInt powMod(const CBigInt& base, const CBigInt& pow, const CBigInt& mod){
	//!!!WARNING!!! division by zero
	CBigInt bigCounter;
	CBigInt result("1");
	while (bigCounter < pow){
	result=(result*base)%mod;
	++bigCounter;
	//std::cout << bigCounter << std::endl;
	}
	return result;
	}
	*/


	/*
	//recursion version
	//STACK OVERFLOW
	CBigInt powMod(const CBigInt& base, const CBigInt& power, const CBigInt& mod){
		CBigInt result;

		CBigInt newPow = (power.isOdd())?((power-CBigInt("1"))/CBigInt("2")):(power/CBigInt("2"));
		//if bottom of recursion reached
		if (newPow == CBigInt("1")){
			if (power.isOdd()){//power==3
				result=base*base*base%mod;
			}
			else{//power==2
				result = base*base%mod;
			}
		}
		else{//recurse
			CBigInt powModResult = powMod(base,newPow,mod);//<-recursion

			//result = powModResult*powModResult*((power.isOdd()) ? (base) : (CBigInt())) % mod;
			result = powModResult*powModResult%mod;
			if (power.isOdd()){
				result = result*base%mod;
			}
			else{
				result = result%mod;
			}
		}
		return result;
	}
	*/

	//====================================================================================================
	//loop version
	CBigInt powMod(const CBigInt& base, const CBigInt& power, const CBigInt& mod) {
		//true - power odd, and we need result=result*result*base
		//false - power even, and we need result=result*result
		CBigInt result = base;

		std::list<bool> oddFlags;
		CBigInt newPow = power;

		if (mod == CBigInt("0")) {
			throw CBigInt::ExZeroDivision();
		}

		if (power == CBigInt("0")) {
			return CBigInt("1");
		}

		if (power == CBigInt("1")) {
			return base%mod;
		}

		while (true) {
			if (newPow == CBigInt("1")) {
				break;
			}
			oddFlags.push_front((newPow.isOdd() ? (true) : (false)));
			newPow = ((newPow.isOdd()) ? (newPow - CBigInt("1")) : (newPow)) / CBigInt("2");
		}
		//Perform loop powering
		for (const auto& oddFlag : oddFlags) {
			result *= result;

			if (oddFlag) {
				result = result*base;
			}
			result %= mod;
		}
		return result;
	}

	//====================================================================================================
	CBigInt& CBigInt::operator+=(const CBigInt&num) {
		*this = *this + num;
		return *this;
	}

	//====================================================================================================
	CBigInt& CBigInt::operator-=(const CBigInt&num) {
		*this = *this - num;
		return *this;
	}

	//====================================================================================================
	CBigInt& CBigInt::operator/=(const CBigInt&num) {
		*this = *this / num;
		return *this;
	}

	//====================================================================================================
	CBigInt& CBigInt::operator*=(const CBigInt&num) {
		*this = *this * num;
		return *this;
	}

	//====================================================================================================
	CBigInt& CBigInt::operator%=(const CBigInt&num) {
		*this = *this % num;
		return *this;
	}

	//====================================================================================================
	CBigInt& CBigInt::operator^=(const CBigInt&num) {
		*this = *this ^ num;
		return *this;
	}

	//====================================================================================================
	bool operator<(const CBigInt& num1, const CBigInt& num2) {
		//Check case with different signs
		if (num1.flagIsPositive != num2.flagIsPositive) {
			//sign flags unequal
			return ((num1.flagIsPositive) ? (false) : (true));
		}
		bool flagRevert = false;
		if (!num1.flagIsPositive) {
			//2 minus signs
			flagRevert = true;
		}
		//Check if sizes not equal
		if (num1.storage.size() != num2.storage.size()) {
			return ((num1.storage.size() < num2.storage.size()) ? ((flagRevert) ? false : true) : ((flagRevert) ? true : false));
		}
		//Performm per-digit comparison
		for (size_t i = 0; i != num1.storage.size(); ++i) {
			if (num1.storage[i] != num2.storage[i]) {
				return (num1.storage[i] < num2.storage[i]) ? ((flagRevert) ? false : true) : ((flagRevert) ? true : false);
			}
		}
		//Numbers equal
		return false;
	}

	//====================================================================================================
	bool operator>(const CBigInt& num1, const CBigInt& num2) {
		return !((operator<(num1, num2)) || (operator==(num1, num2)));
	}

	//====================================================================================================
	bool operator==(const CBigInt& num1, const CBigInt& num2) {
		//num1=+0; num2=+0; -0 == +0
		if (num1.storage == "0" && num2.storage == "0") {
			return true;
		}
		//sign comparison
		if (num1.flagIsPositive != num2.flagIsPositive) {
			return false;
		}
		//Per-digit comparison
		for (size_t i = 0; i != num1.storage.size() + 1; ++i) {//end of string compared too
			if (num1.storage[i] != num2.storage[i]) {
				return false;
			}
		}
		return true;
	}

	//====================================================================================================
	bool operator!=(const CBigInt& num1, const CBigInt& num2) {
		return !operator==(num1, num2);
	}

	//====================================================================================================
	CBigInt CBigInt::abs() const {
		CBigInt result = *this;
		result.flagIsPositive = true;
		return result;
	}

	//====================================================================================================
	bool CBigInt::isOdd() const {
		return (Char2Int(this->storage[this->storage.size() - 1]) % 2) ? (true) : (false);
	}

	//====================================================================================================
	std::ostream& operator<<(std::ostream& outStream, CBigInt n) {
		outStream << ((n.flagIsPositive) ? ("") : ("-")) << n.storage;
		return outStream;
	}

	//====================================================================================================
	//Euclidean algorythm
	CBigInt getGCD(const CBigInt& n1Par, const CBigInt& n2Par) {
		CBigInt n1;
		CBigInt n2;

		if (n1Par < n2Par) {
			n1 = n2Par;
			n2 = n1Par;
		}
		else {
			n1 = n1Par;
			n2 = n2Par;
		}

		CBigInt quotient;
		CBigInt remainder;

		while (true) {
			divide(n1, n2, quotient, remainder);

			if (remainder == CBigInt("0")) {
				break;
			}

			n1 = n2;
			n2 = remainder;
		}

		return n2;
	}

	//====================================================================================================
	//Extended Euclidean algorithm
	bool getDiophantParams(const CBigInt& n1Par, const CBigInt& n2Par, CBigInt& k1, CBigInt& k2) {
		std::vector<CBigInt> paramsStack;
		bool flagExchanged = false;

		//fill stack
		CBigInt n1;
		CBigInt n2;

		if (n1Par < n2Par) {
			n1 = n2Par;
			n2 = n1Par;
			flagExchanged = true;
		}
		else {
			n1 = n1Par;
			n2 = n2Par;
		}

		CBigInt quotient;
		CBigInt remainder;

		//Euclid algorithm part
		while (true) {
			divide(n1, n2, quotient, remainder);

			if (remainder == CBigInt("0")) {
				break;
			}

			paramsStack.push_back(quotient);

			n1 = n2;
			n2 = remainder;
		}

		//phi1 - k1, phi2 - k2
			//init phis
		k1 = CBigInt("1");
		k2 = -paramsStack.back();
		paramsStack.pop_back();

		//check if elementary
		if (paramsStack.empty()) {
			return true;
		}

		//loop
		do {
			k1 += (-paramsStack.back())*(k2);
			paramsStack.pop_back();

			//exchange
			CBigInt tmpInt = k1;
			k1 = k2;
			k2 = tmpInt;
		} while (!paramsStack.empty());

		if (flagExchanged) {
			CBigInt tmpInt = k1;
			k1 = k2;
			k2 = tmpInt;
		}

		//k1, k2 has been set - we can return
		return true;
	}

	//====================================================================================================
	CBigInt multiplyKaratsuba(const CBigInt& n1, const CBigInt& n2) {
		CBigInt result;
		//Checking trivial cases
		if (n1 == CBigInt("0") || n2 == CBigInt("0")) {
			return CBigInt("0");
		}

		if (n1.storage.size() < BIGINT_LEN_BOUND_KARATSUBA || n2.storage.size() < BIGINT_LEN_BOUND_KARATSUBA) {
			//n1 or n2 small - use ordinary multiplication
			return n1*n2;
		}

		//Dividing multipliers
			//n1=a+b*2^n n1=n1_1+n1_2
			//n2=c+d*2^n n2=n2_1+n2_2
		CBigInt n1_1 = n1 / CBigInt("2");

		result.flagIsPositive = ~(n1.flagIsPositive^n2.flagIsPositive);
		return result;
	}
}