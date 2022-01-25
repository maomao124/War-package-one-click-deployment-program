#include<iostream>
#include<stdio.h>
#include<string.h>
#include <Windows.h>
#include<time.h>
//#include <graphics.h>
#include <conio.h>
#include<algorithm>
#include<stdlib.h>
#include <random>
#include <string>
#include <fstream>
#include <ddraw.h>
#include<iomanip>
#include <Psapi.h>
#include<vector>
#include <direct.h>
#include<io.h>
#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include <audioclient.h>
#include <sapi.h>                                             // 语音头文件
#include <sphelper.h>                                     // 语音识别头文件
#include <thread>                                           //多线程
#include"resource1.h"
                                                                        //vs2019自定义控制台代码模板-单文件结构 by mao
                                                                        //作者QQ1296193245
using namespace std;
#pragma warning(disable : 4996)
#pragma comment (lib,"ddraw.lib")
#pragma comment(lib,"sapi.lib")
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"Winmm.lib")                     // 引用 Windows Multimedia API
#pragma intrinsic(__rdtsc)
unsigned __int64 t1, t2, t3, t4;
double run_time;                                                   //微秒级算法时间计时器
LARGE_INTEGER time_start;	                                //开始时间
LARGE_INTEGER time_over;	                                //结束时间
double dqFreq;		                                               //计时器频率
LARGE_INTEGER f;	                                               //计时器频率
double tt;
long double totaltime;

inline void runstart()
{
	QueryPerformanceFrequency(&f);
	dqFreq = (double)f.QuadPart;
	t1 = __rdtsc();
	QueryPerformanceCounter(&time_start);	       //计时开始
}
inline void runend()
{
	QueryPerformanceCounter(&time_over);	        //计时结束
	run_time = 1000 * (time_over.QuadPart - time_start.QuadPart) / dqFreq;
	t2 = __rdtsc();
	t3 = t2 - t1;
}
void ShowProcessMemoryUsageInfo()
{
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	float memoryUsage_M = pmc.WorkingSetSize / (1024.0 * 1024.0);
	float memoryUsage_K = pmc.WorkingSetSize / 1024.0;
	std::cout << std::fixed << setprecision(0) << "程序内存使用率：" << memoryUsage_K << " KB = " << setprecision(2) << memoryUsage_M << " MB" << std::endl;
}
__int64 CompareFileTime(FILETIME time1, FILETIME time2)//转换为整数时间
{
	__int64 a = static_cast<long long>(time1.dwHighDateTime) << 32 | time1.dwLowDateTime;
	__int64 b = static_cast<long long>(time2.dwHighDateTime) << 32 | time2.dwLowDateTime;
	return   (b - a);
}
void rundisplay()
{
	cout << endl;
	cout << "-------------------------------------------------------" << endl;
	if (t3 < 10000)
	{
		cout << "CPU脉冲数：" << (t3) << endl;
	}
	else if (t3 < 100000000 && t3 >= 10000)
	{
		tt = t3;
		cout << "CPU脉冲数：" << (tt) / 10000 << "万" << endl;
	}
	else if (t3 > 100000000000)
	{
		t4 = t3;
		t4 = t3 / 10000000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10 << "亿" << endl;
	}
	else
	{
		t4 = t3;
		t4 = t3 / 10000;
		tt = t4;
		cout << "CPU脉冲数：" << (tt) / 10000 << "亿" << endl;
	}
	if (run_time > 10000)
	{
		cout << "算法运行时间：" << run_time / 1000 << "秒" << endl;
	}
	else if (run_time < 1)
	{
		cout << "算法运行时间：" << run_time * 1000 << "微秒" << endl;
	}
	else
	{
		cout << "算法运行时间：" << run_time << "毫秒" << endl;
	}
	totaltime = clock();
	cout << "程序运行时间：" << totaltime / 1000 << "秒" << endl;
	if (run_time > 0.4)
	{
		double frequency = t3 / run_time / 1000;
		cout << "CPU频率：" << frequency << "MHZ" << endl;
	}
	ShowProcessMemoryUsageInfo();
	long lRet;
	HKEY hKey;
	TCHAR tchData[64];
	DWORD dwSize;
	lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Hardware\\Description\\System\\CentralProcessor\\0", 0, KEY_QUERY_VALUE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		dwSize = sizeof(tchData);
		lRet = RegQueryValueEx(hKey, L"ProcessorNameString", NULL, NULL, (LPBYTE)tchData, &dwSize);
		if (lRet == ERROR_SUCCESS)
		{
			cout << "CPU 名字：";
			wcout << tchData << endl;
		}
		else
		{
			cout << "CPU 名字：";
			wcout << L"未知" << endl;
		}
	}
	else
	{
		cout << "CPU 名字：";
		wcout << L"未知" << endl;
	}
	RegCloseKey(hKey);
	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);
	GlobalMemoryStatusEx(&statex);
	double totalphy = statex.ullTotalPhys / 1024 / 1024;
	totalphy = totalphy / 1024;
	cout << "总物理内存：    " << setprecision(4) << left << setw(7) << totalphy << " GB" << endl;
	double totalvir = statex.ullTotalPageFile / 1024 / 1024;
	totalvir = totalvir / 1024;
	cout << "总虚拟内存：    " << setw(7) << totalvir - totalphy << " GB" << endl;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD coordScreen = { 0, 0 }; //光标位置
	CONSOLE_SCREEN_BUFFER_INFO csbi{};
	if (GetConsoleScreenBufferInfo(hConsole, &csbi))
	{
		//printf("光标坐标:(%d,%d)\n", csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y);
	}
	HANDLE   hStdout;
	//   光标位置   
	COORD     cursorPos;
	//   标准输出句柄   
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	HANDLE hEvent;
	BOOL res;
	FILETIME preidleTime;
	FILETIME prekernelTime;
	FILETIME preuserTime;
	FILETIME idleTime;
	FILETIME kernelTime;
	FILETIME userTime;
	res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
	preidleTime = idleTime;
	prekernelTime = kernelTime;
	preuserTime = userTime;
	hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);// 初始值为 nonsignaled ，并且每次触发后自动设置为nonsignaled
	double freephy = statex.ullAvailPhys / 1024 / 1024;
	freephy = freephy / 1024;
	double usephy = (totalphy - freephy) * 100.0 / totalphy;
	cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
	double freetotal = statex.ullAvailPageFile / 1024 / 1024;
	freetotal = freetotal / 1024;
	cout << "总空闲内存：    " << setprecision(3) << freetotal << " GB" << endl;
	double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
	cout << "虚拟内存使用量：" << setprecision(3) << freevir << " GB" << endl;
	double usevir = freevir * 100 / (totalvir - totalphy);
	cout << "虚拟内存使用率：";
	printf("%.2f%%", usevir);
	cout << endl;
	cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << 0.00 << "%" << endl;
	cout << "CPU空闲率：    " << setw(6) << 100.0<< "%" << endl;
	cout << "-------------------------------------------------------" << endl;
	cout << "按任意键退出程序" << endl;
	while (!_kbhit())
	{
		GlobalMemoryStatusEx(&statex);
		WaitForSingleObject(hEvent, 1000);
		res = GetSystemTimes(&idleTime, &kernelTime, &userTime);
		int idle = CompareFileTime(preidleTime, idleTime);
		int kernel = CompareFileTime(prekernelTime, kernelTime);
		int user = CompareFileTime(preuserTime, userTime);
		float cpu = (kernel + user - idle) * 100.0 / (kernel + user);
		float cpuidle = (idle) * 100.0 / (kernel + user);
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 0;
		SetConsoleCursorPosition(hStdout, cursorPos);
		//cout << "内存使用率：" << statex.dwMemoryLoad << "%" << endl;
		double freephy = statex.ullAvailPhys / 1024 / 1024;
		freephy = freephy / 1024;
		double usephy = (totalphy - freephy) * 100.0 / totalphy;
		cout << "内存使用率：    " << setprecision(3) << usephy << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 1;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freetotal = statex.ullAvailPageFile / 1024 / 1024;
		freetotal = freetotal / 1024;
		cout << "总空闲内存：    " << setprecision(3) << freetotal <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 2;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double freevir = (totalvir - freetotal) - (totalphy * statex.dwMemoryLoad / 100.0);
		cout << "虚拟内存使用量：" << setprecision(3) << freevir <<" GB" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 3;
		SetConsoleCursorPosition(hStdout, cursorPos);
		double usevir = freevir * 100 / (totalvir - totalphy);
		cout << "虚拟内存使用率：";
		printf("%.2f%%", usevir);
		cout << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 4;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU利用率：    " << right << setprecision(2) << setw(6) << cpu << "%" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 5;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "CPU空闲率：    " << setw(6) << cpuidle << "%"<<endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 6;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "-------------------------------------------------------" << endl;
		cursorPos.X = 0;
		cursorPos.Y = csbi.dwCursorPosition.Y + 7;
		SetConsoleCursorPosition(hStdout, cursorPos);
		cout << "按任意键退出程序" << endl;
		preidleTime = idleTime;
		prekernelTime = kernelTime;
		preuserTime = userTime;
	}
	cout << endl;
	
}


//**************************************************************//************************非主函数

BOOL SetConsoleColor(WORD wAttributes)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	return SetConsoleTextAttribute(hConsole, wAttributes);
}

string solve(string& s1, string& s2)
{
	if (s2.size() == 0)
		return s1;
	for (int i = 0; i < (int)(s1.size() - s2.size() + 1);)
	{
		if (s1.substr(i, s2.size()) == s2)
		{
			s1.erase(i, s2.size());
		}
		else
			i++;
	}
	return s1;
}


#define FOREGROUND_BLUE      0x0001 // text color contains blue.文本颜色包含蓝色。
#define FOREGROUND_GREEN     0x0002 // text color contains green.
#define FOREGROUND_RED       0x0004 // text color contains red.
#define FOREGROUND_INTENSITY 0x0008 // text color is intensified.文本颜色增强。
#define BACKGROUND_BLUE      0x0010 // background color contains blue.//背景色包含蓝色。
#define BACKGROUND_GREEN     0x0020 // background color contains green.
#define BACKGROUND_RED       0x0040 // background color contains red.
#define BACKGROUND_INTENSITY 0x0080 // background color is intensified.
#define COMMON_LVB_LEADING_BYTE    0x0100 // Leading Byte of DBCS
#define COMMON_LVB_TRAILING_BYTE   0x0200 // Trailing Byte of DBCS
#define COMMON_LVB_GRID_HORIZONTAL 0x0400 // DBCS: Grid attribute: top horizontal.
#define COMMON_LVB_GRID_LVERTICAL  0x0800 // DBCS: Grid attribute: left vertical.
#define COMMON_LVB_GRID_RVERTICAL  0x1000 // DBCS: Grid attribute: right vertical.
#define COMMON_LVB_REVERSE_VIDEO   0x4000 // DBCS: Reverse fore/back ground attribute.
#define COMMON_LVB_UNDERSCORE      0x8000 // DBCS: Underscore.
#define COMMON_LVB_SBCSDBCS        0x0300 // SBCS or DBCS flag.




string exepath = "";
string tomcatPath = "";
string post = "";
string index = "";
string browserPath = "";
string logPath = "";



//加载配置文件
void loadconfig()
{
	string path = exepath + "config.ini";
	//cout << path << endl;
	ifstream config(path, ios::in);
	if (!config)
	{
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "\a未找到配置文件！！！" << endl;
		_getch();
		exit(0);
	}
	getline(config, tomcatPath);
	getline(config, post);
	getline(config, index);
	getline(config, browserPath);
	getline(config, logPath);
	config.close();
}

void displayConfig()
{
	cout << "  Tomcat路径：   " <<tomcatPath << endl;
	cout << "  端口号：       " << post << endl;
	cout << "  默认页面：     " << index << endl;
	cout << "  浏览器路径：   " << browserPath << endl;
	//cout << "   日志文件路径：" << logPath << endl;
}

//**************************************************************//************************

int main(int args, char* argv[])
{

	//*************************************************************************



	//*************************************************************************
	//runstart();
	//*************************计时开始***************************************
	
	exepath = argv[0];
	string exename = "war包一键部署程序.exe";
	solve(exepath, exename);

	//cout << exepath << endl;

	SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << setw(40) << "war包一键部署程序v1.0   by mao" << endl;
	SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
	loadconfig();
	displayConfig();
	if (args >= 2)
	{
	
	}
	char buffer[MAX_PATH];
	getcwd(buffer, MAX_PATH);
	cout << left << setw(30) << "  当前工作目录为：" << buffer << endl;
	string path = buffer;
	if (args == 2)
	{
		path = path + "\\";
		string pathfile = argv[1];
		solve(pathfile, path);
		if (pathfile.find(".war") >= pathfile.length())
		{
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "文件后缀名不正确\a！！！    正确的后缀名应为.war" << endl;
			cout << "按任意键退出程序......." << endl;
			_getch();
			exit(0);
		}


		cout << "  即将执行的war文件的名称：";
		//cout << files[0].c_str() << endl;
		cout << pathfile << endl;

		cout << "------------------------------------------------" << endl;


		SetConsoleColor( FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		
		cout << "拷贝war包到webapps目录下..." << endl;
		string war = argv[1];
		string cmd = "copy \"" + war + "\" \"" + tomcatPath + "\\webapps\"";
		//cout << cmd << endl;
		system(cmd.c_str());

		cout << "写入bat文件..." << endl;
		string batpath = exepath + "run.bat";
		ofstream writebat(batpath,ios::out);
		writebat << "cd /d \"" + tomcatPath + "\\bin\"\n";
		writebat << "start /min \"run\" \"" + tomcatPath + "\\bin\\startup.bat\"";
		writebat.close();


		cout << "启动Tomcat..." << endl;

		//cmd = "cd " + path;
		//cout << cmd << endl;
		//system(cmd.c_str());

		cmd = "start  \"\" \""+exepath+"run.bat\"";
		//cout << cmd << endl;
		system(cmd.c_str());

		cout << "等待启动服务器..." << endl;

		Sleep(3000);
		cout << "清除多余窗口..." << endl;
		cmd = "taskkill /f /im cmd.exe";
		system(cmd.c_str());

		cout << "启动浏览器..." << endl;
		string name = pathfile;
		string n = ".war";
		solve(name, n);
		string url = "http://localhost:" + post + "/" + name + "/" + index;
		cmd = "start \"" + browserPath + "\" \"" + url+"\"";
		system(cmd.c_str());

		cout << "完成！" << endl;

		ofstream out(logPath, ios::app);
		SYSTEMTIME sys;                                    //获取系统时间
		GetLocalTime(&sys);
		out << "日期：";
		out << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "\t";
		out << "时间：";
		out << setfill('0') << setw(2) << sys.wHour << ":" << setfill('0') << setw(2) << sys.wMinute << ":" << setfill('0') << setw(2) << sys.wSecond << "\t";
		out << "启动的war文件的路径：";
		out << argv[1] << endl;
		out.close();

		cout << "按任意键关闭服务器" << endl;
		_getch();
		cout << "再次按下任意键来关闭程序" << endl;
		_getch();

		cout << "关闭进程..." << endl;
		cmd = "taskkill /f /im java.exe";
		system(cmd.c_str());
		cout << "删除webapps目录下的对应war包..." << endl;
		string deletepath = tomcatPath + "\\webapps\\" + pathfile;
		//cout << deletepath << endl;
		remove(deletepath.c_str());
		if (name!="")
		{
			cout << "删除webapps目录下的对应的文件夹..." << endl;
			cmd = "rd /s /q \"" + tomcatPath + "\\webapps\\" + name + "\"";
			system(cmd.c_str());
		}
		cout << "完成！" << endl;
		cout << "按任意键退出程序" << endl;

		SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "------------------------------------------------" << endl;

	}

	else if (args > 2)
	{
		path = path + "\\";
		cout << "  传入了多个war包文件的路径，请选择要运行的文件" << endl;
		cout << "-------------------------------" << endl;
		string s2;
		for (int i = 0; i < args - 1; i++)
		{
			s2 = argv[i + 1];
			solve(s2, path);
			cout << right << setw(4) << i + 1 << ". " << s2 << endl;
		}
		cout << "-------------------------------" << endl;
		cout << "  请输入序号：";
		int a;
	loop:
		cin >> a;
		if (!cin)                                                          //解决循环内整形或者浮点类型数据输入字符型就会陷入死循环的问题
		{                                                                   //只有输入操作失败，才会跳转到这里
			if (cin.bad())
			{                                                              //流发生严重故障，只能退出函数
				cout << "cin is bad!";
				exit(0);
			}
			if (cin.eof())
			{                                                             //检测是否读取结束

			}
			if (cin.fail())
			{                                                              //流遇到了一些意外情况（int型输入字母）
				cin.clear();                                          //清除/恢复流状态
				string se;
				cin >> se;
				//getchar();                                        //方案二
				cout << endl << "\a请不要胡乱输入！！！" << endl;
				goto loop;
			}
		}
		if (a <= 0 || a > (args - 1))
		{
			cout << "输入的数据不在范围内，请重新输入！！！" << endl;
			goto loop;
		}
		string s3 = argv[a];
		solve(s3, path);
		if (s3.find(".war") >= s3.length())
		{
			SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
			cout << "文件后缀名不正确\a！！！    正确的后缀名应为.war" << endl;
			cout << "按任意键退出程序......." << endl;
			_getch();
			exit(0);
		}

		cout << "  即将执行的jar文件的名称：";
		cout << s3 << endl;

		cout << setw(2) << "" << "开始执行！   执行内容如下：" << endl;
		cout << "------------------------------------------------" << endl;
		SetConsoleColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		
		cout << "拷贝war包到webapps目录下..." << endl;
		string war = s3;
		string cmd = "copy \"" + war + "\" \"" + tomcatPath + "\\webapps\"";
		//cout << cmd << endl;
		system(cmd.c_str());

		cout << "写入bat文件..." << endl;
		string batpath = exepath + "run.bat";
		ofstream writebat(batpath, ios::out);
		writebat << "cd /d \"" + tomcatPath + "\\bin\"\n";
		writebat << "start /min \"run\" \"" + tomcatPath + "\\bin\\startup.bat\"";
		writebat.close();


		cout << "启动Tomcat..." << endl;

		//cmd = "cd " + path;
		//cout << cmd << endl;
		//system(cmd.c_str());

		cmd = "start  \"\" \"" + exepath + "run.bat\"";
		//cout << cmd << endl;
		system(cmd.c_str());

		cout << "等待启动服务器..." << endl;

		Sleep(3000);
		cout << "清除多余窗口..." << endl;
		cmd = "taskkill /f /im cmd.exe";
		system(cmd.c_str());

		cout << "启动浏览器..." << endl;
		string name = s3;
		string n = ".war";
		solve(name, n);
		string url = "http://localhost:" + post + "/" + name + "/" + index;
		cmd = "start \"" + browserPath + "\" \"" + url + "\"";
		system(cmd.c_str());

		cout << "完成！" << endl;

		ofstream out(logPath, ios::app);
		SYSTEMTIME sys;                                    //获取系统时间
		GetLocalTime(&sys);
		out << "日期：";
		out << sys.wYear << "/" << sys.wMonth << "/" << sys.wDay << "\t";
		out << "时间：";
		out << setfill('0') << setw(2) << sys.wHour << ":" << setfill('0') << setw(2) << sys.wMinute << ":" << setfill('0') << setw(2) << sys.wSecond << "\t";
		out << "启动的war文件的路径：";
		out << argv[1] << endl;
		out.close();

		cout << "按任意键关闭服务器" << endl;
		_getch();
		cout << "再次按下任意键来关闭程序" << endl;
		_getch();

		cout << "关闭进程..." << endl;
		cmd = "taskkill /f /im java.exe";
		system(cmd.c_str());
		cout << "删除webapps目录下的对应war包..." << endl;
		string deletepath = tomcatPath + "\\webapps\\" + s3;
		//cout << deletepath << endl;
		remove(deletepath.c_str());
		if (name != "")
		{
			cout << "删除webapps目录下的对应的文件夹..." << endl;
			cmd = "rd /s /q \"" + tomcatPath + "\\webapps\\" + name + "\"";
			system(cmd.c_str());
		}
		cout << "完成！" << endl;
		cout << "按任意键退出程序" << endl;

		SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "------------------------------------------------" << endl;

	}

	else if (args == 1)
	{
		runstart();
		SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
		cout << "  异常，未找到war文件！！！   请通过war文件调用此程序\a" << endl;
		cout << "  选中要运行的war文件，右键，打开方式，选择始终使用此程序打开war文件" << endl;
		cout << "  使用此程序前需要配置config.ini文件" << endl;
		cout << "  关于config.ini文件：" << endl;
		cout << "  编码设置成GB2312 ，否则中文会乱码" << endl;
		cout << "  第一行为Tomcat路径，例如：C:\\Program Files\\Apache Software Foundation\\Tomcat 9.0" << endl;
		cout << "  第二行为端口号，例如：8080" << endl;
		cout << "  第三行为索引页面，例如：index.jsp" << endl;
		cout << "  第四行为浏览器路径，包含程序名，例如：C:\\Program Files (x86)\\Microsoft\\Edge\\Application\\msedge.exe" << endl;
		cout << "  第五行为日志文件路径，包含文件名，例如：C:\\tools\\war包一键部署程序\\war.log" << endl;

		SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		cout << "  按tab键查看日志，按1键打开Tomcat服务器webapps路径，或者按其它键退出" << endl;
		char ch;
		ch = _getch();
		string strlog;
		long totallog = 0;
		if (ch == 9)
		{
			string pass;
			cout << "  请输入密码：";
			cin >> pass;
			if (pass != "123")
			{
				SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
				cout << "  密码错误！！！" << endl;
				SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
			}
			else
			{
				ifstream log(logPath, ios::in);
				{
					if (!log)
					{
						SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
						cout << "没有找到日志文件！！！" << endl;
						cout << "当前日志文件路径为："<<logPath << endl;
						SetConsoleColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
					}
					else
					{

						while (!log.eof())
						{
							getline(log, strlog);
							cout << strlog << endl;
							totallog++;
						}
						cout << "一共" << totallog-1 << "项" << endl;
					}
				}
			}
		}
		else if (ch=='1')
		{
			string cmd = "start \"\" \"" + tomcatPath + "\\webapps\"";
			system(cmd.c_str());
			exit(0);
		}
	}

	//*************************计时结束***************************************
	//runend();



	



	//rundisplay();                                                                   //计时结果显示
	_getch();
	//closegraph();
	// system("pause");
	//Sleep(10000);
	return 0;
}
