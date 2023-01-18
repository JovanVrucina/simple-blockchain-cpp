#include<iostream>
#include<string>
#include<ctime>
#include<vector>
#include"Block.h"
using namespace std;


class Blockchain{
	private:
		Block createGenesis(); //creating a genesis block

	public:
		vector<Block> chain; //chain of blocks - vector 

		Blockchain(); //constructor for the blockchain (calls createGenesis(); which calls addBlock();)

		void addBlock(TxData transacData[16], int txNum); //function for adding a block to the blockchain
		bool isValid(); //function for checking the validity of the whole blockchain

		Block *getLatestBlock(); //FOR DEMO PURPOSES ONLY!!!!! GETTING THE POINTER TO THE LATEST BLOCK TO DEMONSTRATE AN ATTACK ON THE NETWORK
};

/********************************************BLOCK CLASS MEMBERS*********************************************************************/
int main(void){
	cout<<"Probably initialising and validating the genesis block"<<endl;

	Blockchain vrucinaChain;
	TxData mempool[16];

	mempool[0].amount = 6.9;
	mempool[0].from = "Vitalik Buterin";
	mempool[0].to = "JovanVrucina";
	mempool[0].msg = "This code could use some cleanup";

	mempool[1].amount = 4.2;
	mempool[1].from = "JovanVrucina";
	mempool[1].to = "Vitalik Buterin";
	mempool[1].msg = "I know";

	cout<<"Okay, now it's validating the first block"<<endl;
	vrucinaChain.addBlock(mempool, 2);

	for(int i=0; i<2; i++)
		cout<<"\nTransaction "<<i<<":\nAmount: "<<vrucinaChain.chain.back().data[i].amount<<"\nFrom: "<<vrucinaChain.chain.back().data[i].from<<"\nTo: "<<vrucinaChain.chain.back().data[i].to<<"\nMessage: "<<vrucinaChain.chain.back().data[i].msg<<endl;
	

	if(vrucinaChain.isValid())
		cout<<"\nThe data in the blockchain is valid!"<<endl;
	else
		cout<<"\nThe data in the blockchain is invalid"<<endl;

	return EXIT_SUCCESS;	
}

/*************************************FUNCTIONS****************************************************************/
/*************************************CLASS MEMBER FUNCTIONS***************************************************/
/*************************************BLOCKCHAIN CLASS MEMBERS******************************************************/

Blockchain::Blockchain(){
	TxData *dat = new TxData[16];
	dat[0].amount = 0;
	dat[0].from = "NIL";
	dat[0].to = "NIL";
	dat[0].msg = "NULL";
	dat[0].timestamp = time(NULL);

	hash<int> hash1;
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
	vector<Block>::iterator it;
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