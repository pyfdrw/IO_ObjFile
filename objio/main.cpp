#include "mydata.h"

void fileHandle(char** filepath, FILE** objfile);
int storeGeo(FILE* objfile,ObjGeo*& objgeo);
int ouputGeo(char* dir, char* filename, ObjGeo objgeo);

int main()
{
	char* filepath;
	filepath = new char[200];
	FILE* objfile;
	fileHandle(&filepath, &objfile);
	ObjGeo* objgeo;
	storeGeo(objfile, objgeo);
	char* dir = new char[200];
	int lengthtmp = strlen(filepath);
	for (int i = 0; i < lengthtmp; i++)
	{
		if ('/' == *(filepath + lengthtmp - 1 - i) || '\\' == *(filepath + lengthtmp - 1 - i))
		{
			*(filepath + lengthtmp - i) = '\0';
			break;
		}
	}
	strcpy(dir, filepath);
	char* filename = new char[20];

	for (int i = 0; i < 200; i++)
	{
		sprintf(filename, "%d", i);
		strcat(filename,".obj");
		ouputGeo(dir, filename, *(objgeo + i));
	}
	

    for (int i = 0; i < 200; i++)
    {
        delete[](objgeo + i)->face;
        delete[](objgeo + i)->point;
        delete[](objgeo + i)->pointnormal;
    }
	delete[]filepath;
	delete[]dir;
	return 0;
}

void fileHandle(char** filepath, FILE** objfile)
{
	std::cout << "Input .obj file path:" << std::endl;
	std::cin.getline(*filepath,500);

	if (0 != (*objfile = fopen(*filepath, "r")))
	{
		for (int i = 0; i < strlen(*filepath); i++)
		{
			std::cout << *(i + *filepath);
		}
		std::cout << std::endl;
	}
	else
	{
		std::cout << "Wrong path!" << std::endl;
		exit(0);
	}
}

int storeGeo(FILE* objfile, ObjGeo*& objgeo)
{
	char* linetmp = new char[100];
	int Vcount = 0,Fcount = 0,VNcount = 0;
	char* nametmp = new char[20];
	int* Vnumtmp = new int[200]; //num of V = num of VN
	int* Fnumtmp = new int[200];
	char* endindex;
	int pointstorednum = 0;
	int pointstorednumtmp = 0;
	int* storedvnnum = new int [200];
	for (int i = 0; i < 200; i++) *(Vnumtmp + i) = 0, *(Fnumtmp + i) = 0, *(storedvnnum + i) = 0;

	while (0 != fgets(linetmp, 100, objfile)) //count Vnum and Fnum of every parts, no more than 200 organs
	{
		if (1 == strlen(linetmp))
			continue;
		*(linetmp + strlen(linetmp) - 1) = '\0';
		if ('o' == *(linetmp))
		{
			loop1:
			int geoidtmp = 0;
			sscanf(linetmp + 1, "%d", &geoidtmp);
			while (0 != (endindex=fgets(linetmp, 100, objfile)) && 'o' != *(linetmp))
			{
				if (1 == strlen(linetmp))
					continue;
				if ('v' == *(linetmp) && ' ' == *(linetmp+1))
				{
					*(Vnumtmp + geoidtmp) = *(Vnumtmp + geoidtmp) + 1;
				}
				if ('f' == *(linetmp) && ' ' == *(linetmp + 1))
				{
					*(Fnumtmp + geoidtmp) = *(Fnumtmp + geoidtmp) + 1;
				}
			}
			if (0 == endindex)
				break;
			else
			    goto loop1;
		}
	}

	objgeo = new ObjGeo[200]; //store by object name
	for (int i = 0; i < 200; i++)
	{
		(objgeo + i)->point = new Point[1 + *(Vnumtmp+i)];
		(objgeo + i)->pointnormal = new Point[1 + *(Vnumtmp + i)];
		(objgeo + i)->face = new Face[1 + *(Fnumtmp + i)];
		(objgeo + i)->facecount = 0;
		(objgeo + i)->pointcount = 0;
		(objgeo + i)->vncount = 0;
	}

	//back to the begning of file
	fseek(objfile, 0, SEEK_SET);
	while (0 != fgets(linetmp, 100, objfile))
	{
		if (1 == strlen(linetmp))
			continue;
		*(linetmp + strlen(linetmp) - 1) = '\0';
		if ('o' == *(linetmp))
		{
		loop2:
			pointstorednumtmp = pointstorednum;
			int geoidtmp = 0;
			int countcount = 0;////
			sscanf(linetmp + 1, "%d", &geoidtmp);
			float pointtmp1 = 0, pointtmp2 = 0, pointtmp3 = 0;
			int facetmp1 = 0, facetmp2 = 0, facetmp3 = 0;
			while (0 != (endindex = fgets(linetmp, 100, objfile)) && 'o' != *(linetmp))
			{
				if (1 == strlen(linetmp))
					continue;
				if ('v' == *(linetmp) && ' ' == *(linetmp + 1)) //add a v
				{
					sscanf(linetmp + 1, "%f%f%f", &pointtmp1, &pointtmp2, &pointtmp3);
					((objgeo + geoidtmp)->point + (objgeo + geoidtmp)->pointcount + 1)->setcoorX(pointtmp1);
					((objgeo + geoidtmp)->point + (objgeo + geoidtmp)->pointcount + 1)->setcoorY(pointtmp2);
					((objgeo + geoidtmp)->point + (objgeo + geoidtmp)->pointcount + 1)->setcoorZ(pointtmp3);
					(objgeo + geoidtmp)->pointcount = (objgeo + geoidtmp)->pointcount + 1;
					pointstorednumtmp++;
				}
				if ('v' == *(linetmp) && 'n' == *(linetmp + 1))
				{
					sscanf(linetmp + 2, "%f%f%f", &pointtmp1, &pointtmp2, &pointtmp3);
					((objgeo + geoidtmp)->pointnormal + (objgeo + geoidtmp)->vncount + 1)->setcoorX(pointtmp1);
					((objgeo + geoidtmp)->pointnormal + (objgeo + geoidtmp)->vncount + 1)->setcoorY(pointtmp2);
					((objgeo + geoidtmp)->pointnormal + (objgeo + geoidtmp)->vncount + 1)->setcoorZ(pointtmp3);
					(objgeo + geoidtmp)->vncount = (objgeo + geoidtmp)->vncount + 1;
				}
				if ('f' == *(linetmp) && ' ' == *(linetmp + 1))
				{
					sscanf(linetmp + 1, "%d//%*d %d//%*d %d//%*d", &facetmp1, &facetmp2, &facetmp3);
					facetmp1 -= pointstorednum; facetmp2 -= pointstorednum; facetmp3 -= pointstorednum;
					facetmp1 = facetmp1 + *(storedvnnum + geoidtmp); facetmp2 = facetmp2 + *(storedvnnum + geoidtmp); facetmp3 = facetmp3 + *(storedvnnum + geoidtmp);
					((objgeo + geoidtmp)->face + (objgeo + geoidtmp)->facecount + 1)->setpointAindex(facetmp1);
					((objgeo + geoidtmp)->face + (objgeo + geoidtmp)->facecount + 1)->setpointBindex(facetmp2);
					((objgeo + geoidtmp)->face + (objgeo + geoidtmp)->facecount + 1)->setpointCindex(facetmp3);
					(objgeo + geoidtmp)->facecount = (objgeo + geoidtmp)->facecount + 1;
				}
			}
			if (0 == endindex)
				break;
			else
			{
				pointstorednum = pointstorednumtmp;
				*(storedvnnum + geoidtmp) = (objgeo + geoidtmp)->pointcount;
				goto loop2;
			}
		}
	}

	fclose(objfile);
	return 0;
}

int ouputGeo(char* dir, char* filename, ObjGeo objgeo)
{
	if (0==objgeo.facecount)
	{
		return 0;
	}
	else
	{
		FILE* outfile;
		char* outfilepath = new char[100]; *(outfilepath) = '\0';
		strcat(outfilepath, dir); strcat(outfilepath, filename);
		std::cout << "Output file :"; puts(outfilepath);
		if (0 == (outfile = fopen(outfilepath,"w+")))
		{
			return 0;
		}
		else
		{
			fprintf(outfile, "#Rhino\n\n");
			for (int i = 1; i <= objgeo.pointcount; i++)
			{
				fprintf(outfile, "v %.13f %.13f %.13f\n", \
					(objgeo.point + i)->getcoorX(), (objgeo.point + i)->getcoorY(), (objgeo.point + i)->getcoorZ());
			}
			for (int i = 1; i <= objgeo.vncount; i++)
			{
				fprintf(outfile, "vn %.13f %.13f %.13f\n", \
					(objgeo.pointnormal + i)->getcoorX(), (objgeo.pointnormal + i)->getcoorY(), (objgeo.pointnormal + i)->getcoorZ());
			}
			for (int i = 1; i <= objgeo.facecount; i++)
			{
				fprintf(outfile, "f %d//%d %d//%d %d//%d\n", \
					(objgeo.face + i)->getpointAindex(), (objgeo.face + i)->getpointAindex(), \
					(objgeo.face + i)->getpointBindex(), (objgeo.face + i)->getpointBindex(), \
					(objgeo.face + i)->getpointCindex(), (objgeo.face + i)->getpointCindex());
			}
			fclose(outfile);
			return 1;
		}
	}
}