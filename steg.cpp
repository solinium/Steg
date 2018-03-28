#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <iostream>

std::string msg;
std::string key;
int datalength;
int keyindex = 0;
int dataposition = 0;
bool readorwrite;
bool argumentspecify;
void xorEncrypt();
char xorDecrypt(char printchar);
void printLoop(unsigned char *data);
unsigned char *mainLoop(unsigned char *data);
unsigned char *readBMP(char *filename);
unsigned char *writeBMP(char *filename, unsigned char *data);
char outputchar;
unsigned char *rest;
unsigned char info[54];
unsigned char picture[500];

int main(int argc, char *argv[])
{
	system("clear");
	if (argc == 1)
	{
		std::cout << "No arguments passed! try using -h\n";
		exit(1);
	}
	std::string arg = argv[1];
	if ((arg == "-h") || (arg == "--help"))
	{
		std::cout << "steg filename -r or -w (read or write)\nexample: steg test.bmp -r\n";
		exit(0);
	}
	if (argc > 3)
	{
		std::cout << "There are too many arguments. Try using -h.\n";
		exit(1);
	}
	else if (argc < 3)
	{
		std::cout << "There are aren't enough arguments specified. Try using -h.\n";
		exit(1);
	}
	int i2;
	std::string arg2 = argv[2];
	if (arg2 == "-r" || arg2 == "-R")
	{
		readorwrite = false;
		argumentspecify = true;
	}
	else if (arg2 == "-w" || arg2 == "-W")
	{
		readorwrite = true;
		argumentspecify = true;
	}
	else
	{
		std::cout << "Argument '" + arg2 + "' is not valid.\n";
		exit(1);
	}
	char *filename = new char[arg.length() + 1];
	strcpy(filename, arg.c_str());

	FILE *f;
	f = fopen(filename, "rb");
	if (f == NULL)
	{
		std::cout << "File does not exist or is null.\n";
		throw "Argument Exception";
	}
	unsigned char *data = readBMP(filename);
	if (readorwrite)
	{
		std::cout << "\nEnter your message to encrypt into the picture.\n";
		std::cin.ignore();
		getline(std::cin, msg);
		system("clear");
		std::cout << "Enter a password.\n";
		std::cin >> key;
		xorEncrypt();
		int goi = 0;
		while (goi < msg.length())
		{
			data = mainLoop(data);
			goi++;
		}
		writeBMP(filename, data);
		system("clear");
		printLoop(data);
	}
	else
	{
		std::cout << "\nEnter your password...\n";
		std::cin >> key;
		system("clear");
		printLoop(data);
	}
}

void xorEncrypt()
{
	int keylength = key.length();
	int msglength = msg.length();
	int i2;
	for (int i = 0; i < msglength; i++)
	{
		i2 = i % keylength;
		msg[i] = msg[i] ^ key[i2];
	}
}

char xorDecrypt(char printchar)
{
	int keylength = key.length();
	keyindex %= keylength;
	printchar = printchar ^ key[keyindex];
	return printchar;
}

unsigned char *readBMP(char *filename)
{
	FILE *f;
	f = fopen(filename, "rb");
	fread(info, sizeof(unsigned char), 54, f);
	int width = *(int *)&info[18];
	int height = *(int *)&info[22];
	std::cout << "Name: " << filename << std::endl;
	std::cout << "Width: " << width << std::endl;
	std::cout << "Height: " << height << std::endl;
	std::cout << std::endl;
	int row_padded = (width * 3 + 3) & (~3);
	unsigned char *data = new unsigned char[row_padded * 3];
	fread(data, sizeof(unsigned char), row_padded * 3, f);
	rest = (unsigned char *)calloc(row_padded * (height - 3), 1);
	fread(rest, sizeof(unsigned char), row_padded * (height - 3), f);
	datalength = width * 9;
	fclose(f);
	return data;
}

unsigned char *writeBMP(char *filename, unsigned char *data)
{
	FILE *f;
	f = fopen(filename, "wb");
	fwrite(info, sizeof(unsigned char), 54, f);
	int width = *(int *)&info[18];
	int height = *(int *)&info[22];
	int row_padded = (width * 3 + 3) & (~3);
	fwrite(data, sizeof(unsigned char), row_padded * 3, f);
	fwrite(rest, sizeof(unsigned char), row_padded * (height - 3), f);
	fclose(f);
	return data;
}

unsigned char *mainLoop(unsigned char *data)
{
	int goloop = 0;
	int msgposition = dataposition / 8;
	while (goloop < 7)
	{
		char location1 = 0x80;
		char combination1;
		combination1 = msg[msgposition] & location1;

		if (int(!!combination1) == 0)
		{
			data[dataposition + 0] = data[dataposition + 0] & 0xFE;
		}
		else
		{
			data[dataposition + 0] = data[dataposition + 0] | 0x01;
		}

		char location2 = 0x40;
		char combination2;
		combination2 = msg[msgposition] & location2;

		if (int(!!combination2) == 0)
		{
			data[dataposition + 1] = data[dataposition + 1] & 0xFE;
		}
		else
		{
			data[dataposition + 1] = data[dataposition + 1] | 0x01;
		}

		char location3 = 0x20;
		char combination3;
		combination3 = msg[msgposition] & location3;

		if (int(!!combination3) == 0)
		{
			data[dataposition + 2] = data[dataposition + 2] & 0xFE;
		}
		else
		{
			data[dataposition + 2] = data[dataposition + 2] | 0x01;
		}

		char location4 = 0x10;
		char combination4;
		combination4 = msg[msgposition] & location4;

		if (int(!!combination4) == 0)
		{
			data[dataposition + 3] = data[dataposition + 3] & 0xFE;
		}
		else
		{
			data[dataposition + 3] = data[dataposition + 3] | 0x01;
		}

		char location5 = 0x08;
		char combination5;
		combination5 = msg[msgposition] & location5;

		if (int(!!combination5) == 0)
		{
			data[dataposition + 4] = data[dataposition + 4] & 0xFE;
		}
		else
		{
			data[dataposition + 4] = data[dataposition + 4] | 0x01;
		}

		char location6 = 0x04;
		char combination6;
		combination6 = msg[msgposition] & location6;

		if (int(!!combination6) == 0)
		{
			data[dataposition + 5] = data[dataposition + 5] & 0xFE;
		}
		else
		{
			data[dataposition + 5] = data[dataposition + 5] | 0x01;
		}

		char location7 = 0x02;
		char combination7;
		combination7 = msg[msgposition] & location7;

		if (int(!!combination7) == 0)
		{
			data[dataposition + 6] = data[dataposition + 6] & 0xFE;
		}
		else
		{
			data[dataposition + 6] = data[dataposition + 6] | 0x01;
		}

		char location8 = 0x01;
		char combination8;
		combination8 = msg[msgposition] & location8;

		if (int(!!combination8) == 0)
		{
			data[dataposition + 7] = data[dataposition + 7] & 0xFE;
		}
		else
		{
			data[dataposition + 7] = data[dataposition + 7] | 0x01;
		}
		goloop++;
	}
	dataposition += 8;
	return data;
}

void printLoop(unsigned char *data)
{
	std::cout << "You will likely see characters that are not displayed correctly, this is normal.\n\n";
	for (int i = 0; i < datalength / 8; i++)
	{
		outputchar = 0x00;
		if (data[8 * i + 0] & (0x01 != 0))
		{
			outputchar |= 0x80;
		}
		if (data[8 * i + 1] & (0x01 != 0))
		{
			outputchar |= 0x40;
		}
		if (data[8 * i + 2] & (0x01 != 0))
		{
			outputchar |= 0x20;
		}
		if (data[8 * i + 3] & (0x01 != 0))
		{
			outputchar |= 0x10;
		}
		if (data[8 * i + 4] & (0x01 != 0))
		{
			outputchar |= 0x08;
		}
		if (data[8 * i + 5] & (0x01 != 0))
		{
			outputchar |= 0x04;
		}
		if (data[8 * i + 6] & (0x01 != 0))
		{
			outputchar |= 0x02;
		}
		if (data[8 * i + 7] & (0x01 != 0))
		{
			outputchar |= 0x01;
		}
		std::cout << xorDecrypt(outputchar);
		keyindex++;
	}
	std::cout << std::endl;
}