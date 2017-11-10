#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include "Filter.h"
 
using namespace std;
 
#include "rtdsc.h"
 
// Testing rsync
 
//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);
 
int main(int argc, char **argv)
{
 
  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }
 
  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];
 
  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }
 
  Filter *filter = readFilter(filtername);
 
    double sum = 0.0;
    int samples = 0;
     
    /* Initialize and declare outside of loops to avoid repetitive
     * initializations and declarations
     */
    double sample;
   
    struct cs1300bmp *input = new struct cs1300bmp;
    struct cs1300bmp *output = new struct cs1300bmp;
 
  for (int inNum = 2; inNum < argc; inNum++) {
    string inputFilename = argv[inNum];
    string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
 
    // Replace ok variable with direct function call
     
    if (cs1300bmp_readfile( (char *) inputFilename.c_str(), input)) {
      sample = applyFilter(filter, input, output);
      sum += sample;
      samples++;
      cs1300bmp_writefile((char *) outputFilename.c_str(), output);
    }
  }
  fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);
 
}
 
struct Filter *
readFilter(string filename)
{
  ifstream input(filename.c_str());
 
  if ( ! input.bad() ) {
    int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    int div;
    input >> div;
    filter -> setDivisor(div);
    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
    int value;
    input >> value;
    filter -> set(i,j,value);
      }
    }
    return filter;
  }
}
 
 
double
applyFilter(struct Filter *filter, cs1300bmp *input, cs1300bmp *output)
{
    long long cycStart, cycStop;
    cycStart = rdtscll();
     
    /* As compared to the initial form, having a pointer set to another 
       *pointer requires multiple accesses to memory, in this fashion it only
       *requires one. Now this memory is a local variable
    */

    int d = filter -> divisor; 
    int w = input -> width;
    int h = input -> height;
    output -> width = w;
    output -> height = h;
    --w;
    --h;
     
    // Perfect squares
     
    //int get[s][s];
    /*for(int a = 0; a < s; ++a)
    {
        for(int b = 0; b < s; ++b)
        {
            get[b][a] = filter->get(a, b);
        }
    }*/
     
    /* Laterally walking through row three times then moves down column
       Unroll loops and place in proper order for C
       Placing column first allows row to be proper iteration as C reqs
     
       Caching with the w and h
       attempted implementing an initializtion after int col=1; no sig result
       Picture goes black if incrementer is changed?? 
    */
  for(int plane = 0; plane<3; plane++)
  {
    for(int col = 1; col < w; col = col + 1)
    {
        for(int row = 1; row < h; row = row + 1)
        {
            int v = 0;
      
             
            v += input -> color[col - 1][plane][row - 1] * filter -> data[0];
            v += input -> color[col][plane][row - 1] * filter -> data[1];
            v += input -> color[col + 1][plane][row - 1] * filter -> data[2];
             
             
         
            /////////////////////////////////////////////////////
        
             
             
            v += input -> color[col - 1][plane][row] * filter -> data[3];
            v += input -> color[col][plane][row] * filter -> data[4];
            v += input -> color[col + 1][plane][row] * filter -> data[5];
             
             
           
            /////////////////////////////////////////////////////
           
             
            v += input -> color[col - 1][plane][row] * filter -> data[6];
            v += input -> color[col][plane][row] * filter -> data[7];
            v += input -> color[col + 1][plane][row] * filter -> data[8];
             
             
            
             
          
            
			switch(d)
            {
                case 1:
                    break;
               default:
				v = v >>4;
            }
            
            // v /= d;
            // Checking bounds 
             
            if (v < 0) v = 0;
                     
            if (v > 255) v = 255;
       
            output -> color[col][plane][row] = v;



        }
    }
}
     
    cycStop = rdtscll();
    double diff = cycStop - cycStart;
    double diffPerPixel = diff / (w * h);
    fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
    diff, diff / (w * h));
    return diffPerPixel;
}
