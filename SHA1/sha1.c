#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <math.h>

long msgDigest[5], key[4];

long int modulo_Euclidian(long int a, long int b) {
  int m = a % b;
  if (m < 0)
    m = (b < 0) ? m - b : m + b;
  return m;
}

void printMessageGoingToBeProcessed(long int * message) {
    printf("\n");
    int iterator = 0;
        while (iterator<15) {
            printf("%08lx ",message[iterator++]);
            printf("%08lx ",message[iterator++]);
            printf("%08lx ",message[iterator++]);
            printf("%08lx \n",message[iterator++]);
        }
}

void commonAlgorithm(long int hashResult, long int word, long int key) {
    hashResult = modulo_Euclidian((hashResult + msgDigest[4]), 4294967296); // 2^32 = 4294967296
    hashResult = modulo_Euclidian((hashResult + msgDigest[1]<<5), 4294967296);
    hashResult = modulo_Euclidian((hashResult + word), 4294967296);
    hashResult = modulo_Euclidian((hashResult + key), 4294967296);
    msgDigest[4] = msgDigest[3];
    msgDigest[3] = msgDigest[2];
    msgDigest[2] = msgDigest[1]<<30;
    msgDigest[1] = msgDigest[0];
    msgDigest[0] = hashResult;
}

void messageHashing(long int * message) {
    //Now i have the message, Time to hash it.
    int iterator;
    long int hashResult;
    for (iterator=0;iterator<=79;iterator++) {
        if(iterator >=0 && iterator <= 19) {
            hashResult = ((msgDigest[1] & msgDigest[2]) | ((~msgDigest[1]) & msgDigest[3]));
            commonAlgorithm(hashResult, message[iterator%16],key[0]);
        } else if(iterator >=20 && iterator <= 39) {
            hashResult = (msgDigest[1] ^ msgDigest[2] ^ msgDigest[3]);
            commonAlgorithm(hashResult, message[iterator%16],key[1]);
        } else if(iterator >=40 && iterator <= 59) {
            hashResult = ((msgDigest[1] & msgDigest[2]) | (msgDigest[1] & msgDigest[3]) | (msgDigest[2] & msgDigest[3]));
            commonAlgorithm(hashResult, message[iterator%16],key[2]);
        } else if(iterator >=60 && iterator <= 79) {
            hashResult = (msgDigest[1] ^ msgDigest[2] ^ msgDigest[3]);
            commonAlgorithm(hashResult, message[iterator%16],key[3]);
        }
    }
}

void main() {
    // Initiate
    char secureHashInput[500];
    long message[15];
    int charsProcessed=0,charsInMessage, iterator;
    unsigned long long inputLenght;
    // Initial Key Values
    key[0] = 0x5A827999;
    key[1] = 0x6ED9EBA1;
    key[2] = 0x8F1BBCDC;
    key[3] = 0xCA62C1D6;
    // Initial msgDigest Values
    msgDigest[0] = 0x67452301;
    msgDigest[1] = 0xEFCDAB89;
    msgDigest[2] = 0x98BADCFE;
    msgDigest[3] = 0x10325476;
    msgDigest[4] = 0xC3D2E1F0;
    // SHA1 Process
        // Take Input Fix me: Remove strcpy and put take input!
        strcpy(secureHashInput,"abcde");
        // printf("Enter the string to be hashed: ");
        // fgets (secureHashInput, 500, stdin);
        // Calculate SHA1
        inputLenght = 8*(strlen(secureHashInput));
        message[14] = inputLenght/pow(2,32);
        inputLenght = inputLenght<<32;
        message[15] = inputLenght/pow(2,32);
        while (charsProcessed < strlen(secureHashInput)) { // This while loop will divide input to 32 bit messages
            for (charsInMessage=0;charsInMessage<=13;charsInMessage++) { // Reseting the entire message to 0 so that no previous value remains
                message[charsInMessage] = 0x00*pow(2,24) + 0x00*pow(2,16) + 0x00*pow(2,8) + 0x00;
            }
            for (charsInMessage=0;charsInMessage<=13;charsInMessage++) { // Fill in the words for this message
                if(charsProcessed == strlen(secureHashInput)) { // All chars are processed
                    message[charsInMessage++] = 0x80*pow(2,24);
                    break;
                } else if (charsProcessed+4 > strlen(secureHashInput)) { // If the word will not be completely filled with input.
                    iterator = 24;
                    while(1) {
                        if (charsProcessed < strlen(secureHashInput)) {
                            message[charsInMessage] += secureHashInput[charsProcessed++]*pow(2,iterator);
                            iterator=iterator-8;
                        } else {
                            message[charsInMessage] += 0x80*pow(2,iterator);
                            break;
                        }
                    }
                    break;
                } else {
                    message[charsInMessage] =
                        secureHashInput[charsProcessed++]*pow(2,24) +
                        secureHashInput[charsProcessed++]*pow(2,16) +
                        secureHashInput[charsProcessed++]*pow(2,8) +
                        secureHashInput[charsProcessed++];
                }
            }
            charsProcessed += charsInMessage;
            printMessageGoingToBeProcessed(message);
            messageHashing(message);
        }
    // Print the SHA1 Message Digest
    printf("\n%lx %lx %lx %lx %lx is the SHA1 Message Digest.\n",msgDigest[0],msgDigest[1],msgDigest[2],msgDigest[3],msgDigest[4]);
}
