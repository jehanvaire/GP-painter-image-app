// GP-painter-image-app.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <Windows.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv.hpp>
#include <opencv2/highgui.hpp>

using namespace std;

// declare list of colors
const cv::Vec3b couleursColoriage[] = { 
	cv::Vec3b(0, 0, 0),
	cv::Vec3b(102, 102, 102),
	cv::Vec3b(0, 80, 205),
	cv::Vec3b(255, 255, 255),
	cv::Vec3b(170, 170, 170),
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

tuple <int, int> point1;
tuple <int, int> point2;


// define a list of tuples
tuple <int, int> coordonneesCouleurs[] = {
	{ 597, 521 },
	{ 662, 525 },
	{ 722, 527 },
	{ 597, 593 },
	{ 662, 592 },
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
	Sleep(1);
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
}



int main()
{
	cv::Mat img = cv::imread("D:\\Users\\Adrien\\Pictures\\quelle_indignite.png");

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


	


	

	//int pas = 4;

	//for (int coord = 0; coord < sizeof(coordonneesCouleurs) / sizeof(cv::Point); coord++)
	//{
	//	SetCursorPos((get<0>(coordonneesCouleurs[coord])), (get<1>(coordonneesCouleurs[coord])));
	//	click();
	//	for (int i = 0; i < img.rows; i+=pas)
	//	{
	//		for (int j = 0; j < img.cols; j+=pas)
	//		{
	//			cv::Vec3b pixel = img.at<cv::Vec3b>(i, j);
	//			if (pixel == couleursColoriage[coord]) {
	//				SetCursorPos(get<0>(point1)+j, get<1>(point1)+i);
	//				click();
	//			}
	//		}
	//	}
	//}



	
	//while (true) {
	//	Sleep(50);
	//	if (GetAsyncKeyState(VK_NUMPAD0)) {
	//		return 0;
	//	}
	//	if (GetAsyncKeyState(VK_NUMPAD1)) {// Mouseposition
	//		POINT p;
	//		GetCursorPos(&p);
	//		ScreenToClient(hWND, &p);
	//		cout << p.x << ", " << p.y << ";" << endl;
	//		Sleep(1000);
	//	}

	//	if (GetAsyncKeyState(VK_NUMPAD2)) {
	//		// move mouse to a position
	//		SetCursorPos(850, 500);
	//		

	//		INPUT input = { 0 };
	//		input.type = INPUT_MOUSE;
	//		input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	//		SendInput(1, &input, sizeof(input));
	//		ZeroMemory(&input, sizeof(input));
	//		input.type = INPUT_MOUSE;
	//		input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	//		SendInput(1, &input, sizeof(input));
	//	}
	//}
	
	
		

	

	

}