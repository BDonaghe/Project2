#include <vector>
#include <iostream>
#include <string>
#include <time.h> //Seed for random (for testing)

using std::string;
using std::vector;
using std::cin;
using std::cout;
using std::endl;
using std::srand; //For testing

void swap(int&a, int&b) {
	int x = b;
	b = a;
	a = x;
}

int iPow(int x, int d) {
	for (int i = 0; i < d; i++) {
		x *= x;
	}
	return x;
}

int partition(vector<int>& vec1, int low, int high)
{
	int pivot = vec1[high];    // pivot
	int i = (low - 1);  // Index of smaller element

	for (int j = low; j <= high - 1; j++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (vec1[j] <= pivot)
		{
			i++;    // increment index of smaller element
			swap(vec1[i], vec1[j]);
		}
	}
	swap(vec1[i + 1], vec1[high]);
	return (i + 1);
}


// Modified from example at https://www.geeksforgeeks.org/quick-sort/
vector<int> QuickSort(vector<int>& vec1, int low, int high) {
	if (low < high)
	{
		/* pi is partitioning index, arr[p] is now
		at right place */
		int pi = partition(vec1, low, high);

		// Separately sort elements before
		// partition and after partition
		QuickSort(vec1, low, pi - 1);
		QuickSort(vec1, pi + 1, high);
	}
	return vec1;
}

void merge(vector<int>& v, int left, int mid, int right) { //Modified from https://www.geeksforgeeks.org/merge-sort/
	int i, j, k;
	int n1 = mid - left + 1;
	int n2 = right - mid;

	// Temporary vectors
	vector<int> L;
	vector<int> R;

	// Copy to temp vectors
	for (i = 0; i < n1; i++) {
		L.push_back(v[left + i]);
	}
	for (j = 0; j < n2; j++) {
		R.push_back(v[mid + 1 + j]);
	}
		   // Initial index of:
	i = 0; // First subvector
	j = 0; // second subvector
	k = left; // merged subvector

	//Merge temp vectors
	while (i < n1 && j < n2) {
		if (L[i] <= R[j]) {
			v[k] = L[i];
			i++;
		}
		else {
			v[k] = R[j];
			j++;
		}
		k++;
	}

	// Copy anything left in L
	while (i < n1) {
		v[k] = L[i];
		i++;
		k++;
	}

	// Copy anything left in R
	while (j < n2) {
		v[k] = R[j];
		j++;
		k++;
	}
}

void mergeSort(vector<int>& v, int left, int right) {
	if (left < right) {
		int m = (left + right) / 2;

		//sort first /second halves
		mergeSort(v, left, m);
		mergeSort(v, m + 1, right);

		merge(v, left, m, right);
	}
	return;
}

int binarySearch(vector<int> v, int l, int r, int x) {
	if (r >= 1) {
		int m = l + (r - l) / 2; // middle index

		if (v.at(m) == x) {
			return m;
		}
		if (v.at(m) > x) {
			return binarySearch(v, l, m - 1, x);
		}
		else {
			return binarySearch(v, m + 1, r, x);
		}
	}
	else {
		return -1;	// if value was not found
	}
}

int BHash(string a, int tbl[], int tSize) {	// Key, hashtable, table size
	int y = 0;
	for (unsigned int i = 0; i <= a.length(); i++) {
		char c = a[i];
		int x = c;
		x = iPow(x, i+1); // i+1 to eliminate x^0
		y += x;		   // Otherwise, the first digit is always 1
	}
	int b = y % tSize;
	tbl[b] = y;;
	return b;
}

int BHash(int a, int tbl[], int tSize) {	// Key, hashtable, table size
	int y = 0;

	vector<int> digits;	// For manipulation of individual digits from a
	while (a) {
		digits.push_back(a % 10); // Separate a into individual digits
		a /= 10;
	}
	for (unsigned int i = 1; i < digits.size()-1; i++) {
		int x = iPow(digits[i], i+1); // i+1 to eliminate x^0 
		y += x;	// Add to hash value
	}
	int b = y % tSize;
	tbl[b] = y;
	return b;
}

// Like BHash, but with digits reversed	(for bloom filter)
int AHash(string a, int tbl[], int tSize) {	// Key, hashtable, table size
	int y = 0;
	for (unsigned int i = a.length(); i > 0; i--) {
		char c = a[i];
		int x = c;
		x = iPow(x, i + 1); // i+1 to eliminate x^0
		y += x;				// Otherwise, the first digit is always 1
	}
	if (y < 0) {
		y *= -1;	    
	}
	int b = y % tSize;
	tbl[b] = y;
	return b;
}

// Like BHash, but with digits reversed	(for bloom filter)
int AHash(int a, int tbl[], int tSize) {	// Key, hashtable, table size
	int y = 0;

	vector<int> digits;	// For manipulation of individual digits from a
	while (a) {
		digits.push_back(a % 10); // Separate a into individual digits
		a /= 10;
	}
	std::reverse(digits.begin(), digits.end());
	for (unsigned int i = 1; i < digits.size() - 1; i++) {
		int x = iPow(digits[i], i + 1); // i+1 to eliminate x^0 
		y += x;	// Add to hash value
	}
	int b = y % tSize;
	tbl[b] = y;
	return b;
}

class BloomFilter{ // Table size required
public:
	BloomFilter(const int size) {
		s = size;
		table = new bool[size];
		for (int i = 0; i < size; i++) {
			table[i] = 0;
		}
	}
	/*~BloomFilter() {
		delete[] table;
	} */
	void add(string x, int tbl[]) {
		int a = AHash(x, tbl, s);
		int b = BHash(x, tbl, s);
		table[a] = true;
		table[b] = true;
	}
	void add(int x, int tbl[]) {
		int a = AHash(x, tbl, s);
		int b = BHash(x, tbl, s);
		table[a] = true;
		table[b] = true;
	}
	double test(string x, int tbl[]) {
		double llHood = 0; // Likelihood of containment
		if (table[BHash(x, tbl, s)] == 1) {
			llHood++;
		}
		if (table[AHash(x, tbl, s)] == 1) {
			llHood++;
		}
		if (llHood <= 0) {
			return 0;
		}
		else {
			llHood = llHood / s;
			return llHood;
		}
	}
private:
	int s;
	bool* table; // Array placeholder; sized in constructor
};


int main() {
	
	srand(time(NULL));
	vector<int> vec;
	vector<int> vec2;
	for (int i = 0; i < 26; i++) { //Fill vectors with random integers 1-100
		unsigned int x = rand() % 100 + 1;
		vec.push_back(x);
		vec2.push_back(x);
	}
	cout << "Vector before sorting: " << endl;				 // Merge sort
	for (unsigned int i = 0; i < vec.size() - 1; i++) {
		cout << vec.at(i) << " ";
	}
	mergeSort(vec, 0, vec.size() - 1);

	cout << endl << "Vector after merge sort: " << endl;
	for (unsigned int i = 0; i < vec.size() - 1; i++) {
		cout << vec.at(i) << " ";
	} 

	cout << endl << "Vector before sorting: " << endl;		 //Quick sort
	for (unsigned int i = 0; i < vec.size() - 1; i++) {
		cout << vec.at(i) << " ";
	}
	mergeSort(vec, 0, vec.size() - 1);

	cout << endl << "Vector after quick sort: " << endl;
	for (unsigned int i = 0; i < vec2.size() - 1; i++) {
		cout << vec2.at(i) << " ";
	}

	cout << endl << "Enter number to binary search: ";		 //Binary search
	int b;
	cin >> b;
	int x = binarySearch(vec, 0, vec.size()-1, b);
	if (x >= 0) {
		cout << endl << b << " found at " << x << " index" << endl;
	}
	else {
		cout << "Number is not contained in vector" << endl;
	} 
	
	int HTable[19]; // A prime table size (not required but preferred)
	for (int i = 0; i < 19; i++) {
		HTable[i] = 0;
	}

	cout << "Table before hashing: " << endl;
	for (int i = 0; i < 19; i++) {
		cout << i << ": " << HTable[i] << endl;
	}
	
	cout << endl << "Enter integers to be hashed; press enter after each one (-1 to end) ";	 // Hashing ints

	int z;
	do {
		cin >> z;
		BHash(z, HTable, 19);
	} while (z != -1);

	cout << endl << "Enter strings to be hashed; press enter after each one (-1 to end) ";	// Hashing strings
	string y;
	do {
		cin >> y;
		BHash(y, HTable, 19);
	} while (y != "-1");

	cout << endl << "Table after hashing: " << endl;
	for (int i = 0; i < 19; i++) {
		cout << i << ": " << HTable[i] << endl;
	}
	 
	int btable[19];	
	BloomFilter a(19);
	cout << endl << "Enter strings to add to bloom filter; press enter after each one: " << endl; // Bloom filter
	do {
		cin >> y;
		a.add(y, btable);
	} while (y != "-1");
	
	do {
		cout << endl << "Enter string to search for: (-1 to end)";
		cin >> y;
		double prc = a.test(y, btable);
		cout << endl << "Likelihood of containment: " << prc << endl;
	} while (y != "-1");

	return 0;
}