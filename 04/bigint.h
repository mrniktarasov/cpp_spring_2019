#include <string>

using namespace std;

class BigInt
{
	private:
		string number;
		bool sign;
	public:
		BigInt(); 
		BigInt(const string& s);
		BigInt(const string& s, const bool sin); 

		BigInt(const int64_t n);
		void setNumber(const string& s);
		const string& getNumber() const;
		void setSign(bool s);
		const bool& getSign() const;
		BigInt absolute() const;
		BigInt& operator = (const BigInt& b);
		BigInt& operator = (const int64_t b);
		BigInt& operator = (const int b);
		bool operator ==  (const BigInt& b) const;
		bool operator !=  (const BigInt& b) const;
		bool operator >   (const BigInt& b) const;
		bool operator <   (const BigInt& b) const;
		bool operator >=  (const BigInt& b) const;
		bool operator <=  (const BigInt& b) const;
		BigInt operator + (const BigInt& b) const;
		BigInt operator * (const BigInt& b) const;
		BigInt operator - (const BigInt& b) const;
		BigInt& operator += (const BigInt& b);
		BigInt& operator -= (const BigInt& b);
		BigInt& operator *= (const BigInt& b);
		BigInt operator -();
		explicit operator string();

	friend ostream& operator<<(ostream& os, const BigInt& num);
	
	private:
		bool equals (const BigInt& n1, const BigInt& n2) const ;
		bool less (const BigInt& n1, const BigInt& n2) const ;
		bool greater (const BigInt& n1, const BigInt& n2) const ;
		string add (string& number1, string& number2) const ;
		string subtract (string& number1, string& number2) const ;
		string multiply (string& n1, string& n2) const ;
};
