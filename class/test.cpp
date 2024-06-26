#include "../headers/test.hpp"

void testNTTmatrice(){
    NTTCoef c0(0);
    NTTCoef c1(1);
    NTTCoef c2(2);
    NTTCoef c3(3);
    NTTCoef c4(4);
    NTTCoef c5(5);
    NTTCoef c6(6);
    NTTCoef c7(7);
    NTTCoef c8(8);
    
    NTT ntt1({c0, c1, c2, c3, c4, c5, c6, c7, c8});
    NTT ntt2({c8, c7, c6, c5, c4, c3, c2, c1, c0});
    NTT ntt3({c1, c1, c1, c1, c2, c2, c2, c2, c2});
    NTT ntt4({c3, c3, c3, c3, c4, c4, c4, c4, c4});
    NTT ntt5({c1, c1, c1, c1, c1, c1, c1, c1, c1});
    NTT ntt6({c2, c2, c2, c2, c2, c2, c2, c2, c2});

    NTTmatrice m1;
    m1.set(0,0,ntt5);
    m1.set(0,1,ntt5);
    m1.set(1,0,ntt5);
    m1.set(1,1,ntt5);

    NTTmatrice m2;
    m2.set(0,0,ntt6);
    m2.set(0,1,ntt6);
    m2.set(1,0,ntt6);
    m2.set(1,1,ntt6);

    cout << endl << "----- NTT matrice 1-----" << endl;
    for(int i=0; i<m1.sizeRow(); i++){
        for(int j=0; j<m1.sizeCol(); j++){
            cout << endl << "NTT matrice 1: " << i << " " << j << endl;
            for(int k=0; k<m1.get(i,j).getCoef().size(); k++){
                if(k<9){
                    cout << m1.get(i,j).getCoef()[k] << " ";
                }
            }
            cout << endl;
        }
    }

    cout << endl << "----- NTT matrice 2-----" << endl;
    for(int i=0; i<m2.sizeRow(); i++){
        for(int j=0; j<m2.sizeCol(); j++){
            cout << endl << "NTT matrice 2: " << i << " " << j << endl;
            for(int k=0; k<m2.get(i,j).getCoef().size(); k++){
                if(k<9){
                    cout << m2.get(i,j).getCoef()[k] << " ";
                }
            }
            cout << endl;
        }
    }

    NTTmatrice m3 = m1+m2;
    cout << endl << "----- NTT matrice 3 = m1+m2-----" << endl;
    for(int i=0; i<m3.sizeRow(); i++){
        for(int j=0; j<m3.sizeCol(); j++){
            cout << endl << "NTT matrice 3: " << i << " " << j << endl;
            for(int k=0; k<m3.get(i,j).getCoef().size(); k++){
                if(k<9){
                    cout << m3.get(i,j).getCoef()[k] << " ";
                }
            }
            cout << endl << endl;
        }
    }
}

void testBitByteArray(){
    cout << "----- BitArray -----" << endl;

    BitArray bitArray(8);
    // Set bit : 0 1 0 1 0 1 0 1
    bitArray.setBitArray({Bit(0), Bit(1), Bit(0), Bit(1), Bit(0), Bit(1), Bit(0), Bit(1)});
    //cout << "Taille du tableau de bits : " << bitArray.getSize() << endl;
    cout << "Expected bit array : 0-1-0-1-0-1-0-1" << endl;
    cout << "BitArray : " << bitArray << endl << endl;

    cout << "----- BitsToBytes -----" << endl;
    // BitsToBytes
    ByteArray byteArray = ByteArray::bitsToBytes(bitArray);
    // Expected byte array : 0b01010101
    //cout << "Taille du tableau de bytes : " << byteArray.getSize() << endl;
    cout << "Expected byte array from bit array : 01010101" << endl;
    cout << "ByteArray : " << byteArray << endl << endl;

    cout << "----- BytesToBits -----" << endl;
    BitArray bitArray2 = BitArray::bytesToBits(byteArray);
    // Expected bit array : 0-1-0-1-0-1-0-1
    //cout << "Taille du tableau de bits : " << bitArray2.getSize() << endl;
    cout << "Expected bit array : 0-1-0-1-0-1-0-1" << endl;
    cout << "BitArray : " << bitArray2 << endl << endl;

    cout << "----- IntArray -----" << endl;
    IntArray F({15, 3});
    cout << "IntArray : " << F << endl << endl;

    uint32_t d = 12;
    cout << "----- Byte encode on " << d <<" bits (from IntArray) -----" << endl;
    ByteArray byteArray3 = ByteArray::byteEncode(F, d);
    // Expected byte array : 0b00001111 0b00000011
    //cout << "Taille du tableau de bytes : " << byteArray3.getSize() << endl;
    cout << "Expected byte array : 00000000 11110000 00000011" << endl;
    cout << "ByteArray : " << byteArray3 << endl << endl;

    cout << "----- Byte encode 8 (from IntArray) -----" << endl;
    ByteArray byteArray10 = ByteArray::byteEncode(F, 8);
    cout << "Expected byte array : 00001111 00000011" << endl;
    cout << "ByteArray : " << byteArray10 << endl << endl;
    
    cout << "----- Byte decode (from ByteArray) -----" << endl;
    IntArray decodeArray = ByteArray::byteDecode(byteArray3, d);
    //cout << "Taille du tableau d'entiers : " << decodeArray.getSize() << endl;
    cout << "Expected int array : 15 3" << endl;
    cout << "IntArray : " << decodeArray << endl << endl;

    cout << "----- New ByteArray -----";
    ByteArray singleBlock(9);
    singleBlock.set({0b10101010, 0b01010101, 0b11111111, 0b00000000, 0b10101010, 0b01010101, 0b11111111, 0b00000000, 0b00001111});
    //cout << "Taille du tableau de bytes : " << singleBlock.getSize() << endl;
    cout << "Expected byte array : 10101010 01010101 11111111 00000000 10101010 01010101 11111111 00000000 00001111" << endl;
    cout << "ByteArray" << singleBlock << endl << endl;
    
    cout << "----- Byte decode on 8 bits (from ByteArray) -----" << endl;
    IntArray singleArray = ByteArray::byteDecode(singleBlock, 8);
    //cout << "Taille du tableau d'entiers : " << singleArray.getSize() << endl;
    cout << "Expected int array : 10, 5, 15, 0, 10, 5, 15, 0, 15" << endl;
    cout << "IntArray" << singleArray << endl << endl;

    cout << "----- New IntArray -----" << endl;
    IntArray F2({2725, 1535, 10, 2645, 4080, 255});
    cout << "Expected int array : 2725, 1535, 10, 2645, 4080, 255" << endl;
    cout << "IntArray" << F2 << endl << endl;

    cout << "----- Byte encode on " << d <<" bits (from IntArray) -----" << endl;
    ByteArray byteArray4 = ByteArray::byteEncode(F2, d);
    // Expected byte array : 0b00001111 0b00000011
    //cout << "Taille du tableau de bytes : " << byteArray4.getSize() << endl;
    cout << "Expected byte array : 10101010 01010101 11111111 00000000 10101010 01010101 11111111 00000000 11111111" << endl;
    cout << "ByteArray" << byteArray4 << endl << endl;

    cout << "----- Byte decode -----" << endl;
    IntArray decodeArray2 = ByteArray::byteDecode(byteArray4, d);
    cout << "Taille du tableau d'entiers : " << decodeArray2.getSize() << endl;
    cout << "Expected int array : 2725, 1535, 10, 2645, 4080, 255" << endl;
    cout << "IntArray" << decodeArray2 << endl << endl;
}

void testXOF() {
    cout << "----- XOF -----" << endl;
    vector<uint8_t> rho = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};
    XOF xof(rho,2,2); xof.init();
    for(int i=0;i<xof.getDigest().size();i++) cout << static_cast<uint16_t>(xof.getDigest()[i]) << " ";
    cout << endl<< "On rallonge de 3 bytes avec la méthode next()" << endl; xof.next();
    for(int i=0;i<xof.getDigest().size();i++) cout << static_cast<uint16_t>(xof.getDigest()[i]) << " ";
    cout << endl << "On rallonge encore de 3 bytes et on affiche que ces 3 derniers" << endl; xof.next();
    vector<uint8_t> nextBytes = xof.getLastThree(35);
    for(int i=0;i<3;i++) cout << static_cast<uint16_t>(nextBytes[i]) << " ";
    cout << endl << endl;
}

void testRBG() {
    cout << "----- Random Bytes Generator -----" << endl;
    vector<uint8_t> generatedBytes = Crypto::generateRandomBytes(16);
    cout << "Randomly generated bytes :" << endl;
    for(uint8_t elem : generatedBytes) cout << static_cast<uint16_t>(elem) << " ";
    vector<uint8_t> generatedBytes2 = Crypto::generateRandomBytes(32);
    cout << endl << "Other randomly generated bytes :" << endl;
    for(uint8_t elem : generatedBytes2) cout << static_cast<uint16_t>(elem) << " ";
    cout << endl << endl;
}


void testCompDecomp() {
    cout << "----- Compress -----" << endl;
    uint16_t compress(255);
    uint16_t newComp = Crypto::compress(compress, 4);
    cout << static_cast<uint16_t>(newComp);
    cout << endl << endl;

    cout << "----- Decompress -----" << endl;
    uint16_t decompress(255);
    uint16_t newDec = Crypto::decompress(decompress, 4);
    cout << static_cast<uint16_t>(newDec);
    cout << endl << endl;

    cout << "----- Compress after Decompress -----" << endl;
    uint16_t mlkj(255);
    uint16_t res = Crypto::compress(Crypto::decompress(mlkj,4),4);
    cout << static_cast<uint16_t>(res) << endl;
}