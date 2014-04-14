#include "Detect.h"
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/stitching/stitcher.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <thread>

using namespace std;
using namespace cv;

Detect::Detect()
{
}


Detect::~Detect()
{
}


__int64 Detect::calcHammingDistance(__int64 x, __int64 y)
{
	__int64 dist = 0, val = x ^ y;
	while (val)
	{
		++dist;
		val &= val - 1;
	}
	return dist;
}

void Detect::sort(vector<int> arr)
{
	int i, j, key;
	for (i = 1; i < arr.size(); i++)
	{
		key = arr[i];
		j = i - 1;
		while (j >= 0 && arr[j] > key)
		{
			arr[j + 1] = arr[j];
			j--;
		}
		arr[j + 1] = key;
	}
}

void Detect::detect(vector<Square>& squares, mutex& mutex_squares)
{
	while (true)
	{
		cout << squares.size() << endl;
		mutex_squares.lock();
		for (vector<Square>::iterator iter = squares.begin(); iter != squares.end();)
		{
			Square square = *iter;
			milliseconds timeNow = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
			if ((timeNow.count() - square.getTime()) > timeRange)
			{
				//				cout << "erase " << (*iter).first << endl;
				iter = squares.erase(iter);
			}
			else
			{
				const __int64 percept = -3688730819679167236;
				__int64 hash = square.getHash();
				__int64 res = calcHammingDistance(percept, hash);
				if (res <= 4)
				{
					cout << "hand" << " " << res << endl;
					imshow("img", square.getImg());
				}
				iter++;

			}
		}
		mutex_squares.unlock();
		//		this_thread::sleep_for(chrono::milliseconds(20));
		waitKey(20);
	}
}

void Detect::training(vector<Square>& squares, mutex& mutex_squares)
{
	while (true)
	{
		cout << squares.size() << endl;
		mutex_squares.lock();
		milliseconds timeNow = duration_cast<milliseconds>(high_resolution_clock::now().time_since_epoch());
		for (vector<Square>::iterator iter = squares.begin(); iter != squares.end();)
		{
			Square square = *iter;
			if ((timeNow.count() - square.getTime()) > timeRange)
			{
				//				cout << "erase " << (*iter).first << endl;
				iter = squares.erase(iter);
			}
			else
			{
//				cout << square.getTime() << " -- " << square.getHash() << endl;
				iter++;
			}
		}

		vector <vector <int>> hammDistMat;
		const int HammingDist = 2;
		int size = squares.size();
		if (size > 1)
		{
			int i = 0;
			for (vector<Square>::iterator iter = squares.begin(); iter != squares.end(); iter++)
			{
				Square square = *iter;
				vector <int> hammDistRow;
				for (int j = 0; j < size; j++)
				{
					hammDistRow.push_back(calcHammingDistance(square.getHash(), squares[j].getHash()));
				}
				sort(hammDistRow);
				hammDistMat.push_back(hammDistRow);
				i++;
			}
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)
				{
					cout << hammDistMat[i][j] << " ";
/*
					if ((hammDistMat[i][j] < HammingDist)&(i!=j))
					{
						imgs.push_back(squares[i].getImg());
						imgs.push_back(squares[j].getImg());
					}
*/
				}
				cout << endl;
			}

		}
				/*
				string filename = "c:\\temp\\opencv\\";
				char hashtxt[256] = "";
				__int64 hash = square.getHash();
				_i64toa(hash, hashtxt, 10);
				filename = filename + hashtxt + ".png";
				//			cout << filename << endl;
				try
				{
					imwrite(filename, square.getImg());
				}
				catch (runtime_error& ex)
				{
					fprintf(stderr, "Exception converting image to PNG format: %s\n", ex.what());
					break;
				}
				*/
		
		mutex_squares.unlock();
		waitKey(20);
	}
}




