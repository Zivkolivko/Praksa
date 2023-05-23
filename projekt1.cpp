/*      
* 
*       Trainee Embedded Engineer - Patrik Zivic
*
*/
#include <iostream>
#include <thread>
#include <stdio.h>
#include <chrono>

using namespace std;

bool b_state[3] = { 0, 0, 0 };      //states {A, B, C}
bool flag = 1;                      //if 1, allows turning on(1) states      
char c;

/******************************************************************************************
**
**Function          taskCheckState
**
**Description       Every 5 seconds puts a log into the console about current state values.
*******************************************************************************************/
void taskCheckState() {

    while (1) {
        std::this_thread::sleep_for(std::chrono::seconds(5));   //wait 5 seconds
        for (int i = 0; i < 3; i++) {
            if (b_state[i])
                printf("%c%s\n", i + 65, " upaljeno");
            else
                printf("%c%s\n", i + 65, " ugaseno");
        }
        printf(".......\n");
    }
    
}

/**************************************************************************************************************************
**
**Function          taskCheckInput
**
**Description       If all states are turned on(1), wait for input character(s) and proceed with the appropriate procedure.
**                  Input can be a series of characters from which A, B and C are filtered.
**                  It allows a series of procedures one after another with 1 input.
***************************************************************************************************************************/
void taskCheckInput() {
    
    while (1) {
        flag = 1;                                   //allow state changes to 1 (turned on)
        if (b_state[0] && b_state[1] && b_state[2])     
            scanf_s(" %c", &c);                         
        else
            c = ' ';

        //65 == A, 66 == B, 67 == C
        if (c > 64 && c < 68) {
            flag = 0;                               //don't allow state changes to 1 (turned on)
            printf("Gasenje %c\n.......\n", c);
            
                //wait 2 seconds to turn off
                std::this_thread::sleep_for(std::chrono::seconds(2));
                b_state[c - 65] = 0;
            
            //if c == A
            if (c - 65 == 0) {
                    
                    //wait 2 seconds to turn off B
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    b_state[1] = 0;
                
                    //wait 2 seconds to turn off C
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    b_state[2] = 0;
                
                
            }

            //if c == B
            else if (c - 65 == 1) {
                
                    //wait 2 seconds to turn off C
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    b_state[2] = 0;
                
                        
            }
            
        }

    }
    
}

/**********************************************************************************************************************
**
**Function          taskChangeState
**
**Description       If allowed (flag == 1), change state values from turned off(0) to turned on(1) in order from A to C
***********************************************************************************************************************/
void taskChangeState() {
    while (1) {
        if (flag) {
            for (int i = 0; i < 3; i++) {
                if (!b_state[i] && flag) { 

                    //wait 2 seconds to turn on 
                    std::this_thread::sleep_for(std::chrono::seconds(2));
                    b_state[i] = 1;
                    
                }

            }
        }
        
    }
}

int main()
{   
        thread t1(taskCheckState);      //thread1: every 5 seconds shows a log in the console about values of current states
        thread t2(taskChangeState);     //thread2: if changing is allowed (flag == 1), changes 0(turned off) states into 1(turned on) in order from A to C
        thread t3(taskCheckInput);      //thread3: if all states are 1(turned on), wait for input character(s) and proceed with appropriate procedure
    
     
        t2.join();
        t3.join();
        t1.join();

    return 0;
}
