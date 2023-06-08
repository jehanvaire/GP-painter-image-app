#include <iostream>
#include <fstream>
#include <Windows.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv.hpp>
#include <opencv2/highgui.hpp>


std::tuple <int, int> point1;
std::tuple <int, int> point2;

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
std::tuple <int, int> coordonneesCouleurs[] = {
	
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

void drawLine(std::tuple <int, int> point1, std::tuple <int, int> point2) {
	SetCursorPos(std::get<0>(point1), std::get<1>(point1));
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &input, sizeof(input));
	SetCursorPos(std::get<0>(point2), std::get<1>(point2));
	Sleep(1);
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &input, sizeof(input));
}


int main(int argc, char *argv[])
{
	std::string filename = argv[1];

	cv::Mat img = cv::imread("images/" + filename, cv::IMREAD_UNCHANGED);

	// convert image to rgba
	cv::cvtColor(img, img, cv::COLOR_BGRA2RGBA);

	std::cout << img.at<cv::Vec4b>(5, 5) << std::endl;

	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			cv::Vec4b pixel4b = img.at<cv::Vec4b>(i, j);
			if (pixel4b[3] == 0) {
				pixel4b[0] = 255;
				pixel4b[1] = 255;
				pixel4b[2] = 255;
				pixel4b[3] = 255;
				img.at<cv::Vec4b>(i, j) = pixel4b;
			}
		}
	}

	cv::cvtColor(img, img, cv::COLOR_RGBA2RGB);

	// define point 1 with the 2 and 3 arguments
	point1 = std::make_tuple(std::stoi(argv[2]), std::stoi(argv[3]));
	// define point 2 with the 4 and 5 arguments
	point2 = std::make_tuple(std::stoi(argv[4]), std::stoi(argv[5]));


	// resize the image to fit bewteen the two points
	int width = abs(std::get<0>(point2) - std::get<0>(point1));
	int height = abs(std::get<1>(point2) - std::get<1>(point1));
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
	std::vector<std::tuple<cv::Vec3b, std::tuple<std::tuple<int, int>, std::tuple<int, int>>>> lines;


	// for each rows
	for (int i = 0; i < img.rows; i++)
	{
		// create a start point
		std::tuple<int, int> startPoint = std::make_tuple(std::get<0>(point1), std::get<1>(point1) + i);
		// create a end point
		std::tuple<int, int> endPoint = std::make_tuple(std::get<0>(point1), std::get<1>(point1) + i);
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
			std::get<0>(endPoint) = std::get<0>(point1) + k;
			// add the line to the list
			lines.push_back(std::make_tuple(pixel, std::make_tuple(startPoint, endPoint)));
			// update the start point
			startPoint = std::make_tuple(std::get<0>(point1) + k, std::get<1>(point1) + i);
		}
	}


	for (int coord = 0; coord < sizeof(coordonneesCouleurs) / sizeof(cv::Point); coord++)
	{
		SetCursorPos((std::get<0>(coordonneesCouleurs[coord])), (std::get<1>(coordonneesCouleurs[coord])));
		click();
		// draw the lines
		for (int i = 0; i < lines.size(); i++)
		{
			if (std::get<0>(lines[i]) == couleursColoriage[coord])
			{
				if (GetAsyncKeyState(VK_NUMPAD0)) {
					return 0;
				}
				drawLine(std::get<0>(std::get<1>(lines[i])), std::get<1>(std::get<1>(lines[i])));
			}
		}
	}
}