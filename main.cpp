#include <iostream>
#include <string>

#include "DES.h"

using namespace std;

int main(){

    string pt, key;

    pt = "123456ABCD132536";
	key ="AABB09182736CCDD";
    //key = "00";

    key = hex2bin(key);
    cout << key<<endl;


    // Parity bit drop table
	int keyp[56]= { 57, 49, 41, 33, 25, 17, 9,
					1, 58, 50, 42, 34, 26, 18,
					10, 2, 59, 51, 43, 35, 27,
					19, 11, 3, 60, 52, 44, 36,
					63, 55, 47, 39, 31, 23, 15,
					7, 62, 54, 46, 38, 30, 22,
					14, 6, 61, 53, 45, 37, 29,
					21, 13, 5, 28, 20, 12, 4 };
    
    // getting 56 bit key from 64 bit using the parity bits
	key = permute(key, keyp, 56); // key without parity
    cout << key<<endl;
    
	// Number of bit shifts
	int shift_table[16] = { 1, 1, 2, 2,
							2, 2, 2, 2,
							1, 2, 2, 2,
							2, 2, 2, 1 };
    
	// Key- Compression Table
	int key_comp[48] = { 14, 17, 11, 24, 1, 5,
						3, 28, 15, 6, 21, 10,
						23, 19, 12, 4, 26, 8,
						16, 7, 27, 20, 13, 2,
						41, 52, 31, 37, 47, 55,
						30, 40, 51, 45, 33, 48,
						44, 49, 39, 56, 34, 53,
						46, 42, 50, 36, 29, 32 };

    // Splitting the key to left and right
	string left = key.substr(0, 28);
	string right = key.substr(28, 28);
    cout<<"left side:"<<left<< "right side:" << right<<endl;   

    vector<string> rkb; // rkb for RoundKeys in binary
	vector<string> rk; // rk for RoundKeys in hexadecimal
	for (int i = 0; i < 16; i++) {
		// Shifting
		left = shift_left(left, shift_table[i]);
		right = shift_left(right, shift_table[i]);
        //cout<<"left side:"<<left<< "right side:" << right<<endl; 
		
        // Combining
		string combine = left + right;

		// Key Compression
		string RoundKey = permute(combine, key_comp, 48);

		rkb.push_back(RoundKey);
		rk.push_back(bin2hex(RoundKey));
	}

    cout << "\nEncryption:\n\n";
	string cipher = encrypt(pt, rkb, rk);
	cout << "\nCipher Text: " << cipher << endl;

    cout << "\nDecryption\n\n";
	reverse(rkb.begin(), rkb.end());
	reverse(rk.begin(), rk.end());
	string text = encrypt(cipher, rkb, rk);
	cout << "\nPlain Text: " << text << endl;
    return 0;
}