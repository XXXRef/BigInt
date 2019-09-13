<p align="center">

<h1 align="center">BigInt</h1>
C++ library to handle arbitrary-length integers and math operations with them
</p>

## Table of contents
- [What is it](#what-is-it)

- [How to build](#how-to-build)

- [How to use](#how-to-use)

## **What is it**
Standard C++ integer types (*int,long, ...*) are limited in sizes and thus values they can take. BigInt library allows to work with numbers of arbitrary size and provides various math operations with them.
Still don't understand much? Ok, watch closer:
```
#include "BigInt/bigint.hpp"

int main() {
	auto bigInt1 = BigInt::CBigInt("2938457295602498475103498275960567894021978450139485672045983457960394587");
	auto bigInt2 = BigInt::CBigInt("5639586739409287402945867359630956872804357826384592874");
	auto bigIntResult = (--bigInt1) + (bigInt2++); //+, increment,decrement
	bigIntResult = (bigInt1--) * (++bigInt2) / BigInt::CBigInt("32456327845763485746"); //multiplication,division
	std::cout << bigIntResult << std::endl; //output big integer
	return 0;
}
```
## Operations available now:
|   Operation  | Description                                                                                         |
|:------------:|-----------------------------------------------------------------------------------------------------|
|       +      | Addition                                                                                            |
|       -      | Subtraction                                                                                         |
|     ++/--    | Postfix/prefix increment/decrement                                                                  |
|       *      | Multiplication                                                                                      |
|       /      | Division. If *divisor==0* *CBigInt::ExZeroDivision* thrown                                          |
|       %      | Mod. If *divisor==0* *CBigInt:: ExZeroDivision* thrown                                                |
|       ^      | Powering. Only non-negative powers allowed                                                          |
|    *powMod*  | Powering by mod. Only non-negative powers allowed; If *divisor==0* *CBigInt::ExZeroDivision* thrown |
| +=,-=,*=,... | Short operator forms                                                                                |
|==,!=,<=,... | *bool* operators                                                                                     |
|      <<      | Output *CBigInt* number                                                                             |

## **How to build**
**Library build process:**
1. Cd into folder where you want build files to appear
```
cd <build_folder_path>
```
2. Run cmake providing BigInt project root folder path to it
```
cmake <BigInt_project_root_folder>
```
3. Run make
```
make
```
Now there is BigInt static library in build folder

**Testing app build process:**
1. Cd into folder where you want build files to appear
```
cd <build_folder_path>
```
2. Run cmake providing tests folder path to it
```
cmake <BigInt_project_root_folder>/tests
```
3. Run make
```
make
```
Now there is BigIntTest application in build folder

## **How to use**
**Testing:**
When BigIntTest application built it can be executed to check everything is ok and all tests passed

**Library integration:**
1. Copy *BigInt/include* folder contents into your project include folder / provide its path to compiler
2. Link BigInt library during project building