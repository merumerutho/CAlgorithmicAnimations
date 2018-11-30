#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define SIZEX 800
#define SIZEY 600

void generateFrame (int t, int DURATION);

	int main (int argc, char **argv[]){
	int t;
	int DURATION = (argc > 2) ? atoi ((char *) argv[2]) : 256;

	srand (time (NULL));

	// If we passed no parameters, generate all frames
	if (argc == 1){
		for (t = 0; t < DURATION; t++){
			generateFrame (t, DURATION);
		}
	}
	// Otherwise generate only a single frame
	else{
		t = atoi ((char *) argv[1]);
		generateFrame (t, DURATION);
	}
	return 0;
}

	void generateFrame (int t, int DURATION){
	double PI = acos (-1);
	unsigned char color[3];
	char path[32];
	int x, y;
	FILE *f;
	int SINEDURATION = (DURATION / 4);

	// Sequence File name
	snprintf (path, 32, "Frames/%d.ppm", t);
	// Create file
	f = fopen (path, "wb");
	// Write header
	fprintf (f, "P6\n%d %d\n255\n", SIZEX, SIZEY);
	for (y = 0; y < SIZEY; ++y){
		for (x = 0; x < SIZEX; ++x){

			/* //BUILDINGS
			color[0] = (int)((x+y)-(double)(t/2)) | (int)((double)((0xFF)*fabs(sin((double)(-x+t)*2*PI/(SIZEX+SINEDURATION))))); // t^y&(t<<4);
			color[1] = (0xFF-(x^t)-y+(t%127))+(2*t) | (int)((double)((0xFF/(t+1)%16)*fabs(cos((double)(x+y+t)*2*PI/(SIZEX+SIZEY+SINEDURATION))))); // t^y&(t<<4);
			color[2] = (char)(color[0]*fabs(sin((double)(t*2*PI/(SINEDURATION))))) ^ (char) ((int)(0xFF*sin((double)x*2*PI/(SIZEX))) & (int)((double)((y^x)*fabs(cos((double)(x+y-t)*2*PI/(SIZEX+SIZEY+SINEDURATION)))))); // x|(t^y);
			/*

			/* //BENDING THE FLOOR
			color[0] = (y+2*t*fabs(cos((double)t*(x+t)*(y+t)*2*PI/SIZEX/SIZEY/SINEDURATION)));
			color[2] = ((x+y-t)) ^ ((x/(t+1))-(x*y/(t+1))/SIZEX)+2*t;
			color[1] = color[0]^color[2];
			*/

			color[0] = (char)(255*fabs(sin((double)y*2*PI/SIZEY)+cos((double)(x+t)*2*PI/(SIZEX))))%255;
			color[1] = (char)(255*fabs(sin((double)x*2*PI/SIZEX)+cos((double)(y+t)*2*PI/(SIZEY))))%255;
			color[2] = color[1]^color[0];

			// Write the color values
			fwrite (color, 1, 3, f);
		}
	}
	fclose (f);
	return;
}
