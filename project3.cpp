#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <chrono>
using namespace std;
const string filename = "item.txt";
const string logfile ="log.txt";
bool task = false;
struct user
{
    string username;
    string password;
    double balance = 2.5;
};

struct admin
{
    string username;
    string password;
};
struct Item
{
    string name;
    double price;
    int quantity;
};
//log info
void log_info(const string &filename ,const string &command,double time , bool success , string userType , string username) {
ofstream logFile(filename,ios::app);
    
    if (!logFile)
    cout << "can't open the logfile. \n";
    else
     {
        
        logFile << " " << command << " ," <<time  << "s ," 
                << (success ? "Successful" : "Fail") << " " <<"command made by "<<userType <<" "<<username <<'\n';
    }
    
}
 
//check things 

void To_LOWER(string &command)
{
    for (int i = 0; i < command.length(); ++i)
    {
        if (command[i] >= 65 && command[i] <= 90)
            command[i] = command[i] + 32;
    }
}
bool check_to_be_letters(string x)
{
    int len = x.length();
    if (x.empty())
        cout << "empty command please enter a valid command. \n";
    for (int i = 0; i < len; ++i)
    {
        if (isdigit(x[i]))
        {
            cout << "ERROR! you entered numbers you have to write letters. ";
            return false;
        }
    }
    return true;
}
bool check_to_be_Double(const string &str)
{
    if (str.empty())
        cout << "#empty command please enter a valid command. \n";
    try
    {
        size_t pos;
        stod(str, &pos);
        return pos == str.length();
    }
    catch (...)
    {
        return false;
    }
}
bool isFileEmpty(const string &filename) {
    ifstream file(filename);
    if (!file) {
        return true;
    }
    char ch;
    while (file >> noskipws >> ch) { // Read without skipping whitespace
        if (!isspace(ch)) {
            return false; 
        }
    }

    return true;
}
// saving to file
void loadItem(const string& filename , Item item[] , int &numofitem) {
    ifstream file(filename);
    if (!file) {
        cout << "i can't open the file :(\n ";
        return;
    }
    numofitem = 0 ;
    while(file >> item[numofitem].name >> item[numofitem].price >> item[numofitem].quantity)
    {
        numofitem++;
    }
    file.close();
}
void saveitems(const string &filename, Item item[], int &numofitem)
{
    ofstream file(filename, ios ::trunc);

    if (!file)
    {
        cout << "i can't open the file :(\n ";
    }
    else
    {
        for (int i = 0; i < numofitem; ++i)
        {
            file << item[i].name << " " << item[i].price << " " << item[i].quantity << '\n';
        }
    }
    file.close();
}
// Admin commands
void add_new_items(string name, double price, Item *&item, int &numofitem, int tedad)
{
    for (int i = 0; i < numofitem; i++)
    {
        if (item[i].name == name)
        {
            cout << "the item is already availiable in the shop would you like to add more of this item to shop ?\n if yes write the number you want to add othewise enter non empty word.\n$ ";
            string z;
            cin >> z;
            if (!check_to_be_letters(z))
            {
                task = true;
                item[i].quantity += stod(z);
                cout << "the quantity of " << item[i].name << " now is " << item[i].quantity << "\n$";
                saveitems(filename, item, numofitem);
                
                return;
            }
            else{
                task = false;
                return;}
        }
    }
   numofitem+=1; 
    Item *temp = new Item[numofitem];
    for (int i = 0; i < numofitem - 1; ++i){
        temp[i] = item[i];
    }
    temp[numofitem - 1].name = name;
    temp[numofitem - 1].price = price;
    if (tedad == 0)
        temp[numofitem - 1].quantity = 1;
    else
        temp[numofitem - 1].quantity = tedad;
    delete[] item;
    item = temp;
    cout << "$item added successfully!\n"; 
    task = true;
    saveitems(filename, item, numofitem);
}
void remove_item(string x, Item *&item, int &numofitem)
{
    int inx = -1;
    string rname;
    double rprice;
    int rcount;
    for (int i = 0; i < numofitem; ++i)
    {
        if (item[i].name == x)
        {
            inx = i;
            break;
        }
    }
    if (inx == -1)
    {
        cout << "$item not found ! \n$";
        task = false;
        return;
    }
    else
    {
        if(item[inx].quantity == 1){
            cout << "$only 1 item is in the shop so we delete it completly.\n";
        rname = item[inx].name;
        rprice = item[inx].price;
        rcount = item[inx].quantity;

        for (int i = inx; i < numofitem - 1; i++)
        {
            item[i] = item[i + 1];
        }
        numofitem--;

        saveitems(filename, item, numofitem);
        cout << "$item removed successfully ! \n";
        task = true;
        cout << "$if you deleted that by mistake you can bring it back by typing reset otherwise write any non empty word.\n$";
        string g;
        cin >> g;
        if (g == "reset")
        {
            add_new_items(rname, rprice, item, numofitem, rcount);
            saveitems(filename, item, numofitem);
        }
        else
        task = true;
        return ;
        }
        else
        {
        item[inx].quantity--;
        cout << "$1 "<< item[inx].name<<" is removed\n";
        task = true;
        cout << "$if you deleted that by mistake you can bring it back by typing reset otherwise write any non empty word.\n$";
                string g;
        cin >> g;
        if (g == "reset")
        {
           item[inx].quantity++;
            saveitems(filename, item, numofitem);
            cout << "$item is back ! \n";
            task = true;
        }
        else{
        saveitems(filename , item , numofitem);
        task = true;
         return ; }
        }
    }
}
void rename_item(string prev, string after, Item item[], int &numofitem)
{
    if (prev == after)
    {
        cout << "$the name of " << prev << " is already " << prev << '\n'
             << "$";
             task = false;
        return;
    }
    else
    {
        for (int i = 0; i < numofitem; ++i)
        {
            if (item[i].name == after)
            {

                cout << "$you can't change the item name with another item that already is in the shop" << '\n'
                     << "$";
                     task = false;
                return;
            }
        }

        for (int i = 0; i < numofitem; ++i)
        {
            if (prev == item[i].name)
            {
                cout << "$renamed " << item[i].name << " to " << after << " successfully!\n$";
                item[i].name = after;

                saveitems(filename, item, numofitem);
                task = true;
                return;
            }
        }
    }
}
void new_price(string name, double after, Item item[], int &numofitem)
{
    if(after<0){
    cout << "signed numbers is not avaliable for price.\n";
    task = false;
    return;}
    bool found = false;
    for (int i = 0; i < numofitem; ++i)
    {
        if (name == item[i].name)
        {
            found = true;
            item[i].price = after;
            saveitems(filename, item, numofitem);
            break;
        }
    }
    if(found){
    cout << "$the price of " << name << " now changed to " << after << "\n";
    task = true;
    }
    else
    {
        cout << "$item not found.\n";
        task = false;
        return;
    }
}
void add_credit(string username, double gift, user Users[], int numofusers)
{
    if(gift <0){
    cout << "signed numbers is not avaliable.\n";
    task = false;
    return;}
    bool found = false;
    string name;
    for (int i = 0; i < numofusers; ++i)
    {
        if (username == Users[i].username)
        {
            found = true;
            name = Users[i].username;
            Users[i].balance += gift;
            break;
        }
    }
    if(found){
    cout << "$credit added successefully to user " << name << " ! \n";
    task = true;}
    else
    {
        cout << "$user not found.\n";
        task = false;
        return;
    }
}
// user commands
void showBalance(string username, user Users[], int numofuser)
{
    for (int i = 0; i < numofuser; ++i)
    {
        if (Users[i].username == username)
        {
            cout << "#your current balance is : " << Users[i].balance << " $\n";
        }
    }
    task = true;
}
void buying(string x, string username, user Users[], int numofuser, Item item[], int &numofitem)
{
    int userIndex = -1;
    for (int i = 0; i < 3; i++)
    {
        if (Users[i].username == username)
        {
            userIndex = i;
            break;
        }
    }

    for (int i = 0; i < numofitem; ++i)
    {
        if (x == item[i].name)
        {
            if (item[i].quantity <= 0)
            {
                cout << "#we are out of this item. Sorry!\n";
                task = false;
                return;
            }
            if (Users[userIndex].balance < item[i].price)
            {
                cout << "#insufficient balance!\n";
                task = false;
                return;
            }
            item[i].quantity--;
            Users[userIndex].balance -= item[i].price;
            cout << "#purchase successful! Your new balance: " << Users[userIndex].balance << "$\n";
            saveitems(filename, item, numofitem);
            task = true;
            return;
        }
    }
    cout << "#item not found in shop.\n";
    return;
}
void show_Items(Item item[], int &numofitem)
{
    cout << left << setw(20) << "name" << "|" << setw(10) << "price" << "|" << '\n';
    cout << string(32, '-') << '\n';

    for (int i = 0; i < numofitem; i++)
    {
        cout << left << setw(20) << item[i].name << "|"
             << setw(10) << fixed << setprecision(2) << item[i].price << "$ |" << '\n';
    }
    task = true;
}
void show_specific_item(string x, Item item[], int &numofitem)
{
    bool found = false;
    for (int i = 0; i < numofitem; ++i)
    {
        if (x == item[i].name)
            found = true;
    }
    if (!found)
    {
        cout << "item not found !\n";
        return;
    }

    cout << left << setw(20) << "name" << "|" << setw(10) << fixed << setprecision(2) << "price" << " |" << setw(10) << fixed << setprecision(2) << "quantity" << '\n';
    cout << string(32, '-') << '\n';
    for (int i = 0; i < numofitem; ++i)
    {
        if (x == item[i].name)
        {
            cout << left << setw(20) << item[i].name << "|" << setw(10) << fixed << setprecision(2) << item[i].price << "$|" << setw(10) << fixed << setprecision(2) << item[i].quantity << '\n';
        }
    }
    task = true;
}
void helps()
{
    cout << "#enter <show item> to see the list of item in the shop.\n";
    cout << "#enter <show specefic item> to see more info about the item you want to buy.\n";
    cout << "#enter <buy specefic item> to buy an item.\n";
    cout << "#enter <balance> to see your wallet balance.\n";
    cout << "#enter <help> to see explantion on every commands\n";
    cout << "#enter <help specefic command> to see the explaintion on a specefic command. \n";
    cout << "#enter <logout> to logout of your account.\n";
    task = true;
}
void help_command(string x)
{

    if (x == "buy")
    {
        cout << "#buy command can be used to buy 1 item in the shop , for example you can write buy corn to buy 1 corn\n";
    }
    else if (x == "show item")
    {
        cout << "#show item command let you see every items in the shop anything availiable in the shop so use it to see items \n";
    }
    else if (x == "show")
    {
        cout << "#show command can be used to show more details about a specefic item , for example show corn let you see name , price and quantity of the item\n";
    }
    else if (x == "help")
    {
        cout << "#help command alone show you every single commands and a detail about how to use them , if you want more detail on how to use a specefic command write help command \n";
    }
    else if (x == "balance")
    {
        cout << "#balance command can be used to check the balance of your wallet\n";
    }
    else if (x == "logout")
    {
        cout << "if you want to stop using the app and logout of your account use this command\n";
    }
    task = true ;
}
// finding admins and users
bool find_user(string x, string &pointerto, user Users[], int numofuser, admin admins[], int numofadmin)
{
    string p;
    for (int i = 0; i < numofuser; i++)
    {
        if (x == Users[i].username)
        {
            {
                cout << "\nplease enter your password : ";
                getline(cin, p);
                if (p.empty())
                    cout << "#empty password! \n";
                while (p != Users[i].password)
                {
                    cout << "\nwrong password try again : ";
                    getline(cin, p);
                    if (p.empty())
                        cout << "#empty password! \n";
                }

                cout << "\n#welcome user " << x << '\n';

                pointerto = "user";
                return true;
            }
        }
    }
    for (int i = 0; i < numofadmin; i++)
    {
        if (x == admins[i].username)
        {
            cout << "\nplease enter your password : ";
            getline(cin, p);
            if (p.empty())
                cout << "#empty password! \n";
            while (p != admins[i].password)
            {
                cout << "\nwrong password try again : ";

                getline(cin, p);
                if (p.empty())
                    cout << "#empty password! \n";
            }

            cout << "\n$welcome admin " << x << '\n';
            pointerto = "admin";
            return true;
        }
    }
    
    return false;
}
// commands
void User_commands(string x, string username, user Users[], int numofuser, Item item[], int &numofitem)
{
    if (x.empty())
        cout << "#empty command please enter a valid command. \n";
    else if (x == "show item"){
        auto start = chrono::high_resolution_clock::now();
        show_Items(item, numofitem);
         auto end = chrono::high_resolution_clock::now();
        chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "user" , username);
    }
    else if (x == "help"){
        auto start = chrono::high_resolution_clock::now();
        helps();
         auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "user" , username);
    }
    else if (x == "balance"){
        auto start = chrono::high_resolution_clock::now();
        showBalance(username, Users, numofuser);
         auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "user" , username);
    }
    else if (x.substr(0, 5) == "show "){
        auto start = chrono::high_resolution_clock::now();
        show_specific_item(x.substr(5, x.length() - 1), item, numofitem);
         auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "user" , username);
    }
    else if (x.substr(0 ,4) == "buy "){
        auto start = chrono::high_resolution_clock::now();
        buying(x.substr(4), username, Users, numofuser, item, numofitem);
         auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "user" , username);
    }
    else if (x.substr(0 , 5) == "help "){
        auto start = chrono::high_resolution_clock::now();
        help_command(x.substr(5));
         auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "user" , username);
    }
    else if (x == "logout")
        return;
    else{
        cout << "#command not found, please enter help to see the available commands or write a correct command.\n";

    }
    cout << "#enter your command: ";
    getline(cin, x);
    if(cin.fail())
    {
        cin.clear();
        cin.ignore();
        cout << "command not found!\n";
    }
    To_LOWER(x);
    User_commands(x, username, Users, numofuser, item, numofitem);
}
void Admin_commands(string x, Item items[], int &numofitem, user Users[], int numofuser , string username)
{

    if (x == "logout")
        return;
    string action, item, newName;
    double priceOrAmount = 0;
    if (x[0] == 'a')
    {
        if (isalpha(x[x.length()]))
        {
            cout << "$the correct format is <add item price>! .\n";
            return;
        }
        else
        {
            action = x.substr(0, 3);
            int c = 0;
            int i = 4;
            while (x[i] != 32 && i < x.length())
            {
                item += x[i];
                i++;
                c++;
            }
            if (check_to_be_Double(x.substr(4 + c + 1, x.length() - 1)))
            {
                priceOrAmount = stod(x.substr(4 + c + 1, x.length() - 1));
            }
            else
            {
                cout << "$the correct format is <add item price>! .\n";
                return;
            }
        }
    }
    else if (x[0] == 'r' && x[2] == 'n')
    {
        if (!check_to_be_letters(x))
            return;
        action = x.substr(0, 6);
        int c = 0;
        int i = 7;
        while (x[i] != 32 && i < x.length())
        {
            item += x[i];
            i++;
            c++;
        }
        newName = x.substr(7 + c + 1, x.length() - 1);
        //  cout << action <<" "<<item << " "<<newName;
    }
    else if (x[0] == 'r' && x[2] == 'm')
    {
        if (!check_to_be_letters(x))
            return;
        action = x.substr(0, 6);
        int c = 0;
        int i = 7;
        while (x[i] != 32 && i < x.length())
        {
            item += x[i];
            i++;
            c++;
        }
        // cout << action <<" "<<item << " ";
    }
    else if (x[0] == 'p')
    {
        action = x.substr(0, 5);
        int c = 0;
        int i = 6;
        while (x[i] != 32 && i < x.length())
        {
            item += x[i];
            i++;
            c++;
        }
        if (check_to_be_Double(x.substr(6 + c + 1, x.length())))
        {
            priceOrAmount = stod(x.substr(6 + c + 1, x.length()));
        }
        else
        {
            cout << "$the correct format is <price item newprice> ! .\n";
            return;
        }

        //    cout << action << " "<<item << " "<<priceOrAmount;
    }
    else if (x[0] == 'c')
    {
        action = x.substr(0, 6);
        int c = 0;
        int i = 7;
        while (x[i] != 32 && i < x.length())
        {
            item += x[i];
            i++;
            c++;
        }
        if (check_to_be_Double(x.substr(7 + c + 1, x.length())))
            priceOrAmount = stod(x.substr(7 + c + 1, x.length()));
        else
        {
            cout << "$the correct format is <credit user price>! . \n";
        }
    }

    if (action == "add")
    {
    auto start = chrono::high_resolution_clock::now();
    add_new_items(item, priceOrAmount, items, numofitem, 0);
      cin.ignore();
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "Admin" , username);
    }

    else if (action == "remove")
    {
      
        auto start = chrono::high_resolution_clock::now();
        remove_item(item, items, numofitem);
          cin.ignore();
         auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "Admin" , username);
    }
    else if (action == "rename")
    {
        auto start = chrono::high_resolution_clock::now();
        rename_item(item, newName, items, numofitem);
         auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "Admin", username);
    }
    else if (action == "price")
    {
        auto start = chrono::high_resolution_clock::now();
        new_price(item, priceOrAmount, items, numofitem);
         auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "Admin" , username);
    }

    else if (action == "credit")
    {
        auto start = chrono::high_resolution_clock::now();
        add_credit(item, priceOrAmount, Users, numofuser);
         auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    log_info(logfile,x ,duration.count() , task , "Admin" , username);
    }
    else
    {
        cout << "$command not found! please check the format for you'r command.\n";
    }
    cout << "$enter your command: ";
    getline(cin, x);
    To_LOWER(x);
    Admin_commands(x, items, numofitem, Users, numofuser , username);
}

int main()
{
    const int max_item = 1000;
    Item *item = new Item[max_item];
     int numofitem = 0;
  if (isFileEmpty(filename)) {
    numofitem = 10;

    item[0] = {"apple", 2.5, 5};
    item[1] = {"banana", 1.5, 10};
    item[2] = {"melon", 5, 3};
    item[3] = {"orange", 1.2, 10};
    item[4] = {"grape", 5000, 6};
    item[5] = {"mango", 4.6, 9};
    item[6] = {"pineapple", 5.5, 10};
    item[7] = {"cherry", 2.1, 26};
    item[8] = {"strawberry", 3, 30};
    item[9] = {"kiwi", 3.5, 10};

    saveitems(filename, item, numofitem);
}
   
    loadItem(filename,item ,numofitem);
    admin admins[2] = {"jack", "1", "lily", "youareshort"};
    user Users[3] = {"ali", "1234", 100, "levi", "password", 100, "eren", "tataka", 100};
    int numofuser = 3;
    int numofadmin = 2;
    cout << "----------------welcome to WMS !----------------\n";
    cout << "<login>\n please enter your username : ";
    string username;
    string role;
    getline(cin, username);

    if (!check_to_be_letters(username))
    {
        return 0;
    }
    if (!find_user(username, role, Users, numofuser, admins, numofadmin))
    {
        cout << "#user not found :( \n";
    }
    if (role == "user")
    {
        cout << "#(if you want help with commands write help, otherwise enter your command : ";
        string a;
        getline(cin, a);
        To_LOWER(a);
        User_commands(a, username, Users, numofuser, item, numofitem);
    }
    else if (role == "admin")
    {
        cout << "$enter your command :";
        string b;
        getline(cin, b);
        To_LOWER(b);
        Admin_commands(b, item, numofitem, Users, numofuser , username);
    }

    return 0;
}
