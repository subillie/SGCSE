#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <queue>
 
#define BITSNUMBYTE 8
 
using namespace std;
 
class Node {
public:
    char mCharacter;
    int mValue;
    Node* left = NULL;
    Node* right = NULL;
 
    Node(char character, int value, Node* nLeft = NULL, Node* nRight = NULL) {
        mCharacter = character;
        mValue = value;
        left = nLeft;
        right = nRight;
    }
};
 
 
// priority queue를 value 기준으로 오름차순 시켜주기 위한 operator.
struct cmp {
    bool operator()(Node& a, Node& b) {
        return a.mValue > b.mValue;
    }
};
 
 
// ********************
// @ makeTree
// @ 우선순위 큐(priority_queue)를 이용해 tree 만듦
// @ 1. 최소 빈도수 노드 두 개 고르고, 우선순위 큐에서 pop
// @ 2. 최소 빈도수 노드 두 개의 합으로 노드를 하나 만들고, 두 개의 노드는 자식으로 만듦
// @ 3. 만든 노드를 우선순위 큐에 push
// ********************
void makeTree(priority_queue<Node, vector<Node>, cmp>& p) {
    while (true) {
        size_t size = p.size();
        if (size < 2) break;
 
        // select two min value Node, and pop
        // calculate new value
        Node* firstMinNode = new Node(p.top().mCharacter, p.top().mValue,
            p.top().left, p.top().right);
        p.pop();
        Node* secondMinNode = new Node(p.top().mCharacter, p.top().mValue,
            p.top().left, p.top().right);
        p.pop();
        int newValue = firstMinNode->mValue + secondMinNode->mValue;
 
        // newValue로 새로운 Node 생성
        // newValue로 생성된 Node를 priority_queue에 push
        Node* newNode = new Node(NULL, newValue);
        newNode->left = firstMinNode;
        newNode->right = secondMinNode;
        p.push(*newNode);
    }
}
 
 
// ********************
// @ characterMapping
// @ mapping char-string. using unordered_map<char, string> characterMap
// @ tree(node)를 받아서 tree를 순회하면서 mapping 수행
// ********************
void characterMapping(string str, Node node, unordered_map<char, string>& characterMap) {
    if (node.mCharacter != NULL) {
        characterMap[node.mCharacter] = str;
        return;
    }
 
    str += "0";
    characterMapping(str, *node.left, characterMap);
    str[str.size() - 1] = '1';
    characterMapping(str, *node.right, characterMap);
}
 
 
// ********************
// @ byteToBitsString
// @ 1 byte를 입력으로 받아, 길이 len 만큼 binary string을 만들어 반환
// ********************
string byteToBitsString(unsigned char oneByte, int len) {
    string bitsString = "";
    for (int i = 7; i >= 0; i--) {
        if (len <= i) continue;
        unsigned char po = pow(2, i);
        if (oneByte >= po) {
            bitsString += '1';
            oneByte -= po;
        }
        else {
            bitsString += '0';
        }
    }
    return bitsString;
}
 
 
// ********************
// @ encoder
// @ mapping된 characterMap, HuffmanInput.txt에서 읽은 string txt를 가져옴
// @ txt에서 한글자씩 읽어 characterMap과 비교.
// @ 각 글자에 해당하는 binary를 encoding_text.txt에 write
// ********************
void encoder(const string& txt, unordered_map<char, string>& characterMap) {
    ofstream fout;
    string encodedTxt = "";
 
    fout.open("test.txt", ios::out | ios::binary);
    
    // mapping된 characterMap 기록
    size_t size = characterMap.size();
    unsigned char ucSize = size;
    fout << ucSize;
    
    // key, value bits len, value bits 기록
    unsigned char uc = 0;
    for (auto& i : characterMap) {
        uc = i.first;
        fout << uc;  // key 기록
        unsigned char len = i.second.size();
        fout << len;  // value bits 길이 기록
        
        unsigned char summ = 0;
        for (size_t j = 0; j < len;) {
            for (int k = 0; k < BITSNUMBYTE; k++, j++) {
                if (j > len-1) break;
                summ = (summ << 1) | (i.second[j] & 1);
            }
            fout << summ; // value bits 기록
            summ = 0;
        }
    }
 
    // 허프만 기법으로 encoding된 binary encodedTxt 만들기
    size = txt.size();
    for (size_t i = 0; i < size; i++) {
        char currentCharacter = txt[i];
        encodedTxt += characterMap[currentCharacter];
    }
 
    // encodedTxt의 8bits를 1byte로 기록
    unsigned char remainBit = 0; // bit 개수가 8의 배수가 아닐 경우, 나머지 bit처리
    size = encodedTxt.size();
    unsigned char sum = 0;
    for (size_t i = 0; i < size-1;) {
        for (int k = 0; k < BITSNUMBYTE; k++, i++) {
            if (i > size - 1) {
                remainBit = (unsigned char)k;
                break;
            }
            sum = (sum << 1) | (encodedTxt[i] & 1);
        }
        fout << sum;
        sum = 0;
    }
 
    // remainBit는 file의 마지막에 기록
    fout << remainBit;
    fout.close();
}
 
 
// ********************
// @ decoder
// @ encoding_text.txt를 불러옴
// @ encoding_text.txt에서 한 bit씩 읽어 앞 부분의 map 읽고, map 복구. 
// @ map 이후부터 encoding data 읽음.
// @ 마지막이 8bits로 안 나눠 떨어질 수 있음. 남은 bits만큼 읽기.
// ********************
void decoder() {
    ifstream fin;
    ofstream fout;
    fin.open("test.txt", ios::in | ios::binary);
    fout.open("testOutput.txt");
 
    // file의 길이 구하기
    fin.seekg(0, ios::end);
    int fileLen = fin.tellg();
    fin.seekg(0, ios::beg);
 
    // character 개수 세기
    int mapCount = 0;
    mapCount = fin.get();
    int hashLen = 1;
 
    // characterMap 복구
    unordered_map<string, char> characterMap;
    for (int i = 0; i < mapCount; i++) {
        char character = 0;
        fin.get(character);
        
        hashLen+= 2;
        int len = 0;
        len = fin.get();
 
        unsigned char value = 0;
        string valueString = "";
        for (int j = 0; j < len;) {
            value = fin.get();
            if (len > BITSNUMBYTE) {
                valueString += byteToBitsString(value, BITSNUMBYTE);
            }
            else {
                valueString += byteToBitsString(value, len);
            }
            hashLen++;
            len -= BITSNUMBYTE;
        }
 
        characterMap[valueString] = character;
    }
    
    // encoding.txt 를 읽어 binary string인 txt를 만듦
    fileLen -= hashLen;
    unsigned char ucCharacter = 0;
    string txt = "";
    while (true) {
        ucCharacter = fin.get();
        if (fin.fail()) break;
        if (fileLen > 2) {
            txt += byteToBitsString(ucCharacter, BITSNUMBYTE);
        }
        else {
            unsigned char remainBit = fin.get();
            txt += byteToBitsString(ucCharacter, (int)remainBit);
        }
        fileLen -= 1;
    }
    cout << txt << endl;
    // characterMap과 binary string인 txt를 비교하여 HuffmanOutput을 기록
    string tmpTxt = "";
    for (auto& i : txt) {
        tmpTxt += i;
        if(characterMap.count(tmpTxt)){
            fout << characterMap[tmpTxt];
            tmpTxt = "";
        }
    }
 
    fin.close();
    fout.close();
}
 
 
int main()
{
    ifstream fin;
    fin.open("effective.txt");
    
    unordered_map<char, int> hashMap;
    unordered_map<char, string> characterMap;
    string txt = "";
 
    // 각 character의 빈도수 세고, characterMap에 Mapping
    // string txt에 file 내용 옮기기.
    char character;
    while (true) {
        fin.get(character);
        if (fin.fail()) break;
        hashMap[character]++;
        txt += character;
    }
 
    // 우선순위 큐에 node 넣기
    // 정렬은 node의 value 기준으로 오름차순 정렬 (p.top()이 최소값이 됨)
    priority_queue<Node, vector<Node>, cmp> p;
    for (auto& i : hashMap) {
        Node node(i.first, i.second);
        p.push(node);
    }
 
    makeTree(p);
    characterMapping("", p.top(), characterMap);
    encoder(txt, characterMap);
    decoder();
 
    fin.close();
}