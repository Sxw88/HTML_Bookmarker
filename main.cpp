/*
README

The purpose of the program is to automatically generate a menu that links to
article chapters in the same page, given input in the form of a numbered list.

Example Input:
1. TOPIC NUMBER 1
2. TOPIC NUMBER 2
3. TOPIC NUMBER 3

Example Output:
<ol>
	<li style="text-align: left;"><a href="#step_1">TOPIC NUMBER 1</a></li>
    <li style="text-align: left;"><a href="#step_2">TOPIC NUMBER 2</a></li>
    <li style="text-align: left;"><a href="#step_3">TOPIC NUMBER 3</a></li>
</ol>

<h2 id="step_1">TOPIC NUMBER 1</h2>
<span style="font-weight: 400;">INSERT CONTENT HERE</span>

<h2 id="step_2">TOPIC NUMBER 2</h2>
<span style="font-weight: 400;">INSERT CONTENT HERE</span>

<h2 id="step_3">TOPIC NUMBER 3</h2>
<span style="font-weight: 400;">INSERT CONTENT HERE</span>
*/

#include <windows.h>
#include <string>
#include <vector>

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_CHANGETITLE 3
#define FILE_MENU_SUBMENU_ITEM 4
#define HELP_MENU 69
#define EXIT_MENU 70
#define BUTTON_GENERATE 100

LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void AddMenus(HWND);
void AddControls(HWND);
std::wstring AddHTML(std::wstring);

//Declaring handlers for our window objects
HMENU hMenu;
HWND hEdit;   //(type Edit) window for input
HWND hOutput; //(type Edit) window for output

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int nCmdShow)
{
    WNDCLASSW wc = {0};

    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;    //background color of window
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //cursor type
    wc.hInstance = hInst;
    wc.lpszClassName = L"myWindowClass";
    wc.lpfnWndProc = WindowProcedure;

    if (!RegisterClassW(&wc))
        return -1;

    CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100,100,800,900,NULL,NULL,NULL,NULL);
    MSG msg = {0};

    while (GetMessage(&msg, NULL,NULL,NULL) )
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_COMMAND:
            switch (wp)
            {
            case FILE_MENU_NEW:
                MessageBeep(MB_ABORTRETRYIGNORE);
                break;
            case FILE_MENU_OPEN:
                MessageBeep(MB_RIGHT);
                break;
            case FILE_MENU_CHANGETITLE:
                wchar_t text[100];
                GetWindowTextW(hEdit,text,100);
                SetWindowTextW(hWnd,text);        //changes title of window
                break;
            case FILE_MENU_SUBMENU_ITEM:
                break;
            case HELP_MENU:
                MessageBeep(MB_HELP);
                break;
            case EXIT_MENU:
                DestroyWindow(hWnd);
                break;
            case BUTTON_GENERATE:
                wchar_t text_input[5000];
                GetWindowTextW(hEdit,text_input,5000);

                std::wstring str_output( text_input );            //convert LPCWSTR (aka const wchar_t) to std::string
                str_output = AddHTML(str_output);                 ///function to add HTML to the list
                const wchar_t* text_output = str_output.c_str();  //convert std::string back to LPCWSTR

                SetWindowTextW(hOutput,text_output);     //changes the contents of Output window
                break;
            }
        break;
    case WM_CREATE:
        AddMenus(hWnd);
        AddControls(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcW(hWnd, msg, wp, lp);
    }
}

void AddMenus(HWND hWnd)
{
    hMenu = CreateMenu();            //creates top-level menu
    HMENU hFileMenu = CreateMenu();  //sub-menu for file
    HMENU hSubMenu = CreateMenu();

    AppendMenu(hMenu,MF_POPUP,(UINT_PTR)hFileMenu,"File");
        AppendMenu(hFileMenu,MF_STRING,FILE_MENU_NEW,"New");
        AppendMenu(hFileMenu,MF_SEPARATOR,NULL,NULL);
        AppendMenu(hFileMenu,MF_STRING,FILE_MENU_OPEN,"Open");
        AppendMenu(hFileMenu,MF_POPUP,(UINT_PTR)hSubMenu,"Submenu");
            AppendMenu(hSubMenu,MF_STRING,FILE_MENU_SUBMENU_ITEM,"Submenu Item");
    AppendMenu(hFileMenu,MF_STRING,FILE_MENU_CHANGETITLE,"Change Title");

    AppendMenu(hMenu,MF_STRING,HELP_MENU,"Help");
    AppendMenu(hMenu,MF_STRING,EXIT_MENU,"Exit");


    SetMenu(hWnd,hMenu);            //assigns menu to window handler
}

void AddControls(HWND hWnd)
{
    const wchar_t* sampleText = L"1. TOPIC NUMBER 1\r\n2. TOPIC NUMBER 2\r\n3. TOPIC NUMBER 3";

    CreateWindowW(L"Static", L"Enter Numbered List here :", WS_VISIBLE | WS_CHILD | SS_CENTER, 75,10,650,50, hWnd, NULL,NULL,NULL);
    hEdit = CreateWindowW(L"Edit", sampleText, WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL, 75,45,650,200, hWnd, NULL,NULL,NULL);

    CreateWindowW(L"Button", L"Generate HTML", WS_VISIBLE | WS_CHILD | BS_DIBPATTERN8X8, 75,260,650,50, hWnd, (HMENU)BUTTON_GENERATE,NULL,NULL);

    hOutput = CreateWindowW(L"Edit", L"Output", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | WS_HSCROLL, 75,330,650,200, hWnd, NULL,NULL,NULL);
}

/*
Example Input:
1. TOPIC NUMBER 1
2. TOPIC NUMBER 2
3. TOPIC NUMBER 3

Example Output:
<ol>
	<li style="text-align: left;"><a href="#step_1">TOPIC NUMBER 1</a></li>
    <li style="text-align: left;"><a href="#step_2">TOPIC NUMBER 2</a></li>
    <li style="text-align: left;"><a href="#step_3">TOPIC NUMBER 3</a></li>
</ol>

<h2 id="step_1">TOPIC NUMBER 1</h2>
<span style="font-weight: 400;">INSERT CONTENT HERE</span>

<h2 id="step_2">TOPIC NUMBER 2</h2>
<span style="font-weight: 400;">INSERT CONTENT HERE</span>

<h2 id="step_3">TOPIC NUMBER 3</h2>
<span style="font-weight: 400;">INSERT CONTENT HERE</span>
*/

std::wstring AddHTML(std::wstring str1)
{
    std::wstring str2 = L"<!--Anchor Links generated automatically by sxw's AddHTML.exe-->\r\n\r\n<ol>";
    std::wstring str3 = L"\r\n<!--Content Starts Here-->\r\n\r\n";

    ///extract list of topics into vector (vtopics)
    std::vector<std::wstring> vtopics;
    wchar_t prev_c;
    bool insert_to_vector = false;

    for (wchar_t const &c: str1) {
        if (insert_to_vector)
        {
            vtopics.back() += c;
        }
        if (c == ' ' && prev_c == '.')
        {
            vtopics.push_back(L"");
            insert_to_vector = true;
        }
        if (c == '\r')
        {
            insert_to_vector = false;
        }
        prev_c = c;
    }

    ///iterate thru vector and add HTML
    int topic_no = 1;
    for (std::vector<std::wstring>::iterator i = vtopics.begin(); i != vtopics.end(); ++i)
    {
        str2 += L"\r\n    <li style=\"text-align: left;\"><a href=\"#step_";
        str2 += std::to_wstring( topic_no );
        str2 += L"\">";
        str2 += *i;
        str2 += L"</a></li>";

        str3 += L"<h2 id=\"step_";
        str3 += std::to_wstring( topic_no );
        str3 += L"\">";
        str3 += std::to_wstring( topic_no );
        str3 += L". ";
        str3 += *i;
        str3 += L"</h2>\r\n";
        str3 += L"<span style=\"font-weight: 400;\">INSERT CONTENT HERE</span>\r\n\r\n";

        topic_no ++;
    }
    str2 += L"\r\n</ol><hr>\r\n";
    str2 += str3;

    return str2;
}
