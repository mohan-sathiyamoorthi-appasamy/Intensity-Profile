/**********************************************************************************
Filename	: cuda_FilterKernels.cu
Authors		: Jing Xu, Kevin Wong, Yifan Jian, Marinko Sarunic
Published	: Janurary 6th, 2014

Copyright (C) 2014 Biomedical Optics Research Group - Simon Fraser University
This software contains source code provided by NVIDIA Corporation.

This file is part of a Open Source software. Details of this software has been described 
in the papers titled: 

"Jing Xu, Kevin Wong, Yifan Jian, and Marinko V. Sarunic.
'Real-time acquisition and display of flow contrast with speckle variance OCT using GPU'
In press (JBO)
and
"Jian, Yifan, Kevin Wong, and Marinko V. Sarunic. 'GPU accelerated OCT processing at 
megahertz axial scan rate and high resolution video rate volumetric rendering.' 
In SPIE BiOS, pp. 85710Z-85710Z. International Society for Optics and Photonics, 2013."


Please refer to these papers for further information about this software. Redistribution 
and modification of this code is restricted to academic purposes ONLY, provided that 
the following conditions are met:
-	Redistribution of this code must retain the above copyright notice, this list of 
	conditions and the following disclaimer
-	Any use, disclosure, reproduction, or redistribution of this software outside of 
	academic purposes is strictly prohibited


*DISCLAIMER*
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
SHALL THE COPYRIGHT OWNERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
TORT (INCLUDING NEGLIGENCE OR OTHERWISE)ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied.
**********************************************************************************/

//This file will define all necessary items for our Queue Structure
#ifndef _BUFFER_STRUCT_
#define _BUFFER_STRUCT_


//Ensure system has enough ram to take on this many elements in the queue
//Otherwise lower this number to compensate
#define	BUFFNUM	1


struct buffer {
	bool regHost;
	unsigned short *data;
} ;

struct scanSettings
{
	int lineNumber;
	int speed6mm;
	int speed10mm;
	int noOfImages;
	float GalvoStartx10mm;
	float GalvoStarty10mm;
	float GalvoStartz10mm;
	float GalvoStarta10mm;
	float GalvoEndx10mm;
	float GalvoEndy10mm;
	float GalvoEndz10mm;
	float GalvoEnda10mm;
	float  GalvoStartx6mm;
	float  GalvoStarty6mm;
	float  GalvoStartz6mm;
	float  GalvoStarta6mm;
	float  GalvoEndx6mm;
	float  GalvoEndy6mm;
	float  GalvoEndz6mm;
	float  GalvoEnda6mm;
	int port;

	float ScanRange6mmStartX;
	float ScanRange6mmEndX;
	float ScanRange10mmStartX;
	float ScanRange10mmEndX;
};
struct galvoRanges
{
	float GalvoStartx10mm;
	float GalvoStarty10mm;
	float GalvoStartz10mm;
	float GalvoStarta10mm;
	float GalvoEndx10mm;
	float GalvoEndy10mm;
	float GalvoEndz10mm;
	float GalvoEnda10mm;
	float  GalvoStartx6mm;
	float  GalvoStarty6mm;
	float  GalvoStartz6mm;
	float  GalvoStarta6mm;
	float  GalvoEndx6mm;
	float  GalvoEndy6mm;
	float  GalvoEndz6mm;
	float  GalvoEnda6mm;
};
enum scanTypes {
	HDLINE,
	RADIAL,
	CUBE,
	CUBE_GLAUCOMA_OPTICAL_DISC,
	CUBE_GLAUCOMA_MACULA,
	HDLINE_ANTERIOR_SEGMENT,
	RADIAL_ANTERIOR_SEGMENT
};
enum hdLineScanLengths{
	HDLINESCAN_6MM,
	HDLINESCAN_10MM

};
#endif