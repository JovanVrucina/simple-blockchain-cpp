#include"Block.h"
#include<vector>
#ifndef BLOCKCHAIN_H
#define BLOCKCHIAN_H

class Blockchain{
	private:
		Block createGenesis(); //creating a genesis block

	public:
		std::vector<Block> chain; //chain of blocks - vector 

		Blockchain(); //constructor for the blockchain (calls createGenesis(); which calls addBlock();)

		void addBlock(TxData transacData[16], int txNum); //function for adding a block to the blockchain
		bool isValid(); //function for checking the validity of the whole blockchain

		Block *getLatestBlock(); //FOR DEMO PURPOSES ONLY!!!!! GETTING THE POINTER TO THE LATEST BLOCK TO DEMONSTRATE AN ATTACK ON THE NETWORK
};

#endif