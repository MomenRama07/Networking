#include<iostream>
#include<fstream>
#include<math.h>

#define MAX 10
#define INT_MAX 10000
#define LIMIT 8
#define isGreater(a,b) (a>b? return true:return false)
using namespace std;

float STEP_SIZE=0.25;
string str_TaskName[MAX];
float f_TaskPeriod[MAX],f_TaskExec[MAX],f_TaskInit[MAX];
int i_ptr=-1;
int i_state[LIMIT]={0};
float f_stack[LIMIT*2][3]={0};
int i_sp=-1;
float f_currentTask[3]={INT_MAX};
bool b_isIDL=true;

ofstream fout;
void sortStack()
{
    for(int i=0;i<i_sp;i++)
    {
            for(int j=i+1;j<=i_sp;j++)
            {
                if(f_TaskPeriod[(int)f_stack[i][0]] < f_TaskPeriod[(int)f_stack[j][0]])
                {
                        float TaskID;
                        float RemainingTime;
                        float deadline;
                        TaskID=f_stack[j][0];
                        RemainingTime=f_stack[j][1];
                        deadline=f_stack[j][02];
                       
                        f_stack[j][0]=f_stack[i][0];
                        f_stack[j][1]=f_stack[i][1];
                        f_stack[j][2]=f_stack[i][2];
                       
                        f_stack[i][0]=TaskID;
                        f_stack[i][1]=RemainingTime;
                        f_stack[i][2]=deadline;
                       
                }
            }
    }
}
bool push(float TaskID,float RemainingTime,float deadline)
{
    if(i_sp<(2*LIMIT))
    {
        i_sp++;
        f_stack[i_sp][0]=TaskID;
        f_stack[i_sp][1]=RemainingTime;
        f_stack[i_sp][2]=deadline;
        sortStack();//Sorting require
        return true;
    }
    else
    {
        cout<<"Stack Overflow";
        return false;
    }
}


bool addTask(string TaskName,float TaskInit,float TaskPeriod,float TaskExec)
{
    if(i_ptr>=MAX-1)
    {
        cout<<"\nStack Overflow\n";
        return false;
    }
    else
    {
        i_ptr++;
        str_TaskName[i_ptr]=TaskName;
        f_TaskPeriod[i_ptr]=TaskPeriod;
        f_TaskExec[i_ptr]=TaskExec;
        f_TaskInit[i_ptr]=TaskInit;
        return true;
    }
}

void sortTask()
{
    for(int i=0;i<i_ptr;i++)
    {
            for(int j=i+1;j<=i_ptr;j++)
            {
                    if(f_TaskPeriod[i]>f_TaskPeriod[j])
                    {
                        float f_tempPeriod=f_TaskPeriod[i];
                        float f_tempExec=f_TaskExec[i];
                        float f_tempInit=f_TaskInit[i];                       
                        string str_tempname=str_TaskName[i];
                       
                        f_TaskPeriod[i]=f_TaskPeriod[j];
                        f_TaskExec[i]=f_TaskExec[j];
                        f_TaskInit[i]=f_TaskInit[j];
                        str_TaskName[i]=str_TaskName[j];
                       
                        f_TaskPeriod[i]=f_tempPeriod;
                        f_TaskExec[i]=f_tempExec;
                        f_TaskInit[i]=f_tempInit;
                        str_TaskName[i]=str_tempname;                       
                    }
            }
    }
}

void printtask()
{
        cout<<"TASK\tInit\tPeriod\tExc. Time"<<endl;
        for(int i=0;i<=i_ptr;i++)
        {
            cout<<str_TaskName[i]<<"\t"<<f_TaskInit[i]<<"\t"<<f_TaskPeriod[i]<<"\t"<<f_TaskExec[i]<<endl;
        }
}

void addTaskinQueue(float i_ti)
{
    for(int i=0;i<=i_ptr;i++)
    {
        if((i_state[i]*f_TaskPeriod[i]+f_TaskInit[i])==i_ti && i_ti>=f_TaskInit[i])
        {
            if(b_isIDL)
            {
                f_currentTask[0]=i;
                f_currentTask[1]=f_TaskExec[i];
                f_currentTask[2]=f_TaskPeriod[i]+i_ti;
                i_state[i]++;
                b_isIDL=false;
            }
            else
            {
                
                push(i,f_TaskExec[i],i_ti+f_TaskPeriod[i]);
                i_state[i]++;
            }
        }
    }
}

void assignTask()
{
    if(b_isIDL) 
    {
        if(i_sp>=0) 
        {
            f_currentTask[0]=f_stack[i_sp][0];
            f_currentTask[1]=f_stack[i_sp][1];
            f_currentTask[2]=f_stack[i_sp][2];
            i_sp--;
            b_isIDL=false;
        }
    }
    else
    {
        if(i_sp>=0)
        {
            float  f_TaskID=f_currentTask[0];
            float  f_Remaining=f_currentTask[1];
            float  f_deadline=f_currentTask[2];
            f_currentTask[0]=f_stack[i_sp][0];
            f_currentTask[1]=f_stack[i_sp][1];
            f_currentTask[2]=f_stack[i_sp][2];   
            f_stack[i_sp][0]=f_TaskID;
            f_stack[i_sp][1]=f_Remaining;
            f_stack[i_sp][2]=f_deadline;           
        }

    }
}

void startScehduler()
{
    cout<<endl;
    fout<<"Time,Task"<<endl;
   
    for(float i=0;i<LIMIT;i=i+STEP_SIZE)
    {
        cout<<endl<<"SLOT: "<<i<<" =>";
        fout<<i<<",";
        addTaskinQueue(i);
        if(b_isIDL) 
        {
            if(i_sp<0)
            {
                    fout<<"IDLE."<<endl;
                    cout<<"IDLE.";
                    continue;
            }
            else
            {
                assignTask();
            }
        }
        if(i<f_currentTask[3])//Check for Deadline
        {
            cout<<"Unable to schedule Task:"<<str_TaskName[(int)f_currentTask[0]];
            break;
        }
        if(i_sp>=0 && f_currentTask[1]>f_stack[i_sp][1])
        {
            assignTask();
        }
        fout<<str_TaskName[(int)f_currentTask[0]]<<endl;
        cout<<str_TaskName[(int)f_currentTask[0]]<<"("<<i_state[(int)f_currentTask[0]]<<")" <<" Remaining Time:"<<f_currentTask[1]<<" DeadLine:"<<f_currentTask[2];
        if(!b_isIDL) 
        {
            f_currentTask[1]-=STEP_SIZE; 
        }
        if(f_currentTask[1]==0) 
        {
            b_isIDL=true;
            cout<<":: Completed";
        }   
        else
        {
            cout<<endl;
        }
           
    }
    if(i_sp>=0)
    {
        cout<<"\n*** Unable to schedule "<<i_sp+1<<" Tasks within Time-LIMIT ***\n";
    }
   
}
float getGCD(float a, float b )
{
        float ans;
        if(a<b)
        {
                b=b-a;
                a=a+b;
                b=a-b;
        }
       
        while(b!=0)
        {
                ans=b;
                b=fmod(a,b);
                a=ans;
        }
        return a;
}
bool isSchedulable()
{
    for(int i=0;i<=i_ptr;i++)
    {
        float t=f_TaskPeriod[i];
        int sum=0;
        for(int j=0;j<=i;j++)
        {
            sum=sum+ceil(t/f_TaskPeriod[j])*f_TaskExec[j];           
        }
        if(sum>t)
        {
            cout<<"\nTask "<<str_TaskName[i] <<" is not schedulable";
            return false;
        }
       
    }
    return true;
}


int main()
{
    
        addTask("I1",0,2,0.5);
        addTask("I2",1,6,2);
        addTask("I3",3,10,1.75);
        fout.open("output.csv");
        float start=f_TaskExec[0];
                for(int i=0;i<=i_ptr;i++)
                {
                        start=getGCD(start,f_TaskExec[i]);
                }
                cout<<start<<endl;
                STEP_SIZE=start;       
               
        sortTask();
        printtask();
       
        if(isSchedulable())
        {
            startScehduler();   
        }
        else
        {
            cout<<"\n****Tasks are not schedulable***\n";
        }
   
    addTask("I1",0,100,20);
    //addTask("I2",0,150,30);
    //addTask("I3",0,210,80);
    //addTask("I4",0,400,100);
   
    return 0;
}
