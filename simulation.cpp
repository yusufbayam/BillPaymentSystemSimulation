#include <iostream>
#include <fstream>
#include <cstdlib>
#include <pthread.h>
#include <vector>
#include <string>
#include <bits/stdc++.h>

using namespace std;

pthread_mutex_t mutexCust;  //mutex for customer thread
pthread_mutex_t mutexAtm;   //mutex for atm thread
int customerCount;          //number total customers
int processedBills = 0;     //number of processed bills

int cableTV = 0;            //variable for cable tv summation
int electricity = 0;        //variable for electricity summation
int gas = 0;                //variable for gas summation
int telecommunication = 0;  //variable for telecom summation
int water = 0;              //variable for water summation

ofstream outputFile;        //define output file

struct customer             //customer struct for passing to customer thread
{
    int customerNo;         //customer no
    int sleepTime;          //sleep time
    int atmNo;              //atm instance of customer
    string billType;        //bill type of customer
    int amount;             //bill amount of customer
};

vector<customer> atmQueue[10];  // vector of customers for each ATM, there is 10 ATM

void *customerRunner(void *custArgs)  // customer thread runner function
{

    customer *myData;   //customer data
    myData = (customer *)custArgs; //type cast received argument to customer 

    struct timespec ts = {0, (myData->sleepTime) * 100000L};
    nanosleep(&ts, NULL);  //customer sleep

    pthread_mutex_lock(&mutexCust);  // lock customer thread
    //cout << "customer no" << myData->customerNo << endl;
    //cout << "atm No:" << myData->atmNo <<", current size: "<< atmQueue[myData->atmNo -1].size() << endl;
    atmQueue[myData->atmNo - 1].push_back(*myData);  //push customer to the corresponding atm instance
    //cout << "atm No:" << myData->atmNo <<", after push: "<< atmQueue[(myData->atmNo) - 1].size() << endl;
    pthread_mutex_unlock(&mutexCust); // release customer mutex lock
}

void *atmRunner(void *atmArgs)      //atm thread runner function
{
    vector<customer> *c;            // takes vector of customers as parameter
    c = (vector<customer> *)atmArgs;   // type cast void parameter to vector of strings
    
    while (processedBills != customerCount)   // dont exit from thread until processed bills = processed customers
    {
        while (!c->empty())         // continue until atm queue is not empty
        {

            customer tmp = c->at(0);    // customer that will be processed
            pthread_mutex_lock(&mutexAtm);  // lock atm thread for crucial operations

            if (tmp.billType == "cableTV")  
            {
                cableTV += tmp.amount;  //  if bill type is cableTV sum it to global variable
            }
            else if (tmp.billType == "water")
            {
                water += tmp.amount;    //  if bill type is water sum it to global variable
            }
            else if (tmp.billType == "electricity")
            {
                electricity += tmp.amount;  //  if bill type is electricity sum it to global variable
            }
            else if (tmp.billType == "gas")
            {
                gas += tmp.amount;  //  if bill type is gas sum it to global variable
            }
            else if (tmp.billType == "telecommunication")
            {
                telecommunication += tmp.amount;    //  if bill type is telecom sum it to global variable
            }
            else
            {   
                cout << "no such bill type" << tmp.billType << endl;
            }
            processedBills++;       // increase number of processed bills
            outputFile << "customer" << tmp.customerNo << ","  << tmp.amount << "," << tmp.billType << endl;    // write processed customer's data tı output file
            c->erase(c->begin());    // delete customer from atm queue
            pthread_mutex_unlock(&mutexAtm);    //release atm thread mutex lock
/*             if(processedBills == customerCount){
                cout << atmQueue[tmp.atmNo -1].size() << endl;
                break;
            } */
        }
    }
}

int main(int argc, char **argv)
{
    //cout << "input file name : " << argv[1] << endl;

    string ofName = argv[1];   
    ofName.append("_log.txt");    // determine output log file name
    //cout << "ooutput file name : " << ofName << endl;

    outputFile.open(ofName);       // open output file

   
    ifstream MyFile(argv[1]);   // input file name received from argument
    string firstLine;           // first line
    getline(MyFile, firstLine);     //get first line of input file
    customerCount = stoi(firstLine);    //cast first line to integer and assing it to customer count

    pthread_t atmThreads[10];    // atm threads  array
    pthread_t customerThreads[customerCount];   // customer threads array

    struct customer custData[customerCount];  // customer data array

    //cout << "first line : " << customerCount << endl;

    for (int i = 0; i < 10; i++)
    {
        pthread_create(&atmThreads[i], NULL, atmRunner, (void *)&atmQueue[i]);  // create atm threads
    }

    string currLine;        // current line of input file
    int customerNo = 1;     // current customer no
    while (getline(MyFile, currLine)) // read file line by line
    {

        stringstream line(currLine);    // get current line in variable
        string temp;     // temp variable

        custData[customerNo - 1].customerNo = customerNo;   // customer data for current customer

        getline(line, temp, ',');
        int st = stoi(temp);
        custData[customerNo - 1].sleepTime = st;        //sleep time of current customer

        getline(line, temp, ',');
        int atm = stoi(temp);
        custData[customerNo - 1].atmNo = atm;       // atm instance of current customer

        getline(line, temp, ',');
        custData[customerNo - 1].billType = temp;   // bill type of current customer

        getline(line, temp);        
        int a = stoi(temp);
        custData[customerNo - 1].amount = a;    // amount of current customer

        pthread_create(&customerThreads[customerNo - 1], NULL, customerRunner, (void *)&custData[customerNo - 1]);  // create customer threads

        customerNo++;       // increment customer No
    }

    for (int i = 0; i < customerCount; i++)
    {
        
        void *status;
        pthread_join(customerThreads[i], &status);      // join customer threads

    }

    for (int i = 0; i < 10; i++)
    {
        void *statusAtm;
        pthread_join(atmThreads[i], &statusAtm);        // join atm threads

    }
    //cout << "all ATM threads joined" << endl;
    outputFile << "All payments are completed." << endl;        // print total of bill payments to output file
    outputFile << "CableTV Total: " << cableTV << endl;
    outputFile << "Electricity Total: " << electricity << endl;
    outputFile << "Gas Total: " << gas << endl;
    outputFile << "Telecommunication Total: " << telecommunication << endl;
    outputFile << "Water Total: " << water << endl;

    return 0;
}