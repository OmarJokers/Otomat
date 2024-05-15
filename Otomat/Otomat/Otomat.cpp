#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

#define log(x) cout << x << endl
#define logSingle(x) cout << x


// Eğer verilen parametre(startTime) ile şu anki zaman arasında bir gün geçtiyse true döner. Aksi halde false döner.
bool HasOneDayPassed(std::time_t startTime)
{
	std::time_t currentTime = std::time(nullptr);
	std::chrono::seconds difference = std::chrono::seconds(currentTime - startTime);
	std::chrono::hours hoursPassed = std::chrono::duration_cast<std::chrono::hours>(difference);

	return hoursPassed.count() >= 24;
}

// Ürünlerin ortak özelliklerini barındırır
struct Urun
{
	int id;
	string ad;
	int stok;
	double fiyat;
};

// Paranın yönetimini sağlar
class CihazKasa {
private:
	double para;

public:
	CihazKasa()
	{
		para = 500;
	}

	// Kasadaki parayı döndürür
	double KasadakiPara()
	{
		return para;
	}

	// Kasaya parametre(miktar) kadar para ekler.
	void paraEkle(double miktar)
	{
		para += miktar;
	}

	// Kasadan parametre(miktar) kadar para eksiltir. Eğer miktar kasadaki paradan büyükse bir işlem yapmaz.
	void paraAl(double miktar)
	{
		if (para >= miktar)
		{
			para -= miktar;
		}
	}
};

// Dosyadan ürünleri yükler ve vector'e aktarır
vector<Urun> UrunleriYukle()
{
	ifstream dosya("urunler.json");
	json veri;
	dosya >> veri;
	vector<Urun> urunler;

	for (const auto& urun : veri["urunler"])
	{
		Urun yeniUrun;
		yeniUrun.id = urun["id"];
		yeniUrun.ad = urun["ad"];
		yeniUrun.stok = urun["stok"];
		yeniUrun.fiyat = urun["fiyat"];
		urunler.push_back(yeniUrun);
	}
	return urunler;
}

// Urunler listesinde urunId'yi arar ve bulursa indexini döndürür. Aksi halde -1 döndürür.
int IndexOf(int urunId, vector<Urun>& urunler)
{
	for (int i = 0; i < urunler.size(); i++)
	{
		if (urunler[i].id == urunId)
		{
			return i;
		}
	}
	return -1;
}

// Girilen string(input) sayı ise true değil ise false döner.
bool SayiMi(string input)
{
	for (size_t i = 0; i < input.size(); i++)
	{
		if (isdigit(input[i]) == false)
		{
			return false;
		}
	}
	return true;
}

// girilen string(str) double ise true değil ise false döner.
bool isDouble(const string& str)
{
	istringstream doubleMi(str);
	double test;
	return (doubleMi >> test) && doubleMi.eof();
}

// girilen string(str)'i double'a çevirir.
double stringToDouble(const string& str)
{
	istringstream girdi(str);
	double result;
	if (girdi >> result)
	{
		if (girdi.eof())
		{
			return result;
		}
	}
	return -1.0;
}

void PrintProducts(vector<Urun>& urunler)
{
	for (const auto& urun : urunler)
	{
		cout << "ID: " << urun.id << endl;
		cout << "Ad: " << urun.ad << endl;
		cout << "Fiyat: " << urun.fiyat << " TL" << endl;
		cout << "---------------------------------" << endl;
	}
}

// Programın giriş noktası.
int main()
{
	CihazKasa kasa;
	auto lastUpdateTime = std::time_t(0);
	vector<Urun> urunler = UrunleriYukle();
	while (true)
	{
		log(" ************** HOSGELDINIZ **************");
		log("");
		log("");
		PrintProducts(urunler);

		logSingle("Istediginiz urunu secmek icin  bulunan sayiyi giriniz :");
		string secilenUrunSTR;
		cin >> secilenUrunSTR;
		if (SayiMi(secilenUrunSTR) == false)
		{
			cout << "Lutfen bir sayi giriniz." << endl;
			continue;
		}
		int secilenUrun = stoi(secilenUrunSTR);
		int urunIndex = IndexOf(secilenUrun, urunler);
		if (urunIndex == -1)
		{
			cout << "Gecersiz bir sayi girdiniz." << endl;
			continue;
		}
		if (urunler[urunIndex].stok <= 0)
		{
			log("Sectiginiz urun stokta bulunmamaktadir.");
			continue;
		}
		while (true)
		{
			cout << "Odeyeceginiz miktari giriniz :";
			string odenenMiktarStr;
			cin >> odenenMiktarStr;
			if (isDouble(odenenMiktarStr) == false)
			{
				cout << "Lutfen bir sayi giriniz.";
				continue;
			}

			double odenenMiktar = stringToDouble(odenenMiktarStr);
			double farkdeger = urunler[urunIndex].fiyat - odenenMiktar;
			while (farkdeger > 0)
			{
				cout << "Istediginiz urun icin " << farkdeger << "TL daha odemeniz gerekmektedir.\n";
				cout << "Lutfen eksik parayi giriniz :";
				double eksikpara;
				cin >> eksikpara;
				if (farkdeger - eksikpara >= 0)
				{
					farkdeger -= eksikpara;
					continue;
				}
				farkdeger -= eksikpara;
				break;
			}
			if (farkdeger < 0)
			{
				double paraustu = farkdeger * -1;
				kasa.paraAl(paraustu);
				cout << "Iste paraustunuz : " << paraustu << endl;
			}

			kasa.paraEkle(odenenMiktar);
			urunler[urunIndex].stok--;
			cout << "Iste Urununuz!\n";

			cout << "Baska bir islem yapmak ister misiniz? (e/h) :";
			string karar;
			cin >> karar;
			if (karar == "e")
			{
				break;
			}
			else if (karar == "h")
			{
				return 0;
			}
			break;
		} // while

		if (HasOneDayPassed(lastUpdateTime))
		{
			double butunPara = kasa.KasadakiPara();
			kasa.paraAl(butunPara);
			double hasılat = butunPara - 500;
			kasa.paraEkle(500);
		}
	} // main While
}
