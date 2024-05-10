#include <iostream>
#include <string>
#include <fstream>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

int main()
{
    cout << " ************** HOSGELDINIZ **************" << endl;
    cout << "" << endl;
    cout << "Istediginiz urunu secmek icin yaninda bulunan sayiyi giriniz." << endl;
    cout << "" << endl;
    cout << "Urun Listesi    Fiyat Listesi\n";
    cout << "1-Su             1.50TL\n";
    cout << "2-Limonata       2.50TL\n";
    cout << "3-Soguk Cay      3.25TL\n";
    cout << "4-Soguk Kahve    3.75TL\n";
    cout << "5-Coca Cola      4.75TL\n";
    cout << "6-Pepsi          4.50TL\n";
    cout << "7-Meyve Suyu     2.25TL\n";
    cout << "8-Gazoz          3.50TL\n";
    cout << "9-Soda           2.75TL\n";
    cout << "0-Ayran          1.75TL\n";
}
