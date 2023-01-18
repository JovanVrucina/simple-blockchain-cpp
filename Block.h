#include<string>
#include<vector>
#ifndef BLOCK_H
#define BLOCK_H

class TxData{
	public:
		std::string from;
		std::string to;
		std::string msg; //transaction message could be anything form hello world to functoin args(future implementations)
		double amount;
		time_t timestamp;
};

class Block{
	private:
		friend class Blockchain;
		unsigned int index; //Block index
		int TxNum; //Number of transactions in the block
		unsigned int nonce; //nonce for checking the validity of Tx
		size_t blockHash; 
		size_t prevHash; //Hash of the previous block (if genisis then hash(0))
		size_t validateHash(); //proof-of-work hash generation
		Block(int idx, TxData transacData[], size_t _prevHash,  int txNum);
		
	
	public:
			size_t getHash(); //hash getter function
			size_t generateHash(int nonce); //generating a hash with nonce for validity checking
			size_t getPrevHash();//getter for previous block hash
			int getIndex(); //getter for index
			int getNonce(); //getter for nonce

			std::vector<TxData> data; //transaction array for a single block (max 16 tx)

			bool isHashValid(); //function to check validity of the block
};

#endif