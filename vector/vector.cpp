// C++ dilinde "vector," C++ Standard Template Library(STL)
// tarafından sağlanan dinamik bir dizi sınıfıdır.Vektör, bir dizi veriyi depolamak ve işlemek için kullanılır.
// Standart dizilere(arrays) kıyasla, vektörler daha esnek ve güçlüdür, çünkü boyutları dinamik olarak ayarlanabilir.

#include <iostream>
using namespace std;

// method overload yaparak fonksiyonları tekrarlamaya gerek yok.

// template <typename T>
// T square(T x) {
// 	return x * x;
// }

// vectorler büyüyüp küçülebilen dizilerdir.
#include <vector>


int main() {
	// cout << square(5.5) << endl;

	// Constructors
	{
		vector <int> v;
		vector <int> v1(5, 10); // 5 elemanlı 10 larla dolu bir vektör oluşturur.
		vector <int> v2(10); // 10 elemanlı 0 larla dolu bir vektör oluşturur.
		vector <int> v3(v2); // v2 nin kopyası v3 e atanır.
		vector <int> v4(&v2[0], &v2[5]); // v2 nin 0. elemanından 5. elemanına kadar olan elemanlar v4 e atanır.
	}

	// vector member functions

	{
		vector <int> v;
		vector <int> v2;

		v.assign(5, 10); // vektörün tüm elemanlarını 10 yapar.
		v.push_back(10); // vektörün sonuna 10 ekler.
		v.pop_back(); // vektörün sonundaki elemanı siler.
		v.insert(v.begin() + 2, 100); // vektörün 2. elemanına 100 ekler.
		v.erase(v.begin() + 2); // vektörün 2. elemanını siler.
		v.erase(v.begin() + 2, v.begin() + 4); // vektörün 2. elemanından 4. elemanına kadar olan elemanları siler.
		v.clear(); // vektörün tüm elemanlarını siler.
		v.empty(); // vektörün boş olup olmadığını kontrol eder.
		v.size(); // vektörün eleman sayısını verir.
		v.capacity(); // vektörün kapasitesini verir.
		v.resize(10); // vektörün eleman sayısını 10 yapar.
		v.resize(10, 5); // vektörün eleman sayısını 10 yapar ve elemanları 5 yapar.
		v.reserve(100); // vektörün kapasitesini 100 yapar.
		v.shrink_to_fit(); // vektörün kapasitesini eleman sayısına eşitler.
		v.at(2); // vektörün 2. elemanını verir.
		v.front(); // vektörün ilk elemanını verir.
		v.back(); // vektörün son elemanını verir.
		v.data(); // vektörün ilk elemanının adresini verir.
		v.swap(v2); // vektörlerin elemanlarını değiştirir.
	}

	{
		vector <int> v;
		v.push_back(10);
		v.push_back(20);
		v.push_back(30);

		std::cout << v[0] << std::endl; // 10
		std::cout << v.front() << std::endl; // 10
	}

}

// vectorlerin avantajları
// 1. dinamik boyut
// 2. veri ekleme ve çıkarma işlemleri hızlıdır.
// 3. vektörlerin elemanları arasında rahatça gezinebiliriz.
// 4. vektörlerin elemanlarına [] operatörü ile erişebiliriz.
// 5. vektörlerin elemanlarına at() fonksiyonu ile erişebiliriz.
// 6. vektörlerin elemanlarına front() ve back() fonksiyonları ile erişebiliriz.


