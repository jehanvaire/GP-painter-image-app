// GP-painter-image-app.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <Windows.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;
using namespace cv;

tuple <int, int> point1;
tuple <int, int> point2;

// declare list of colors
const cv::Vec3b couleursColoriage[] = { 
	cv::Vec3b(0, 80, 205), // bleu foncé
	//cv::Vec3b(255, 255, 255), // blanc
	cv::Vec3b(38, 201, 255),
	cv::Vec3b(1, 116, 32),
	cv::Vec3b(153, 0, 0),
	cv::Vec3b(150, 65, 18),
	cv::Vec3b(17, 176, 60),
	cv::Vec3b(255, 0, 19),
	cv::Vec3b(255, 120, 41),
	cv::Vec3b(176, 112, 28),
	cv::Vec3b(153, 0, 78),
	cv::Vec3b(203, 90, 87),
	cv::Vec3b(255, 193, 38),
	cv::Vec3b(255, 0, 143),
	cv::Vec3b(254, 175, 168),
	cv::Vec3b(170, 170, 170), // gris clair
	cv::Vec3b(102, 102, 102), // gris foncé 
	cv::Vec3b(0, 0, 0), // noir
};

const cv::Scalar couleursDisponibles[] = {
	cv::Scalar(0, 0, 0),
	cv::Scalar(102, 102, 102),
	cv::Scalar(0, 80, 205),
	cv::Scalar(255, 255, 255),
	cv::Scalar(170, 170, 170),
	cv::Scalar(38, 201, 255),
	cv::Scalar(1, 116, 32),
	cv::Scalar(153, 0, 0),
	cv::Scalar(150, 65, 18),
	cv::Scalar(17, 176, 60),
	cv::Scalar(255, 0, 19),
	cv::Scalar(255, 120, 41),
	cv::Scalar(176, 112, 28),
	cv::Scalar(153, 0, 78),
	cv::Scalar(203, 90, 87),
	cv::Scalar(255, 193, 38),
	cv::Scalar(255, 0, 143),
	cv::Scalar(254, 175, 168),
};

// define a list of tuples
tuple <int, int> coordonneesCouleurs[] = {
	
	{ 722, 527 },
	//{ 597, 593 },
	{ 722, 593 },
	{ 597, 658 },
	{ 662, 661 },
	{ 722, 659 },
	{ 597, 728 },
	{ 662, 727 },
	{ 722, 728 },
	{ 597, 793 },
	{ 662, 793 },
	{ 722, 809 },
	{ 597, 861 },
	{ 662, 862 },
	{ 722, 862 },
	{ 662, 592 },
	{ 662, 525 },
	{ 597, 521 },
};

void click() {
		INPUT input = { 0 };
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		SendInput(1, &input, sizeof(input));
		ZeroMemory(&input, sizeof(input));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
SendInput(1, &input, sizeof(input));
}

void drawLine(tuple <int, int> point1, tuple <int, int> point2) {
	SetCursorPos(get<0>(point1), get<1>(point1));
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(input));
	SetCursorPos(get<0>(point2), get<1>(point2));
	Sleep(10);
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
}


int main()
{
	cv::Mat img = cv::imread("D:\\Users\\Adrien\\Pictures\\naurmal-thinking.png");

	// convert image to rgb
	cv::cvtColor(img, img, cv::COLOR_BGR2RGB);

	HWND hWND = NULL;


	cout << "Appuyez sur 1 pour definir le point en haut a gauche de la zone de dessin" << endl;
	while (true) {
		if (GetAsyncKeyState(VK_NUMPAD1)) {
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hWND, &p);
			get<0>(point1) = p.x;
			get<1>(point1) = p.y;
			cout << "Point defini en haut a gauche" << endl;
			break;
		}
	}

	cout << "Appuyez sur 2 pour definir le point en bas a droite de la zone de dessin" << endl;
	while (true) {
		if (GetAsyncKeyState(VK_NUMPAD2)) {
			POINT p;
			GetCursorPos(&p);
			ScreenToClient(hWND, &p);
			get<0>(point2) = p.x;
			get<1>(point2) = p.y;
			cout << "Point defini en bas a droite" << endl;
			break;
		}
	}

	// resize the image to fit bewteen the two points
	int width = abs(get<0>(point2) - get<0>(point1));
	int height = abs(get<1>(point2) - get<1>(point1));
	cv::resize(img, img, cv::Size(width, height));

	// for each pixel, change color to the closest color in the list
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			cv::Vec3b& pixel = img.at<cv::Vec3b>(i, j);
			int distance = 255 * 3;
			int colorIndex = -1;
			for (int k = 0; k < sizeof(couleursDisponibles) / sizeof(cv::Scalar); k++)
			{
				int newDistance = sqrt(pow((pixel[0] - couleursDisponibles[k][0]), 2) + pow((pixel[1] - couleursDisponibles[k][1]), 2) + pow((pixel[2] - couleursDisponibles[k][2]), 2));

				if (newDistance < distance)
				{
					distance = newDistance;
					colorIndex = k;
				}
			}
			pixel[0] = couleursDisponibles[colorIndex][0];
			pixel[1] = couleursDisponibles[colorIndex][1];
			pixel[2] = couleursDisponibles[colorIndex][2];
		}
	}


	// declare list of points
	vector<tuple<cv::Vec3b, tuple<tuple<int, int>, tuple<int, int>>>> lines;


	// for each rows
	for (int i = 0; i < img.rows; i++)
	{
		// create a start point
		tuple<int, int> startPoint = make_tuple(get<0>(point1), get<1>(point1) + i);
		// create a end point
		tuple<int, int> endPoint = make_tuple(get<0>(point1), get<1>(point1) + i);
		// for each columns
		for (int j = 0; j < img.cols; j++)
		{
			// get the pixel color
			cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);
			int k = j;
			// loop until we find a pixel that is not the same color as the first one
			for (; k < img.cols; k++)
			{
				cv::Vec3b pixel2 = img.at<cv::Vec3b>(i, k);
				if (pixel[0] != pixel2[0] || pixel[1] != pixel2[1] || pixel[2] != pixel2[2])
				{
					break;
				}
			}

			j = k;
			
			// update the end point
			get<0>(endPoint) = get<0>(point1) + k;
			// add the line to the list
			lines.push_back(make_tuple(pixel, make_tuple(startPoint, endPoint)));
			// update the start point
			startPoint = make_tuple(get<0>(point1) + k, get<1>(point1) + i);
		}
	}




	for (int coord = 0; coord < sizeof(coordonneesCouleurs) / sizeof(cv::Point); coord++)
	{
		SetCursorPos((get<0>(coordonneesCouleurs[coord])), (get<1>(coordonneesCouleurs[coord])));
		click();
		// draw the lines
		for (int i = 0; i < lines.size(); i++)
		{
			if (get<0>(lines[i]) == couleursColoriage[coord])
			{
				if (GetAsyncKeyState(VK_NUMPAD0)) {
					return 0;
				}
				drawLine(get<0>(get<1>(lines[i])), get<1>(get<1>(lines[i])));
			}
		}
	}

}