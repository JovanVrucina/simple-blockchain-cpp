#include<iostream>
#include<string>
#include<ctime>
#include<vector>
using namespace std;
//classes definiton

//transaction data (struct)
class TxData{
	public:
		string from;
		string to;
		string msg; //transaction message could be anything form hello world to functoin args(future implementation)
		double amount;
		time_t timestamp;
};

//block class
class Block{
	private:
		unsigned int index;
		size_t blockHash;
		size_t prevHash;
		size_t validateHash(); //proof-of-work hash generation
		unsigned int nonce; //nonce for checking the validity of Tx
		
	
	public:
		//constructor
			Block(int idx, TxData transacData, size_t _prevHash);

			size_t getHash(); //hash getter function
			size_t generateHash(int nonce); //generating a hash with nonce for validity purposes
			size_t getPrevHash();//getter for previous block hash
			int getIndex(); //getter for index
			int getNonce(); //getter for nonce

			TxData data; //Transaction data for a single transaction
			bool isHashValid(); //function to check validity of the block
};

class Blockchain{
	private:
		Block static createGenesis(); //creating a genesis block

	public:
		vector<Block> chain; //chain of blocks - vector 

		Blockchain(); //constructor for the blockchain (calls createGenesis();)

		void addBlock(TxData transacData); //function for adding a block to the blockchain
		bool isValid(); //function for checking the validity of the whole blockchain

		Block *getLatestBlock(); //FOR DEMO PURPOSES ONLY! GETTING THE POINTER TO THE LATEST BLOCK TO DEMONSTRATE AN ATTACK ON THE NETWORK
};

//MAIN CODE
int main(void){
	Blockchain vrucinaCoin;

	TxData testTx;
	testTx.amount = 420;
	testTx.from = "Kid A";
	testTx.to = "Saul Goodman";
	testTx.msg = "Hello World!";

	cout<<"Trasnasction successful!\nNonce: "<<vrucinaCoin.chain.back().getNonce()<<"\nHash: "<<vrucinaCoin.chain.back().getHash()<<endl;

	return 0;
}

/*************************************FUNCTIONS****************************************************************/
/*************************************BLOCK CLASS MEMBERS******************************************************/
//FUNCTIONS OF ALL CLASS MEMBERS EXPLAINED IN
Block::Block(int idx, TxData transacData, size_t _prevHash){
	index = idx;
	data = transacData;
	prevHash = _prevHash;
	blockHash = validateHash();
}

size_t Block::validateHash(){
	nonce = 0;
	size_t fHash;
	do{	
		fHash = generateHash(nonce++);
	}while(fHash/1000000000000!=0); //Automatic difficulty to be added

	nonce--;
	return fHash;
		
}

size_t Block::generateHash(int _nonce){
	hash<string> hash1;
	hash<size_t> finalHash;
	string toHash = to_string(data.amount) + data.to + data.from + to_string(data.timestamp)+to_string(_nonce); 

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




//BLOCKCHAIN CLASS MEMBERS
//Blockchain constructor

Blockchain::Blockchain(){
		Block genesis = createGenesis();
		chain.push_back(genesis);
}

//function that creates genesis block
Block Blockchain::createGenesis(){
	TxData dat;
	dat.amount = 0;
	dat.from = "NIL";
	dat.to = "NIL";
	dat.msg = "NULL";
	dat.timestamp = time(NULL);

	hash<int> hash1;
	Block genesis(0, dat, hash1(0));

	return genesis;
}

//FOR DEMO PURPOSES ONLY!!!!!!!!!!!
Block *Blockchain::getLatestBlock(){
	return &chain.back();
}

void Blockchain::addBlock(TxData transacData){
	int index = chain.size();
	transacData.timestamp = time(NULL);
	Block newBlock(index, transacData, chain.back().getHash());
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