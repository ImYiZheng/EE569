//
//  main.cpp
//  mhc_demosaicing
//
//  created by Yi Zheng on 9/9/15.
//  version 1.0
//


#include <stdio.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])

{
	// Define file pointer and variables
	FILE *file;
	int bytesPerPixel;
	int size1 = 424;
	int size2 = 636;

	// Check for proper syntax
	if (argc < 3){
		cout << "Syntax Error - Incorrect Parameter Usage:" << endl;
		cout << "program_name input_image.raw output_image.raw [BytesPerPixel = 1] [Size = 256]" << endl;
		return 0;
	}

	// Check if image is grayscale or color
	if (argc < 4){
		bytesPerPixel = 1; // default is grey image
	}
	else {
		bytesPerPixel = atoi(argv[3]);
		// Check if size is specified
		/*
		if (argc >= 5){
			Size = atoi(argv[4]);
		}
		*/
	}

	// Allocate image data array
	unsigned char Imagedata[size2][size1][bytesPerPixel];


	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), size2*size1*bytesPerPixel, file);
	fclose(file);


	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int i,j,k,l;
	int N=5;
	int channel1=0;			//red channel
    int channel2=1;			//green channel
    int channel3=2;			//blue channel
	float val;

	//matrix extension
	unsigned char tmp1[size2][size1+N-1][bytesPerPixel];
	unsigned char tmp2[size2+N-1][size1+N-1][bytesPerPixel];
	for (l=0;l<3;l++)
	{
		for(i=0;i<=size2-1;i++)
		{
			for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++)
			{
				tmp1[i][j][l] = Imagedata[i][j-(N-1)/2][l];
			}
		}
		for(i=0;i<=size2-1;i++)
		{
			for(j=0;j<(N-1)/2;j++)
			{
				tmp1[i][j][l] = Imagedata[i][(N-1)/2-j][l];
				tmp1[i][size1+(N-1)/2+j][l] = Imagedata[i][size1-((N-1)/2+j)][l];
			}
		}
		for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++)
		{
			for(j=0;j<=size1+(N-1)-1;j++)
			{
				tmp2[i][j][l] = tmp1[i-(N-1)/2][j][l];
			}

		}
		for(j=0;j<=size1+(N-1)-1;j++)
		{
			for(i=0;i<(N-1)/2;i++)
			{
				tmp2[i][j][l] = tmp1[(N-1)/2-i][j][l];
				tmp2[size2+(N-1)/2+i][j][l] = tmp1[size2-((N-1)/2+i)][j][l];
			}
		}

	}
	//demosaicing the image
	unsigned char Imagedata_color[size2][size1][3]; 		//color image

		for(i=0;i<size2;i=i+2)		//Red channel
			{
				for(j=0;j<size1;j=j+2)
				{
					Imagedata_color[i][j][channel1]=tmp2[i+2][j+2][0];
				}
				for(k=1;k<size1;k=k+2)
				{
					val=(8*(int)tmp2[i+2][k+2-1][0]+8*(int)tmp2[i+2][k+2+1][0]+10*(int)tmp2[i+2][k+2][0]-2*(int)tmp2[i+2][k+2-2][0]-2*(int)tmp2[i+2][k+2+2][0]-2*(int)tmp2[i+2-1][k+2-1][0]-2*(int)tmp2[i+2-1][k+2+1][0]-2*(int)tmp2[i+2+1][k+2-1][0]-2*(int)tmp2[i+2+1][k+2+1][0]+1*(int)tmp2[i+2+2][k+2][0]+1*(int)tmp2[i+2-2][k+2][0])/16;
					if(val<0)
					{
						Imagedata_color[i][k][channel1]=(unsigned char)0;
						continue;
					}
					if(val>255)
					{
						Imagedata_color[i][k][channel1]=(unsigned char)255;
						continue;
					}
					Imagedata_color[i][k][channel1]=(unsigned char)val;
				}
			}
		for(i=1;i<size2;i=i+2)
			{
				for(j=0;j<size1;j=j+2)
				{
					val=(8*(int)tmp2[i+2-1][j+2][0]+8*(int)tmp2[i+2+1][j+2][0]+10*(int)tmp2[i+2][j+2][0]-2*(int)tmp2[i+2-1][j+2-1][0]-2*(int)tmp2[i+2-1][j+2+1][0]-2*(int)tmp2[i+2+1][j+2-1][0]-2*(int)tmp2[i+2+1][j+2+1][0]-2*(int)tmp2[i+2-2][j+2][0]-2*(int)tmp2[i+2+2][j+2][0]+1*(int)tmp2[i+2][j+2+2][0]+1*(int)tmp2[i+2][j+2-2][0])/16;
					if(val<0)
					{
						Imagedata_color[i][j][channel1]=(unsigned char)0;
						continue;
					}
					if(val>255)
					{
						Imagedata_color[i][j][channel1]=(unsigned char)255;
						continue;
					}
					Imagedata_color[i][j][channel1]=(unsigned char)val;
				}
				for(k=1;k<size1;k=k+2)
				{
					val=(4*(int)tmp2[i+2-1][k+2-1][0]+4*(int)tmp2[i+2-1][k+2+1][0]+4*(int)tmp2[i+2+1][k+2-1][0]+4*(int)tmp2[i+2+1][k+2+1][0]+12*(int)tmp2[i+2][k+2][0]-3*(int)tmp2[i+2-2][k+2][0]-3*(int)tmp2[i+2+2][k+2][0]-3*(int)tmp2[i+2][k+2-2][0]-3*(int)tmp2[i+2][k+2+2][0])/16;
				    if(val<0)
				    {
				    	Imagedata_color[i][k][channel1]=(unsigned char)0;
				    	continue;
				    }
				    if(val>255)
				    {
				    	Imagedata_color[i][k][channel1]=(unsigned char)255;
				    	continue;
				    }
				    Imagedata_color[i][k][channel1]=(unsigned char)val;
				}

			}

		for(i=0;i<size2;i=i+2)		//Green channel
			{
				for(j=0;j<size1;j=j+2)
				{
					val=(4*(int)tmp2[i+2-1][j+2][0]+4*(int)tmp2[i+2+1][j+2][0]+4*(int)tmp2[i+2][j+2-1][0]+4*(int)tmp2[i+2][j+2+1][0]+8*(int)tmp2[i+2][j+2][0]-2*(int)tmp2[i+2-2][j+2][0]-2*(int)tmp2[i+2+2][j+2][0]-2*(int)tmp2[i+2][j+2-2][0]-2*(int)tmp2[i+2][j+2+2][0])/16;
					if(val<0)
					{
						Imagedata_color[i][j][channel2]=(unsigned char)0;
						continue;
					}
					if(val>255)
					{
						Imagedata_color[i][j][channel2]=(unsigned char)255;
						continue;
					}
					Imagedata_color[i][j][channel2]=(unsigned char)val;
				}
				for(k=1;k<size1;k=k+2)
				{
					Imagedata_color[i][k][channel2]=tmp2[i+2][k+2][0];
				}
			}
		for(i=1;i<size2;i=i+2)
			{
				for(j=0;j<size1;j=j+2)
				{
					Imagedata_color[i][j][channel2]=tmp2[i+2][j+2][0];
				}
				for(k=1;k<size1;k=k+2)
				{
					val=(4*(int)tmp2[i+2-1][k+2][0]+4*(int)tmp2[i+2+1][k+2][0]+4*(int)tmp2[i+2][k+2-1][0]+4*(int)tmp2[i+2][k+2+1][0]+8*(int)tmp2[i+2][k+2][0]-2*(int)tmp2[i+2-2][k+2][0]-2*(int)tmp2[i+2+2][k+2][0]-2*(int)tmp2[i+2][k+2-2][0]-2*(int)tmp2[i+2][k+2+2][0])/16;
					if(val<0)
					{
						Imagedata_color[i][k][channel2]=(unsigned char)0;
						continue;
					}
					if(val>255)
					{
						Imagedata_color[i][k][channel2]=(unsigned char)255;
						continue;
					}
					Imagedata_color[i][k][channel2]=(unsigned char)val;
				}

			}
		for(i=0;i<size2;i=i+2)		//Blue channel
			{
				for(j=0;j<size1;j=j+2)
				{
					val=(4*(int)tmp2[i+2-1][j+2-1][0]+4*(int)tmp2[i+2-1][j+2+1][0]+4*(int)tmp2[i+2+1][j+2-1][0]+4*(int)tmp2[i+2+1][j+2+1][0]+12*(int)tmp2[i+2][j+2][0]-3*(int)tmp2[i+2+2][j+2][0]-3*(int)tmp2[i+2-2][j+2][0]-3*(int)tmp2[i+2][j+2+2][0]-3*(int)tmp2[i+2][j+2-2][0])/16;
					if(val<0)
					{
						Imagedata_color[i][j][channel3]=(unsigned char)0;
						continue;
					}
					if(val>255)
					{
						Imagedata_color[i][j][channel3]=(unsigned char)255;
						continue;
					}
					Imagedata_color[i][j][channel3]=(unsigned char)val;
				}
				for(k=1;k<size1;k=k+2)
				{
					val=(8*(int)tmp2[i+2+1][k+2][0]+8*(int)tmp2[i+2-1][k+2][0]+10*(int)tmp2[i+2][k+2][0]-2*(int)tmp2[i+2-2][k+2][0]-2*(int)tmp2[i+2+2][k+2][0]-2*(int)tmp2[i+2-1][k+2-1][0]-2*(int)tmp2[i+2-1][k+2+1][0]-2*(int)tmp2[i+2+1][k+2-1][0]-2*(int)tmp2[i+2+1][k+2+1][0]+1*(int)tmp2[i+2][k+2-2][0]+1*(int)tmp2[i+2][k+2+2][0])/16;
					if(val<0)
					{
						Imagedata_color[i][k][channel3]=(unsigned char)0;
						continue;
					}
					if(val>255)
					{
						Imagedata_color[i][k][channel3]=(unsigned char)255;
						continue;
					}
					Imagedata_color[i][k][channel3]=(unsigned char)val;
				}
			}
		for(i=1;i<size2;i=i+2)
			{
				for(j=0;j<size1;j=j+2)
				{
					val=(8*(int)tmp2[i+2][j+2-1][0]+8*(int)tmp2[i+2][j+2+1][0]+10*(int)tmp2[i+2][j+2][0]-2*(int)tmp2[i+2][j+2-2][0]-2*(int)tmp2[i+2][j+2+2][0]-2*(int)tmp2[i+2-1][j+2-1][0]-2*(int)tmp2[i+2-1][j+2+1][0]-2*(int)tmp2[i+2+1][j+2-1][0]-2*(int)tmp2[i+2+1][j+2+1][0]+1*(int)tmp2[i+2+2][j+2][0]+1*(int)tmp2[i+2-2][j+2][0])/16;
					if(val<0)
					{
						Imagedata_color[i][j][channel3]=(unsigned char)0;
						continue;
					}
					if(val>255)
					{
						Imagedata_color[i][j][channel3]=(unsigned char)255;
						continue;
					}
					Imagedata_color[i][j][channel3]=(unsigned char)val;
				}
				for(k=1;k<size1;k=k+2)
				{
					Imagedata_color[i][k][channel3]=tmp2[i+2][k+2][0];
				}

			}
	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_color, sizeof(unsigned char), size1*size2*3, file);
	fclose(file);


	return 0;
}
