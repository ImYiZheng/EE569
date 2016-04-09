//
//  main.cpp
//  image filtering for oil painting effect
//
//  created by Yi Zheng on 9/10/15.
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
	int size1 = 580;
	int size2 = 247;
	int size;
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
		if (argc >= 5){
			size = atoi(argv[4]);
		}
	}
	// Allocate image data array
	unsigned char Imagedata[size2][size1][bytesPerPixel];
	// Read image (filename specified by first argument) into image data matrix
	if (!(file=fopen(argv[1],"rb"))) {
		cout << "Cannot open file: " << argv[1] <<endl;
		exit(1);
	}
	fread(Imagedata, sizeof(unsigned char), size1*size2*bytesPerPixel, file);
	fclose(file);

	///////////////////////// INSERT YOUR PROCESSING CODE HERE /////////////////////////
	int Imagedata_channel[size2][size1][3];
	unsigned char Imagedata_new[size2][size1][3];
	int height[size1*size2];
	int width[size1*size2];
    int i,j,k,l;
    int m,n,x;
    int sum;
    //cout the frequence of four color;
	int M=4;        //number of colors for each channel
    int a[3][M];	//store four color for each channel
    int t;			//stand for each color of four color
    //convert the char value to int
    for(k=0;k<=2;k++) {
    	for(i=0;i<size2;i++) {
    		for(j=0;j<size1;j++) {
    				Imagedata_channel[i][j][k]=(int)Imagedata[i][j][k];
    			}
    	}
    }

	//Quantize all colors of the input color image
	for(l=0;l<3;l++) {
		m=0;
		n=0;
		for(k=0;k<=255;k++) {
			for(i=0;i<size2;i++) {
				for(j=0;j<size1;j++) {
				if(Imagedata_channel[i][j][l]==k) {
					height[m]=i;
					width[n]=j;
					m=m+1;
					n=n+1;
					}
				}
			}
		}
		x=0;
		t=0;
		for(k=size1*size2/M-1;k<size1*size2;k=k+size1*size2/M) {
			sum=0;
			for(j=x;j<size1*size2/M+x;j++) {
				sum = sum+Imagedata_channel[height[j]][width[j]][l];
			}
			for(j=x;j<size1*size2/M+x;j++) {
				Imagedata_new[height[j]][width[j]][l]=(unsigned char)(M*sum/(size2*size1));
			}
			x = x+size1*size2/M;
			a[l][t]=M*sum/(size2*size1);
			t=t+1;
		}
	}

	//matrix extension by relection
	int N=7;	//N*N neighborhood
	unsigned char tmp1[size2][size1+N-1][bytesPerPixel];
	unsigned char tmp2[size2+N-1][size1+N-1][bytesPerPixel];
	for (l=0;l<3;l++) {
		for(i=0;i<=size2-1;i++) {
			for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++) {
				tmp1[i][j][l] = Imagedata_new[i][j-(N-1)/2][l];
			}
		}
		for(i=0;i<=size2-1;i++) {
			for(j=0;j<(N-1)/2;j++) {
				tmp1[i][j][l] = Imagedata_new[i][(N-1)/2-j][l];
				tmp1[i][size1+(N-1)/2+j][l] = Imagedata_new[i][size1-((N-1)/2+j)][l];
			}
		}
		for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++) {
			for(j=0;j<=size1+(N-1)-1;j++) {
		     tmp2[i][j][l] = tmp1[i-(N-1)/2][j][l];
			}
		}
		for(j=0;j<=size1+(N-1)-1;j++) {
			for(i=0;i<(N-1)/2;i++) {
				tmp2[i][j][l] = tmp1[(N-1)/2-i][j][l];
				tmp2[size2+(N-1)/2+i][j][l] = tmp1[size2-((N-1)/2+i)][j][l];
			}
		}

	}
	//
	int tmp3[size2+N-1][size1+N-1][bytesPerPixel];
	for(l=0;l<3;l++) {
		for(i=0;i<size2+N-1;i++) {
			for(j=0;j<size1+N-1;j++) {
				for(x=0;x<M;x++) {
					if ((int) tmp2[i][j][l] == a[l][x]) {tmp3[i][j][l] = x;}
				}
			}
		}
	}

    //oil-painting the image
	unsigned char Imagedata_oil[size2][size1][3];
	int R,G,B;
	int max;
	int o,p,q;
	int b[M][M][M];
	for(i=(N-1)/2;i<=size2-1+(N-1)/2;i++) {
			for(j=(N-1)/2;j<=size1-1+(N-1)/2;j++) {
				for(o=0;o<M;o++) {
					for(p=0;p<M;p++) {
						for(q=0;q<M;q++) {
							b[o][p][q]=0;
						}
					}
				}
				for(m=0;m<=(N-1)/2;	m++) {
					for(n=0;n<=(N-1)/2;n++) {
						if(m==0&&n==0) {
						b[tmp3[i][j][0]][tmp3[i][j][1]][tmp3[i][j][2]]++;
						}
						if(m==0&&n!=0) {
						b[tmp3[i][j-n][0]][tmp3[i][j-n][1]][tmp3[i][j-n][2]]++;
						b[tmp3[i][j+n][0]][tmp3[i][j-n][1]][tmp3[i][j-n][2]]++;
						}
						if(n==0&&m!=0) {
						b[tmp3[i+m][j][0]][tmp3[i+m][j][1]][tmp3[i+m][j][2]]++;
						b[tmp3[i-m][j][0]][tmp3[i-m][j][1]][tmp3[i-m][j][2]]++;
						}
						if(m!=0&&n!=0) {
						b[tmp3[i+m][j+n][0]][tmp3[i+m][j+n][1]][tmp3[i+m][j+n][2]]++;
						b[tmp3[i+m][j-n][0]][tmp3[i+m][j-n][1]][tmp3[i+m][j-n][2]]++;
						b[tmp3[i-m][j+n][0]][tmp3[i-m][j+n][1]][tmp3[i-m][j+n][2]]++;
						b[tmp3[i-m][j-n][0]][tmp3[i-m][j-n][1]][tmp3[i-m][j-n][2]]++;
						}
					}
				}
				max = b[0][0][0];
				R=0;
				G=0;
				B=0;
				for(o=0;o<M;o++) {
					for(p=0;p<M;p++) {
						for(q=0;q<M;q++) {
							if(max<b[o][p][q]) {
								max=b[o][p][q];
								R=o;
								G=p;
								B=q;
							}
						}
					}
				}
				Imagedata_oil[i-(N-1)/2][j-(N-1)/2][0]=(unsigned char)a[0][R];
				Imagedata_oil[i-(N-1)/2][j-(N-1)/2][1]=(unsigned char)a[1][G];
				Imagedata_oil[i-(N-1)/2][j-(N-1)/2][2]=(unsigned char)a[2][B];
			}
		}

	// Write image data (filename specified by second argument) from image data matrix

	if (!(file=fopen(argv[2],"wb"))) {
		cout << "Cannot open file: " << argv[2] << endl;
		exit(1);
	}
	fwrite(Imagedata_oil, sizeof(unsigned char), size1*size2*3, file);
	fclose(file);

}