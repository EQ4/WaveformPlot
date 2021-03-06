//
//  main.c
//  WaveformPlot
//
//  Created by Matthew Hosack on 8/5/13.
//  Copyright (c) 2013 Matthew Hosack. All rights reserved.
//

#include <stdio.h>
#include "portaudio.h"
#include "plotter.h"

#define SAMPLE_RATE (44100)

static PaStream* stream;
static PaError err;

static int callback(const void* input,
					void* output,
					unsigned long framesPerBuffer,
					const PaStreamCallbackTimeInfo* time_info,
					PaStreamCallbackFlags flags,
					void* userData)
{
	float* in = (float*)input;
	float* out = (float*)output;

	plotter_write(in, out, framesPerBuffer);

	return 0;
}

void print_error(PaError err){
	printf("Error: %s\n",Pa_GetErrorText(err));
}

void pa_init(){
    printf("Opening Portaudio stream\n");
	//
	// Initialize PortAudio
	////////////////////////
	err = Pa_Initialize();
	if (err != paNoError){
		print_error(err);
	}
    
	//
	// Open a stream
	////////////////
	err = Pa_OpenDefaultStream(	&stream,
                               1,
                               1,
                               paFloat32,
                               SAMPLE_RATE,
                               256,
                               callback,
                               NULL);
	if (err != paNoError){
		print_error(err);
	}
    
	//
	// Start the stream
	///////////////////
	err = Pa_StartStream(stream);
	if (err != paNoError){
		print_error(err);
	}
}

void pa_close(){
    printf("Closing Portaudio\n");
    err = Pa_CloseStream(stream);
	if (err != paNoError){
		print_error(err);
	}
    
	//
	// Terminate Port Audio. We're done!
	////////////////////////////////////
	err = Pa_Terminate();
	if (err != paNoError){
		print_error(err);
	}
}

int main(int argc, const char * argv[])
{
    plotter_init();
    pa_init();
    getchar();
    pa_close();
    plotter_close();
    return 0;
}

