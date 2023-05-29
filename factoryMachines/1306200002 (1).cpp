 // Zekiye Özdemir
 // Bilgisayar Mühendisliði
 // 1306200002
 // Tarih: 06.06.2021
 // Geliþtirme Ortamý: Visual Studio2019











// base required libraries
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <string>

using namespace std;

//declaration of operation class
class Operation
{
    //public field
    public:
        int opCode;
        int amount;

    
    static vector<Operation> parse(string const& file_path){
        vector<Operation> vec {};
        
        //file operations
        ifstream file {};
        file.open(file_path);   //open file 
        
        Operation op;            
        string str;
        while (getline(file, str))
        {
            int colonFound = str.find(";");     //split semicolon
            string opCode = str.substr(0,colonFound);
            string amount = str.substr(colonFound+1,str.size()-1); 

            op.amount = stoi(amount);
            op.opCode = stoi(opCode);
            vec .push_back(op);
        }
        return vec;
    }
};

//declaration of SetupDuration class
class SetupDuration
{
    //public fields
    public:
        int opCodeOne;
        int opCodeTwo;
        int transferDuration;

 
    static vector<SetupDuration> parse(string file_path)  {
        vector<SetupDuration> vec {};

        ifstream file {};
        file.open(file_path);   //file open
         
        string str;
        SetupDuration sd;
            
        while (getline(file, str))  
        {
            string strCpy = str;

            int colonFound = strCpy.find(";");  //find semicolon
            string opCodeOne = strCpy.substr(0,colonFound);
                
            strCpy.erase(0,colonFound+1);
            colonFound = strCpy.find(";");
            string opCodeTwo = strCpy.substr(0,colonFound);
            string transferDuration  = strCpy.substr(colonFound+1,str.size()-1); 
                
            sd.opCodeOne = stoi(opCodeOne);
            sd.opCodeTwo = stoi(opCodeTwo);
            sd.transferDuration = stoi(transferDuration);

            vec.push_back(sd);
        }
        return vec;
    }
};

//declaration of Orders class
class Orders
{
    public: 
        int orderCode;
        int amountOfWork;
        int operationCode;
        int deadline;

      
        static vector<Orders> parse(string const& file_path){
            vector<Orders> vec {};
            
            ifstream file {};
            file.open(file_path);
            
            Orders  od;            
            string str;
    
  
            while (getline(file, str))
            {
                string strCpy = str;

                int colonFound = strCpy.find(";");
                string orderCode = strCpy.substr(0,colonFound);
                    
                strCpy.erase(0,colonFound+1);
                colonFound = strCpy.find(";");
                string amountOfWork = strCpy.substr(0,colonFound);

                strCpy.erase(0,colonFound+1);
                colonFound = strCpy.find(";");
                string OperationCode = strCpy.substr(0,colonFound);

                string Deadline  = strCpy.substr(colonFound+1,str.size()-1);

                od.operationCode = stoi(OperationCode);
                od.orderCode = stoi(orderCode);
                od.deadline = stoi(Deadline);
                od.amountOfWork = stoi(amountOfWork);

                vec.push_back(od);
            } 

            return vec;
        }
};

//declaration of Schedule class
class Schedule
{
    public:
        int scheduleTime;
        int operationCode;
        int orderCode;
        int amountOfWork;
        int setupOverhead;

        static void bubbleSortWrtDeadline(vector<Orders>& vec)
        {
            std::sort (vec.begin(), vec.end(),
            [](Orders a, Orders b){return b.deadline > a.deadline;}
            );
        };

        static void bubbleSortWrtOpCode(vector<Orders>& vec, Orders upcomingOrder, bool lastOrders)
        {
            std::sort (vec.begin(), vec.end(),
            [](Orders a, Orders b){return b.operationCode > a.operationCode;}
            );

            //swaps two and three
            int opTwoPos, opThreePos;
            for (int i = 0 ; i < vec.size() ; i++)
            {
                if(vec[i].operationCode == 2)
                {
                    opTwoPos = i;
                    for (int j = i+1; j < vec.size() ; j++)
                    {
                        if (vec[j].operationCode == 3) 
                        {
                            opThreePos = j;
                            iter_swap(vec.begin() + opTwoPos, vec.begin() + opThreePos);
                        }
                    } 
                } 
            }

            if(!lastOrders)
            {
                for (int i = 0 ; i < vec.size() ; i++)
                {
                    if(vec[i].operationCode == upcomingOrder.operationCode)
                    {
                        iter_swap(vec.begin() + i, vec.begin() + vec.size() - 1);
                    }
                }
            }
        };

        static vector<Orders> rearrange(vector<Orders> vec)
        {
            vector<Orders> temp;
            vector<Orders> returnVec;
            int minDeadline = vec[0].deadline;
            
            for(int i = 0 ; i < vec.size() ; i++)
            {
                if(vec[i].deadline > minDeadline)
                { 
                    bubbleSortWrtOpCode(temp,vec[i],false);
                    returnVec.insert(returnVec.end(), temp.begin(), temp.end());

                    minDeadline = vec[i].deadline;
                    temp.clear();           
                    temp.push_back(vec[i]);
                }
                else if(i == vec.size() - 1)
                {
                    temp.push_back(vec[i]);
                    minDeadline = vec[i].deadline;
                    
                    bubbleSortWrtOpCode(temp,vec[i],true);
                    returnVec.insert(returnVec.end(), temp.begin(), temp.end());
                    
                    temp.clear();
                }
                else
                {
                    temp.push_back(vec[i]);
                }
            }

            return returnVec;
        };

        static int returnOpSpeed(vector<Operation> operations, int operationCode)
        {
            for (int i = 0 ; i < operations.size() ; i++)
            {
                if (operations[i].opCode == operationCode) return operations[i].amount;              
            }
        };

        static int returnSetupDuration(vector<SetupDuration> durations, int opOne, int opTwo)
       {
            for (int i = 0 ; i < durations.size(); i++)
            {
                if (durations[i].opCodeOne == opOne && durations[i].opCodeTwo == opTwo
                   || durations[i].opCodeOne == opTwo && durations[i].opCodeTwo == opOne)
                {
                    return durations[i].transferDuration;
                }
            }
        };

        static vector<Schedule> schedulize(vector<Operation> operations, vector<Orders> orders, vector<SetupDuration> durations)
        {
            int scheduleTimeStart = 0, workTime = 0, oldOperation = -1, prevWorkTime = -1, prevWorkTimeHolder = 0;
            vector<Schedule> schedules;

            Schedule sh;
            for (int i  = 0 ; i < orders.size() ; i++)
            {   
                if (oldOperation != -1)
                {
                    int newOperation = orders[i].operationCode;                   
                    sh.setupOverhead = returnSetupDuration(durations,newOperation,oldOperation);                 

                    oldOperation = orders[i].operationCode;
                }
                else 
                {
                    oldOperation = orders[i].operationCode;
                    sh.setupOverhead = 0;  
                }                
                
                if (prevWorkTime != -1)
                {     
                    prevWorkTimeHolder = prevWorkTime;              
                    sh.scheduleTime = prevWorkTime + sh.setupOverhead;
                    workTime = ceil(double(orders[i].amountOfWork) / double(returnOpSpeed(operations,orders[i].operationCode)));
                    prevWorkTime = sh.scheduleTime + workTime;                    
                }
                else 
                {
                    workTime = ceil(double(orders[i].amountOfWork) / double(returnOpSpeed(operations,orders[i].operationCode)));
                    sh.scheduleTime = 0;  
                    prevWorkTime = workTime;
                } 

                sh.operationCode = orders[i].operationCode;
                sh.orderCode = orders[i].orderCode;
                sh.amountOfWork = orders[i].amountOfWork;

                if (sh.scheduleTime <= orders[i].deadline)
                {
                    schedules.push_back(sh);                    
                }
                else 
                {
                    prevWorkTime = prevWorkTimeHolder;
                    continue;
                }
                
            }

            return schedules;
        }

        static vector<Schedule> makeSchedule(vector<Operation> operations, vector<Orders> orders, vector<SetupDuration> durations)
        {
            bubbleSortWrtDeadline(orders);
            vector<Orders> rearranged = rearrange(orders);

            return schedulize(operations, rearranged, durations);           
        };

        static void printFile(vector<Schedule> schedules)
        {
            ofstream file("./Schedule.txt");

            for(int i = 0; i < schedules.size(); i++)
            {
                file << schedules[i].scheduleTime << ";" << schedules[i].operationCode << ";" << schedules[i].orderCode << ";" << schedules[i].amountOfWork << ";" << schedules[i].setupOverhead << "\n";
            }
        }
};
//FileParser class
template <class T>
class FileParser
{    
    public:        
        static vector<T> parseFile(string const& filePath)
        {
            vector<T> vec = T::parse(filePath);
            return vec;
        };      
};

//main function 
int main()
{
    vector<Operation> operations = FileParser<Operation>::parseFile("./Operations.txt");
    vector<SetupDuration> setupDur = FileParser<SetupDuration>::parseFile("./SetupDuration.txt");
    vector<Orders> orders = FileParser<Orders>::parseFile("./Orders.txt");
    vector<Schedule> schedules = Schedule::makeSchedule(operations,orders,setupDur);
    Schedule::printFile(schedules);
}