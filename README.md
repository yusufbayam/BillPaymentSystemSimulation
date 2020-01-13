# BillPaymentSystemSimulation
Multi-threaded application for simulating a bill payment system using C++.

This application simulates event of customers which will come to different ATMs and pay their bills. 
İnput file consists of n lines where each line is bill information corresponding to a customer. First line of input file is number of customers. After that each line has information of sleep time, ATM number, bill type, bill amount of the customer seperated by commas(,).

Output of program is a log file where each line has payment info for one customer. Each line has customer number, bill amount and bill type seperated by commas. After that there is information of total payment collected for each bill type.

How to run :
1. Go to directory of application files.
2. Run make command on terminal.
3. Run application by "./simualation inputfile"

Output file is named inputfile_log.txt
