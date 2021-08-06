#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

#define Day 7
#define Period 5
vector <string> Col0,OutlierTime,Col0Soft;//create vector time 
vector <float> Col1,OutlierValues,Col1Soft;//creaate vector values

//Function read csv file 
void Read_File(string NameFile)
{
    vector <string>row;
    fstream f;
	string word, line;
    Col1.clear();Col0.clear();
	f.open(NameFile,ios::in);
    getline(f, line);//Read subject line
    do              
    {
        row.clear();                    
        getline(f, line);//Read 1 line in csv file               
        stringstream s(line);           
        while(getline(s, word, ',')) //Separate Time and Value
           row.push_back(word);
		Col0.push_back(row[0]);Col0Soft.push_back(row[0]);
		Col1.push_back(stof(row[1]));Col1Soft.push_back(stof(row[1]));
    }
    while(!f.eof()); //check end file 
    Col1.pop_back();Col0.pop_back();
    Col1Soft.pop_back();Col0Soft.pop_back();
    f.close();//close file
}

//Function Insertion Soft
void Insertion_Soft()
{
    int i,j,n = Col1Soft.size();
    float key1; 
    string key2;
    for (i=1;i<n-1;i++)
    {
        key1 = Col1Soft[i]; key2 = Col0Soft[i];
        j=i-1;
        while((j>=0)&&(Col1Soft[j]<key1))
        {
            Col1Soft[j+1]=Col1Soft[j];Col0Soft[j+1]=Col0Soft[j];
            j=j-1;
        }
        Col1Soft[j+1]=key1; Col0Soft[j+1]=key2;
    }
}

//Function Selction Soft
void Selection_Soft()
{
    int p;              
    float max;          
    for (int i=0;i<Col1Soft.size();i++)
    {
        max = Col1Soft[i];
        //find max values in unsofted vector
        for (int j=i;j<Col1Soft.size();j++)
        {
            
            if (max<Col1Soft[j]) 
            {
                max = Col1Soft[j];//set max = value if higher than max 
                p = j;
            }
        }
        //set value in right position
        Col1Soft[p]=Col1Soft[i];Col1Soft[i]=max;
        //set time vector follow values vector 
        Col0Soft[i].swap(Col0Soft[p]);
    }   
}

//Function Create Data Task 1
void Create_Data()
{
    int SetDay = Day,SetPeriod = Period;
    char check;
    cout << "If you want to define enter'n', import enter other character";//check user using default value or customize
	cin  >>  check;
	if (check != 'n') 
    {
        //set customize extraction cycle and time measuremen
        do 
        {   
            cout << "Enter sample time measurement (day): ";
		    cin >> SetDay;
            cout << "Enter extraction cycle (minute): ";
		    cin >> SetPeriod;
        } while ((SetDay <= 0)||(SetPeriod<=0));
    }
    fstream f;		
	f.open("Pham_Thi_Trang_temperature_data.csv", ios::out);//crate data csv file mode write
	srand(time(0));
	time_t Now = time(0);
	int NumData = SetDay*24*60/SetPeriod;//caculate number of data point
	f<<"Time,Values"<<endl;//write subject line
	for (int i = 0; i < NumData; i++)
	{	
		tm* ltm = localtime(&Now);//get time now
        //write time into file
		f << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon << "-" << ltm->tm_mday << " " << 1 + ltm->tm_hour << ":" << 1 + ltm->tm_min << ":" << 1 + ltm->tm_sec;
		//creater random tempratue among -25 and 125
        float RanData = (rand() % 150) - 25;
		RanData = RanData + rand() % 6 * 0.2;
		f <<","<< RanData <<endl;//write random tempratue into file
		Now = Now + SetPeriod * 60; //add time 
	}
	f.close();//close file
}

//Function outlier data
void Outlier_Data()
{
    fstream f2;
    Read_File("Pham_Thi_Trang_temperature_data.csv");//get vector time and values from data file
    f2.open("Pham_Thi_Trang_outlier_data.csv",ios::out);//create outlier data file mode  write
    for (int i=0;i<Col1.size();i++)
   {    
       if((Col1[i]<15||Col1[i]>42))//filter data under 15 and hgher than 42
       {
        OutlierTime.push_back(Col0[i]);
        OutlierValues.push_back(Col1[i]);
        //delete wrong data
        Col0.erase(Col0.begin()+i);
        Col1.erase(Col1.begin()+i);
        i--;
       }
   }
   //write data into file
    f2<< "invalid_data_points,"<<OutlierValues.size()<<endl;
    f2<<"time,values"<<endl;
    for(int i=0;i<OutlierValues.size();i++)
    f2<<OutlierTime[i]<<","<<OutlierValues[i]<<endl;
    f2.close();
}

//Function Summary Data
void Summary_Data()
{
    fstream f3;
    f3.open("Pham_Thi_Trang_data_summary.csv",ios::out);//create summary data file
    vector<string> Time;
    int count  = 0; //number of values each day
    float max = -25, min =125,sum=0;//max, min value each day and sum value each day
    string CheckDay = " ",word; //check day is previous date
    f3 << "Parameters"<<","<<"Time"<<","<<"Values"<<endl; //subject line 
    for(int i=0;i<Col1.size();i++)
    {
        Time.clear();
        stringstream s(Col0[i]);
        while(getline(s,word,' '))//separate Date and Time
            Time.push_back(word);//time[0] is Date and time [1] is Time
        if ((CheckDay == " " ) || (CheckDay == Time[0] ))//compare current date and previous date
        {
            count++; //number of value +1
            if (Col1[i]>max) max=Col1[i]; //find max value
            if (Col1[i]<min) min=Col1[i]; //find min value
            CheckDay = Time[0]; //Previous date
            sum = sum + Col1[i]; //add current value
        }
        else //current date and previous date different
        {
            sum = sum/count; //caculate medium value
            f3<<"Temperature_max"<<","<<CheckDay<<","<<max<<endl;//write max value 
            f3<<"Temperature_min"<<","<<CheckDay<<","<<min<<endl; //write min value
            f3<<"Mean"<<","<<CheckDay<<","<<sum<<endl;//write everage value
            CheckDay = Time[0];//set new previous date
            sum = Col1[i]; max = Col1[i];min=Col1[i];count = 0;
        }        
    }
    f3.close();//close file
}

//Function Static Data
void Static_Data()
{
    fstream f4;
    int a[7]; for(int i=0;i<7;i++)a[i]=0;//create file
    f4.open("Pham_Thi_Trang_data_statistics.csv",ios::out);
    for (int i=0;i<Col1.size();i++)
    {
        //caculate number of data
        if ((Col1[i]>=15)&&(Col1[i]<18)) a[0]++;
        else if ((Col1[i]>=18)&&(Col1[i]<23)) a[1]++;
        else if ((Col1[i]>=23)&&(Col1[i]<28)) a[2]++;
        else if ((Col1[i]>=28)&&(Col1[i]<32)) a[3]++;
        else if ((Col1[i]>=32)&&(Col1[i]<35)) a[4]++;
        else if ((Col1[i]>=35)&&(Col1[i]<38)) a[5]++;
        else if ((Col1[i]>=38)&&(Col1[i]<=42)) a[6]++;
    }
    //write number of data point
    f4<<"ranger,quantity"<<endl;
    f4<<"15 to 18,"<<a[0]<<endl;
    f4<<"18 to 23,"<<a[1]<<endl;
    f4<<"23 to 28,"<<a[2]<<endl;
    f4<<"28 to 32,"<<a[3]<<endl;
    f4<<"32 to 35,"<<a[4]<<endl;
    f4<<"35 to 38,"<<a[5]<<endl;
    f4<<"38 to 42,"<<a[6]<<endl;
    f4.close();//close file
}

//Function Sort Data
void Sort_Data()
{
    char x;
    fstream f5;
    f5.open("Pham_Thi_Trang_data_sorted.csv",ios::out);//create file
    cout<<"Please choose a mothod to soft:\n1.Insertion Soft\n2.Selection Soft\n ";//choose method sort
    cin>>x;
    switch (x)
    {
        case '1': Insertion_Soft();
        default: Selection_Soft();   
    }
    //cout data sorted
    f5<<"time,values"<<endl;
    for (int i=0;i<Col1Soft.size();i++)
    f5<<Col0Soft[i]<<","<<Col1Soft[i]<<endl;
    f5.close();
}

// Main Program
int main()
{
    Create_Data();
    cout <<"Complete to create file Temprature\n\n";
    Outlier_Data(); 
    cout <<"Complete to create file Outlier\n";
    Summary_Data();  
    cout <<"Complete to create file Summary\n";
    Static_Data();    
    cout <<"Complete to create file Static\n";
    Sort_Data();
    cout <<"Complete to create file Sort\n";
	return 0;
}
