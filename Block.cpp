#include"Block.h"
#include<string>
#include<ctime>

Block::Block(int idx, TxData transacData[], size_t _prevHash, int txNum){
	index = idx;
	TxNum = txNum;

	
	for(int i=0; i<TxNum; i++){
		data.push_back(transacData[i]);
		data.back().timestamp = time(NULL);
	}

	prevHash = _prevHash;
	blockHash = validateHash();
}

size_t Block::validateHash(){
	nonce = 0;
	size_t fHash;
	do{	
		fHash = generateHash(nonce++);
	}while(fHash/10000000000000!=0);

	nonce--;
	return fHash;
		
}

size_t Block::generateHash(int _nonce){
	std::hash<std::string> hash1;
	std::hash<size_t> finalHash;

	//initialising hash
	std::string toHash = "\0";
	for(int i=0; i<TxNum;i++){
		std::string bufferHash = std::to_string(data[i].amount)+data[i].from+data[i].to+std::to_string(data[i].timestamp)+data[i].msg;
		toHash += bufferHash;
	}
	toHash += std::to_string(_nonce);

	return finalHash(hash1(toHash)+prevHash);

}

//block hash getter function
size_t Block::getHash(){
	return blockHash;
}

//block previous hash getter function
size_t Block::getPrevHash(){
	return prevHash;
}

int Block::getIndex(){
	return index;
}

int Block::getNonce(){
	return nonce;
}

//block function isHashValid
bool Block::isHashValid(){
	return generateHash(nonce) == blockHash;
}