#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
using namespace std;

int main(int argc, char* argv[]) {

    // Check the number of arguments
    if (argc != 6) {
        // If wrong, let the user know the usage
        cerr << "usage: " << argv[0] << " input_file output_file result_file generator dataword_size" << endl;
        return 1;
    }

    // Check if the params are valid, and initialize the variables
    ifstream input(argv[1], ios::binary);
    if (!input.is_open()) {
        cerr << "input file open error." << endl;
        return 1;
    }
    ofstream output(argv[2], ios::binary);
    if (!output.is_open()) {
        cerr << "output file open error." << endl;
        return 1;
    }
    ofstream result(argv[3]);
    if (!result.is_open()) {
        cerr << "result file open error." << endl;
        return 1;
    }
    bitset<9> generator(argv[4]);
    int dataword_size = stoi(argv[5]);
    if (dataword_size != 4 && dataword_size != 8) {
        cerr << "dataword size must be 4 or 8." << endl;
        return 1;
    }

    // Calculate the CRC checksum
    int padding_size;
    input.read((char*)&padding_size, sizeof(padding_size));
    vector<char> crc(dataword_size / 8);
    bitset<8> checksum;
    int frame_count = 1, error_count = 0;
    for (; input.read(&crc[0], crc.size()); frame_count++) {
        // Remove the padding bits
        if (frame_count == 1) {
            checksum = bitset<8>(crc[0]);
            checksum >>= (8 - padding_size);
            crc[0] &= ((1 << (8 - padding_size)) - 1);
        }

        // Convert to a bitset
        bitset<64> codeword;
        for (int i = 0; i < crc.size(); i++) {
            codeword <<= 8;
            codeword |= bitset<8>(crc[i]);
        }
        codeword >>= (64 - dataword_size - 9);

        // Calculate the CRC checksum
        for (int i = 0; i < dataword_size + 9; i++) {
            bool bool_xor = checksum[7] ^ codeword[i];
            checksum <<= 1;
            checksum[0] = bool_xor;
            if (i == dataword_size + 8 && checksum.any())
                error_count++;
        }

        // Write the dataword to the output file
        bitset<64> dataword = codeword >> 9;
        for (int i = dataword_size / 8 - 1; i >= 0; i--)
            for (int j = 7; j >= 0; j--)
                output.put(dataword[i * 8 + j]);
    }

    // Write the CRC checksum to the output file
    for (int i = 7; i >= 0; i--)
        output.put(checksum[i]);

    // Write the result to the result file
    result << "number of frames: " << frame_count << endl;
    result << "number of errors detected: " << error_count << endl;

    // Close the files
    input.close();
    output.close();
    result.close();

    return 0;
    }
