#include"blockchain.h"

Blockchain::Blockchain(){
	TxData *dat = new TxData[16];
	dat[0].amount = 0;
	dat[0].from = "NIL";
	dat[0].to = "NIL";
	dat[0].msg = "NULL";
	dat[0].timestamp = time(NULL);

	std::hash<int> hash1;
	Block genesis(0, dat, hash1(0), 1);

	chain.push_back(genesis);
}

//FOR DEMO PURPOSES ONLY!!!!!!!!!!!
Block *Blockchain::getLatestBlock(){
	return &chain.back();
}

void Blockchain::addBlock(TxData transacData[],  int txNum){
	int index = chain.size();

	Block newBlock(index, transacData, chain.back().getHash(), txNum);
	chain.push_back(newBlock);
}

bool Blockchain::isValid(){
	std::vector<Block>::iterator it;
	size_t chainLen = chain.size();

	for(it=chain.begin(); it!=chain.end(); it++){
		Block currentBlock = *it;
		if(!currentBlock.isHashValid())
			return false;

		if(chainLen>1){
			Block prevBlock = *(it-1);
			if(currentBlock.getPrevHash()!=prevBlock.getHash())
				return false;
		}
	}

	return true;
}