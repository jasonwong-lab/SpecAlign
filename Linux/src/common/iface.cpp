/***************************************************************************************************
File:		iface.cpp
Created:	2005/09/18
Author:		Jason W H Wong <jason.wong@chem.ox.ac.uk>
	
Purpose:	Implements the interface to access and process specAlign data objects.

License:	The stand-alone specAlign application and its source code is free to academic, 
		government and non-profit users for non-commercial use. Use of the specAlign
		application and source code in part or in whole for commercial use by non-profit 
		and for-profit entities requires a license agreement. Parties interested in 
		commercial should contact jason.wong@chem.ox.ac.uk for permission to use
		the specAlign application or its source code.

		The use of specAlign in any research must cite as appropriate:
		Wong, J.W.H., Cagney, G. and Cartwright, H.M. (2005) SpecAlign - processing and 
			alignment of mass spectra datasets. Bioinformatics. 21(9):2088-2090.
		Wong, J.W.H., Durante, C and Cartwright, H.M. (2005) Application of Fast Fourier 
			Transform Cross-Correlation for the Alignment of Large Chromatographic and 
			Spectral Datasets. Anal. Chem. 77 (17), 5655 -5661		

Disclaimer:	specAlign and all associated documents ARE PROVIDED "AS IS" WITHOUT ANY WARRANTY 
		OF ANY KIND, EITHER EXPRESS, IMPLIED, OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, 
		ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
		FREEDOM FROM INFRINGEMENT, OR THAT specAlign and all associated documents WILL 
		BE ERROR FREE. The authur MAKES NO REPRESENTATIONS THAT THE USE OF specAlign or 
		any associated documents WILL NOT INFRINGE ANY PATENT OR PROPRIETARY RIGHTS OF 
		THIRD PARTIES. In no event will the authors be liable for any damages, including 
		but not limited to direct, indirect, special or consequential damages, arising out 
		of, resulting from, or in any way connected with the use of specAlign or any associated 
		documents.
***************************************************************************************************/

#include <sys/types.h>
#include <dirent.h>

#include "convert.h"
#include "iface.h"
#include "time.h"
#include <math.h>
#include <stdlib.h>
#include "FFTReal.h"



iface::iface(){
	srand((unsigned)(time(NULL)));
}


iface::~iface(){

}

int iface::readFileInput(int type, char* filename, bool first, bool open)
{
	char buffer[65536] = "";
	memset(buffer,'*',65536);
	register int line = 0;
	vector <double> tmpmz;
	int orgsize = train.size();
	int added = 0;
	ifstream testlines((const char*)filename);
	int numLines = 0;
	char btmp[65536] ="";
	if (testlines.is_open()){
		while (!testlines.eof()){
			testlines.getline(btmp,65536);
			++numLines;
		}
		testlines.close();
	}
	else{
		return 0;
	}
	ifstream file((const char*)filename);		// opens file to read
	if (! file.is_open())
  		{ cout << "Error opening file"; return 0; }
	string fname = filename;
	string::size_type pos = fname.find_last_of("/");
	string::size_type pos2 = fname.find_last_of(".");
	string::size_type l = fname.length();
	if (pos == string::npos)
		pos = 0;
	else
		pos++;
	if (pos2 < pos)
		pos2 = l;
	fname=fname.substr(pos,pos2-pos);
	if (!open){
  		if (! file.eof())
 		{
   			file.getline (buffer,65536);
			if (!iswalnum(buffer[0]) && !iswpunct(buffer[0]) && !iswspace(buffer[0]) && !iswdigit(buffer[0]))
				return -2;
			register int i=0;
			register int j=0;
			while (!iswdigit(buffer[i])&& buffer[i] != '-'){
   				file.getline (buffer,65536);
			}
			while ((iswdigit(buffer[i])|| buffer[i] == '-'))
			{
				if (i == 0 && first){
					mz_val.push_back(Convert::ToDouble(buffer+i,strlen(buffer+i)));
					//mz_val.push_back((float) atof((const char *)buffer+i));
				}
				else if (i == 0){
					tmpmz.push_back(Convert::ToDouble(buffer+i,strlen(buffer+i)));
				}
				j =0;
				if (i!= 0){
					data newData;
					newData.input.push_back(Convert::ToDouble(buffer+i,strlen(buffer+i)));
					newData.mz_val = &mz_val;
					newData.pos = train.size();
					newData.name = fname;
					newData.name += "_";
					newData.name += itoa(train.size()+1,10);
					if (type == 0){
						train.push_back(newData);
						added++;
					}
					else if (type == 1)
						test.push_back(newData);
				}
				while ((char)(buffer[i+j]) != ','&&(char)(buffer[i+j])!='\t'&&(char)(buffer[i+j])!=' '&&(char)buffer[i+j] != '*'){
					j++;
				}
				int cr  = 1;
				while ((char)buffer[i+j+cr] == ' ' || (char)buffer[i+j+cr] == '\t')
					cr++;
				i+=j+cr;
				line++;
			}
			memset(buffer,'*',65536);
			file.getline (buffer,65536);
		}
	}
	else{
		if (!file.eof()){
			do{
				memset(buffer,'*',65536);
				file.getline(buffer,65536);
			}while (buffer[0] == '+');
		}
	}
	register unsigned int te;
	register unsigned int tr;
	while (!file.eof()){
		line = 0;
		register int i=0;
		register int j = 0;

		if (first)
			te = tr = 0;
		else{
			tr = orgsize;
			te = orgsize;
		}
		while (tr < train.size() && (iswdigit(buffer[i]) || buffer[i] == '-'))
		{
			if (i == 0 && first){
				mz_val.push_back(Convert::ToDouble(buffer+i,strlen(buffer+i)));
			}
			else if (i == 0){
				tmpmz.push_back(Convert::ToDouble(buffer+i,strlen(buffer+i)));
			}
			j =0;
			if (i!=0){
				if (type == 0){
					train.at(tr).input.push_back(Convert::ToDouble(buffer+i,strlen(buffer+i)));
					++tr;
				}
				else if (type == 1){
					test.at(te).input.push_back(Convert::ToDouble(buffer+i,strlen(buffer+i)));
					++te;
				}
			}
			while ((char)(buffer[i+j]) != ','&&(char)(buffer[i+j])!='\t'&&(char)(buffer[i+j])!=' '&&(char)buffer[i+j] != '*'){
				++j;
			}
			int cr = 1;
			while ((char)buffer[i+j+cr] == ' '|| (char)buffer[i+j+cr] == '\t')
				cr++;
			i+=j+cr;
			line++;
		}
		memset(buffer,'*',65536);
		file.getline (buffer,65536);
	}
	if (!tmpmz.empty() && (tmpmz.size() != mz_val.size() || tmpmz.front() != mz_val.front() || tmpmz.back() != mz_val.back())){ // file does not match previous
		//train.resize(orgsize);
		//AfxGetApp()->DoMessageBox(_T("Warning: New data scale does not match those of existing data, SpecAlign will automatically adjust it to fit existing data."),0,0);
//		rescaleDlg rDlg;
//		rDlg.lower_v=min(mz_val.front(),tmpmz.front());
//		rDlg.upper_v=max(mz_val.back(),tmpmz.back());
//		if (rDlg.DoModal() == IDOK){
//			if (rDlg.choice == 1)
				train.back().input = convertBothXScale(&tmpmz,&train.back().input);
// 			else if (rDlg.choice == 2){
// 				vector <double> newMZ;
// 				for (int i = rDlg.lower_v; i < rDlg.upper_v;i+=rDlg.scale_v){
// 					newMZ.push_back(i);
// 				}
// 				convertPrevXScale(newMZ);
// 				for (unsigned int i = orgsize; i < tr;++i){
// 					train.at(i).input = convertCurXScale(&tmpmz,&train.at(i).input);
// 					train.at(i).mz_val=&mz_val;
// 				}
// 			}
// 			else{
// 			//tmpmz.clear();
// 				for (unsigned int i = orgsize; i < tr;++i){
// 					train.at(i).input = InterpolXScale(&tmpmz,&train.at(i).input,orgsize);
// 				}
// 			}
// 		}
// 		else{
// 			for (unsigned int i = orgsize; i < tr;++i){
// 				train.pop_back();
// 			}
// 		}
			//return -1;
	}
	else if (mz_val.size() < 2){ // not a spectrum file.
		train.clear();
		mz_val.clear();
		return -2;
	}
	
	file.close();
	writeToBackup();

	return 1;
}

int iface::readFolderInput(char* foldername)
{
	bool warn = false;
	int choice = -1;
	double lower, upper, scale;
	DIR* dta_dir;
	struct dirent *dta_dit;
	char c_file[256]="";
	strcat(c_file,foldername);
	strcat(c_file,"/");
	int files = 0;
	double cur;
	double curmass;
	bool mzed = false;
	vector <double> tmpmz;
	if ((dta_dir = opendir(foldername)) != NULL){
		while ((dta_dit = readdir(dta_dir)) != NULL){
			tmpmz.clear();
			strcat(c_file,dta_dit->d_name);
			if (strcmp(dta_dit->d_name,"..")!= 0 && strcmp(dta_dit->d_name,".")){
				files++;
				cout << files<<" "<<c_file<<endl;
				char buffer[256] = "";
				ifstream file((const char*)c_file);		// opens file to read
				cout.flush();
				if (! file.is_open()){
					cout << "an Error opening file";
					cout.flush();
					return 0;
				}
				cur=0;
				curmass = 0;
				data d;	
				d.pos = train.size();
				bool one = false;
				while (! file.eof())
				{
					file.getline (buffer,256);
					if (!one){
						one = true;
						if (!iswalnum(buffer[0]) && !iswpunct(buffer[0]) && !iswspace(buffer[0]) && !iswdigit(buffer[0]))
							return -2;
					}
					int i = 0;
					int j = 0;
					if ((iswdigit(buffer[i])|| buffer[i] == '-')){
						while ((char)buffer[i] != ','&&(char)buffer[i] != '\t' && (char)buffer[i] != ' '){
							i++;
						}
						if (i == 256)
							continue;
						curmass=Convert::ToDouble(buffer,strlen(buffer));
						i++;
						while ((char)buffer[i] == ' '|| (char)buffer[i] == '\t')
							i++;
						//while ((char)buffer[i] != ','&&(char)buffer[i] != '\t'){
						//	i++;
						//}
						cur = Convert::ToDouble(buffer+i,strlen(buffer+i));
						if (!mzed){
							mz_val.push_back(curmass);
						}
						else
							tmpmz.push_back(curmass);
						d.input.push_back(cur);
					}
				}
				if (!mzed){
					d.mz_val = &mz_val;
					train.push_back(d);
					mzed = true;
				}
				else if (!tmpmz.empty() && tmpmz.size() == mz_val.size() && tmpmz.front() == mz_val.front() && tmpmz.back() == mz_val.back()){ // file does not match previous
					if (tmpmz.size() > 2){
						d.mz_val = &mz_val;
						train.push_back(d);
					}
				}
				else{
					if (tmpmz.size() > 2){
						if (!warn){
							cout << "Different scale... interpolating..." <<endl;
							cout.flush();
							warn = true;
							choice = 3;
						}
						d.mz_val = &mz_val;
						train.push_back(d);
						if (choice == 1)
							train.back().input = convertBothXScale(&tmpmz,&train.back().input);
						else if (choice == 2){
							lower = min(lower,tmpmz.front());
							upper = max(upper,tmpmz.back());
							vector <double> newMZ;
							for (double i = lower; i < upper;i+=scale){
								newMZ.push_back((int)i);
							}
							convertPrevXScale(newMZ);
							train.back().input = convertCurXScale(&tmpmz,&train.back().input);
						}
						else{
							train.back().input = InterpolXScale(&tmpmz,&d.input,train.size()-1);
						}
					}
				}
				file.close();
				//}
				train.back().name = dta_dit->d_name;
				string::size_type pos = train.back().name.find_last_of("/");
				string::size_type pos2 = train.back().name.find_last_of(".");
				string::size_type l = train.back().name.length();
				if (pos == string::npos)
					pos = 0;
				else
					pos++;
				if (pos2 < pos)
					pos2 = l;
					train.back().name = train.back().name.substr(pos,pos2-pos);
			}
			strcpy(c_file,"");
			strcat(c_file,foldername);
			strcat(c_file,"/");
		}
	}
	closedir(dta_dir);

	if (train.empty()){
		return -1;
	}
	writeToBackup();
	return 1;
}

int iface::readAvg(char *filename, bool open)
{
	ifstream file((const char*)filename);		// opens file to read
	if (! file.is_open())
  		{ cout << "Error opening file"; return 0; }
	double cur=0;
	double curmass = 0;
	vector <double> tmpmz;
	vector <double> tmpi;
	bool isAvg=false;
	char buffer[256] = "";
	if (!file.eof()){
		file.getline (buffer,256);
		if(buffer[0]=='A'){
			avg.avgOf = atoi(buffer+4);
			isAvg=true;
		}
	}
	while (! file.eof())
  	{
		int i = 0;
		int j = 0;
		if (isAvg)
   			file.getline (buffer,256);
		else
			isAvg=true;
		if ((iswdigit(buffer[i])|| buffer[i] == '-')){
			while ((char)buffer[i] != ','&&(char)buffer[i] != '\t'&& (char)buffer[i] != ' '){
				i++;
			}
			if (i == 256)
				continue;
			curmass=atof((char*)buffer);
			i++;
			while ((char)buffer[i] == ' '|| (char)buffer[i] == '\t')
				i++;
			cur = atof((char*)buffer+i);
			tmpmz.push_back(curmass);
			tmpi.push_back(cur);
		}
	}
	if (!tmpmz.empty() && tmpmz.size() == mz_val.size() && tmpmz.front() == mz_val.front() && tmpmz.back() == mz_val.back()){ // file does match previous
		if (tmpmz.size() > 2){
			avg.input.clear();
			avg.mz_val = &mz_val;
			avg.input = tmpi;
		}
		file.close();
	}
	else{
		file.close();
		return -1;
	}

	return 1;
}

int iface::exportPeaks(char *filename, double cutoff, double win, double iter){
	if (train.empty()){
		return 0;
	}
	ofstream out;
	out.open(filename,ios::trunc);
// 	CString fname = filename;
// 	CString text;
// 	text.SetString("Exporting peaks to ");
// 	text+=fname;
	bool avgEx = !avg.input.empty();
	if (!avgEx)
		sumSpec();
	if (avg.peaks.size()== 0){
		if (train.at(0).peaks.size()==0)
			pickPeaks(cutoff,(int)win,(int)iter,true);
		else{
			for (unsigned int i = 0; i <train.at(0).peaks.size(); ++i){
				avg.peaksAt(&train.at(0).peaks);
				//avg.addPeak(dat.train.at(0).peaks.at(i));
				//dat.avg.manualPeaks.push_back(dat.train.at(0).manualPeaks.at(i));
			}
		}
	}
	bool notE = false;
	for (unsigned int i = 0; i < train.size();++i){
		if (avg.peaks.size() != train.at(i).peaks.size())
			notE=true;
	}
	if (notE){
		this->clearPeaks(true);
		pickPeaks(cutoff,(int)win,(int)iter,true);
	}
	double lastMZ = -1;
	//for (unsigned int i = 0; i< train.size();++i){
	//	train.at(i).peaksAt(&avg.peaks);
	//}
	for (unsigned int i = 0; i< avg.peaks.size();++i){
		if (avg.peaks.at(i).mz != lastMZ){
			out<<avg.peaks.at(i).mz<<",";
			lastMZ = avg.peaks.at(i).mz;
			for (unsigned int j = 0; j < train.size(); ++j){
				if (j!= train.size()-1)
					out<<train.at(j).peaks.at(i).intn<<",";
				else
					out << train.at(j).peaks.at(i).intn<<endl;
			}
		}

	}
	if (!avgEx){
		avg.input.clear();
		avg.peaks.clear();
	}
	out.close();
	return 1;
}


int iface::exportSingleFile(char *filename, bool save)
{
	if (train.empty()){
		return 0;
	}
	
	ofstream out;
	if (!save)
		out.open(filename,ios::trunc);
	else{
		out.open(filename,ios::in|ios::trunc);
	}
	for (unsigned int i = 0; i < mz_val.size(); ++i){

		out<<mz_val.at(i)<<",";
		for (unsigned int j = 0; j < train.size(); ++j){
			if (j!= train.size()-1)
				out<<train.at(j).input.at(i)<<",";
			else
				out << train.at(j).input.at(i)<<endl;
		}
	}
	out.close();
	return 1;
}

int iface::exportMultiFile(char * foldername)
{
	if (train.empty())
		return 0;
	ofstream out;
	char t[256]="";
	string fname;	
	for (unsigned int i = 0; i < train.size(); ++i){
		if (train.at(i).name.length() == 0){
			fname = foldername;
			string tmp = itoa(i+1,10);
			fname+=tmp;
			fname+=".csv";
			out.open(fname.c_str(),ios::trunc);
		}
		else{
			fname = foldername;
			string s = train.at(i).name;
			s += ".csv";
			fname += "/";
			fname += s;
			out.open(fname.c_str(),ios::trunc);
		}
		cout << i+1<< " "<<fname.c_str()<<endl;
		for (unsigned int j = 0; j < mz_val.size(); ++j){
			if (j != mz_val.size()-1)
				out<<mz_val.at(j)<<","<<train.at(i).input.at(j)<<endl;
			else
				out<<mz_val.at(j)<<","<<train.at(i).input.at(j);
		}
		out.close();
	}
	return 1;
}

int iface::exportAvg(char * filename, bool save)
{
	if (train.empty() || avg.input.empty()){
		return 0;
	}
	
	ofstream out;
	if (!save)
		out.open(filename,ios::trunc);
	else{
		out.open(filename,ios::in|ios::app);
	}
	out<<"Avg,"<<avg.avgOf<<endl;
	for (unsigned int j = 0; j < mz_val.size(); ++j){
		if (j != mz_val.size()-1)
			out<<mz_val.at(j)<<","<<avg.input.at(j)<<endl;
		else
			out<<mz_val.at(j)<<","<<avg.input.at(j);
	}
	out.close();
	return 1;
}



void iface::normalizeInput()
{
	vector <double> mins;
	vector <double> maxs;
	mins.assign(train.at(0).input.size(), 99999);
	maxs.assign(train.at(0).input.size(), 0);	
	for (unsigned int i = 0; i < train.at(0).input.size(); ++i){
		for (unsigned int j = 0; j < train.size(); ++j){
			if (train.at(j).input.at(i) < mins.at(i)){
				mins.at(i) = train.at(j).input.at(i);
			}
		}
	}
	
	for (unsigned int i = 0; i < train.at(0).input.size(); ++i){
		for (unsigned int j = 0; j < train.size(); ++j){
			train.at(j).input.at(i) -= mins.at(i);
		}
	}

	for (unsigned int i = 0; i < train.at(0).input.size(); ++i){
		for (unsigned int j = 0; j < train.size(); ++j){
			if (train.at(j).input.at(i) > maxs.at(i)){
				maxs.at(i) = train.at(j).input.at(i);
			}
		}
	}

	for (unsigned int i = 0; i < train.at(0).input.size(); ++i){
		for (unsigned int j = 0; j < train.size(); ++j){
			if (maxs.at(i) != 0){
				train.at(j).input.at(i) /= maxs.at(i);
			}
		}
	}

	if (!test.empty()){
		for (unsigned int i = 0; i < test.at(0).input.size(); ++i){
			for (unsigned int j = 0; j < test.size(); ++j){
				test.at(j).input.at(i) -= mins.at(i);
			}
		}
		for (unsigned int i = 0; i < test.at(0).input.size(); ++i){
			for (unsigned int j = 0; j < test.size(); ++j){
				if (maxs.at(i) != 0){
					test.at(j).input.at(i) /= maxs.at(i);
				}
			}
		}
	}
}

void iface::extract(vector <int> vals)
{
	if (vals.empty() || train.empty()){
		return;
	}
	vector <data> temp;
	for (unsigned int j = 0; j < train.size(); ++j){
			data a;
			temp.push_back(a);
	}
	for (unsigned int i = 0; i < vals.size(); ++i){
		for (unsigned int j = 0; j < train.size(); ++j){
			temp.at(j).input.push_back(train.at(j).input.at(vals.at(i)));
		}
	}
	for (unsigned int j = 0; j < train.size(); ++j){
		train.at(j).input.clear();
	}
	for (unsigned int i = 0; i < train.size(); ++i){
		train.at(i).input = temp.at(i).input;
	}
	int z = train.at(0).input.size();
	if (!test.empty()){
		temp.clear();
		for (unsigned int j = 0; j < test.size(); ++j){
				data a;
				temp.push_back(a);
		}
		for (unsigned int i = 0; i < vals.size(); ++i){
			for (unsigned int j = 0; j < test.size(); ++j){
				temp.at(j).input.push_back(test.at(j).input.at(vals.at(i)));
			}
		}
			for (unsigned int j = 0; j < test.size(); ++j){
				test.at(j).input.clear();
			}
		for (unsigned int i = 0; i < test.size(); ++i){
			test.at(i).input = temp.at(i).input;
		}
	}
}

	
void iface::normalizeSpec(int start, int end)
{
	writeToLast(0);
	double TIC[1000];	
	for (unsigned int i = 0; i < train.size(); ++i){
		TIC[i] = 0;
		for (unsigned int j = start; j < end; ++j){
			TIC[i] += train.at(i).input.at(j);
		}
		TIC[i] /= train.at(i).input.size();
		int a = train.at(i).input.size();
		int b = 0;
	}
	//for (unsigned int i = train.size(); i < train.size()+test.size(); ++i){
	//	TIC[i] = 0;
	//	for (unsigned int j = 0; j < test.at(i).input.size(); ++j){
	//		TIC[i] += test.at(i).input.at(j);
	//	}
	//	TIC[i] /= test.at(i).input.size();
	//}
	double normCo = 0;
	for (unsigned int i = 0; i < train.size(); ++i){
		normCo += TIC[i];
	}
	normCo/= train.size();

	for (unsigned int i = 0; i < train.size(); ++i){
		TIC[i] = normCo/TIC[i];
		for (unsigned int j = 0; j < train.at(i).input.size(); ++j){
			train.at(i).input.at(j)*=TIC[i];
		}
	}	
	//for (unsigned int i = train.size(); i < train.size()+test.size(); ++i){
	//	TIC[i] = normCo/TIC[i];
	//	for (unsigned int j = 0; j < test.at(i).input.size(); ++j){
	//		test.at(i).input.at(j)*=TIC[i];
	//	}
	//}
	clearPeaks(false);
//	if (avg.input.size() != 0)
		//sumSpec();
//	printSpec(train.at(0).input);
}

vector <double> iface::sumSpec()
{
	avg.avgOf = train.size();
	avg_last = avg;
	vector <double> res;
	res.assign(train.at(0).input.size(),0);
	for (unsigned int i = 0; i < train.size(); ++i){		
		for (unsigned int j = 0; j < train.at(i).input.size(); ++j){
			res[j]+=train.at(i).input.at(j);
		}
	}
	for (unsigned int j = 0; j < train.at(0).input.size(); ++j){
		res[j]/=train.size();
	}
	avg.input = res;
	avg.mz_val = &mz_val;
	if (avg_backup.input.empty())
		avg_backup = avg;
	if (avg_last.input.empty())
		avg_last = avg;
	return res;
}

vector <int> iface::avgPeakList(double cutoff, int window)
{
	vector <double> sum = sumSpec();
	vector <double> res;
	vector <int> peaks;
	double max = 0;
	for (unsigned int i = 0; i < sum.size();++i){
		if (sum[i] > max){
			max = sum[i];
		}
	}
	for (unsigned int i = 0; i < sum.size();++i){
		if (sum[i+(int)floor((double)(window)/2)] > cutoff*max){
			unsigned int pos = i+(int)floor((double)(window)/2);
			bool peak = false;
			int j = 1;
			while (j <=(int)floor((double)(window)/2)){
				if (sum[pos-j] < sum[pos] && sum[pos] > sum[pos+j])
					peak = true;
				else
					peak = false;
				++j;
			}
			if (peak){
				res.push_back(sum[i+(int)floor((double)(window)/2)]);
				peaks.push_back(i);
			}
		}
	}
	return peaks;
}

void iface::pickPeaks(double cutoff, int window, int iters, bool clr)
{
	if (train.empty()){
		return;
	}
	if (clr)
		clearPeaks(false);
	bool avgExist = !avg.input.empty();
	vector <double> orgAvg;
	if (!avgExist)
		sumSpec();
	else
		orgAvg=avg.input;
	avg.SGsmooth(13);
	avg.pickPeaks(cutoff,window,iters);
	for (unsigned int a = 0; a < train.size(); ++a){
		train.at(a).peaksAt(&avg.peaks);
	}
	if (avgExist){
		avg.input = orgAvg;
		vector <mass> pK=avg.peaks;
		avg.peaks.clear();
		avg.peaksAt(&pK);
	}
	else{
		avg.input.clear();
		avg.peaks.clear();
	}
}

void iface::clearPeaks(bool manual)
{
	avg.peaks.clear();
	if (manual)
		avg.manualPeaks.clear();
	//avg.trough.clear();
	for (unsigned int i = 0; i < train.size(); ++i){
		train.at(i).peaks.clear();
		if (manual)
			train.at(i).manualPeaks.clear();
	}
}

void iface::alignPeaks(int margin, double cutoff, double window, int peakIter,double low, double up)
{
	writeToLast(2);
	double closest = 999999;
	double insDVal = 0;
	double insIVal = 0;
	register unsigned int lastp = 0;
	register unsigned int lastt = 0;
	register unsigned int nextt = 0;
	register unsigned int tpos = 0;
	register unsigned int ntpos = 0;	
	int curpos = 0;
	int oldp = 0;
	bool past = false;
	bool bnext = false;
	if (avg.peaks.size() == 0)
		pickPeaks(cutoff,(int)window,peakIter, true);	
	for (unsigned int i = 0; i < train.size(); ++i){
		lastp = 0;
		tpos = 0;
		lastt = 0;
		vector <double> last_del;
		for (unsigned int j = 0; j < avg.peaks.size(); ++j){
			if (avg.peaks.at(j).mz >= low && avg.peaks.at(j).mz <= up){
			//	k = lastp;
				closest = 999999;
				past = false;
				for (unsigned int k = lastp; k < train.at(i).peaks.size() && !past ; ++k){		// find matching peak
					if (fabs(train.at(i).peaks.at(k).mz-avg.peaks.at(j).mz) > fabs(closest)){
						past = true;
					}
					else {
						closest = train.at(i).peaks.at(k).mz-avg.peaks.at(j).mz;
						lastp = k;
					}
				}
				bnext = false;
				if (j != avg.peaks.size()-1 && lastp != train.at(i).peaks.size() && fabs(closest) > fabs(train.at(i).peaks.at(lastp).mz-avg.peaks.at(j+1).mz)){
					bnext = true;
				}
				double realm = 1;				
				if (lastp < train.at(i).peaks.size() && train.at(i).peaks.at(lastp).pos+margin < mz_val.size())
					realm=fabs(train.at(i).peaks.at(lastp).mz-mz_val.at(train.at(i).peaks.at(lastp).pos+margin));
				if (closest != 999999 && closest != 0 && fabs(closest) < realm && !bnext){  // if there is a peak present and are not already aligned and close enough
					bool doNext = true;
					//unsigned int k = 0;
					if (lastp == 0)
						lastt = train.at(i).findTrough(getXIndex(low),train.at(i).peaks.at(lastp).pos);
					else
						lastt = train.at(i).findTrough(train.at(i).peaks.at(lastp-1).pos,train.at(i).peaks.at(lastp).pos);					
					if (lastp == train.at(i).peaks.size()-1)
						nextt = train.at(i).findTrough(train.at(i).peaks.at(lastp).pos,getXIndex(up));
					else
						nextt = train.at(i).findTrough(train.at(i).peaks.at(lastp).pos,train.at(i).peaks.at(lastp+1).pos);
					double nval = mz_val.at(nextt);
					double tval = mz_val.at(lastt);
					ntpos = nextt;
					tpos = lastt;
					
					if (ntpos >= train.at(i).input.size())
						insDVal = 0;
					else
						insDVal = train.at(i).input.at(ntpos);
					 
					if (tpos >= train.at(i).input.size())
						insIVal = 0;
					else
						insIVal = train.at(i).input.at(tpos);

					//curpos = getXIndex(train.at(i).peaks.at(lastp).mz);
					curpos = train.at(i).peaks.at(lastp).pos;
					int count = 0;
					vector <double> ins;
					vector <double> cur_del;
					if (closest < 0){  // insertion
						if (oldp+1 != lastp)
							last_del.clear();
						if (lastp == 0 || abs(avg.peaks.at(j).pos-curpos) < 6 || train.at(i).input.at(tpos) > 1)
							ins.resize(abs(avg.peaks.at(j).pos-curpos),insIVal);
						else
							ins = calInsert(tpos,train.at(i).peaks.at(lastp-1).pos,train.at(i).peaks.at(lastp).pos,abs(avg.peaks.at(j).pos-curpos),i);
						while (curpos < avg.peaks.at(j).pos && curpos < train.at(i).input.size()){
							if (last_del.empty())
								train.at(i).input.insert(train.at(i).input.begin()+tpos+count,ins[count]);
							else{
								train.at(i).input.insert(train.at(i).input.begin()+tpos-1,last_del.back());
								last_del.pop_back();
							}
							count++;
							curpos++;
							if (doNext){
								//if (count%2==0){
								//	cur_del.push_back(train.at(i).input.at(ntpos));
								//	train.at(i).input.erase(train.at(i).input.begin()+ntpos);
								//}
								//else{
									cur_del.insert(cur_del.begin(),train.at(i).input.at(ntpos-1));
									train.at(i).input.erase(train.at(i).input.begin()+ntpos-1);
								//}
							}
						}
						last_del = cur_del;
						cur_del.clear();
						//smooth(tpos-10,tpos+count+10,i);
					}
					else{				// deletion
						if (lastp == train.at(i).peaks.size()-1 || abs(avg.peaks.at(j).pos-curpos) < 6 || train.at(i).input.at(ntpos) > 1)
							ins.resize(abs(avg.peaks.at(j).pos-curpos),insDVal);
						else
							ins = calInsert(ntpos,train.at(i).peaks.at(lastp).pos,train.at(i).peaks.at(lastp+1).pos,abs(avg.peaks.at(j).pos-curpos),i);
						while (curpos > avg.peaks.at(j).pos && curpos > 0 && tpos < train.at(i).input.size()){
		//					if (count%2==0)
								train.at(i).input.erase(train.at(i).input.begin()+tpos);
		//					else
		//						train.at(i).input.erase(train.at(i).input.begin()+tpos-1);
							curpos--;
							if (doNext){
								train.at(i).input.insert(train.at(i).input.begin()+ntpos,ins[count]);
								count++;
							}
						}
						last_del.clear();
						//smooth(ntpos-10,ntpos+count+10,i);
					}
					train.at(i).input.resize(mz_val.size(), 0);
					oldp = lastp;
					lastp++;
					//train.at(i).peaks.clear();
					//train.at(i).pickPeaks(cutoff,window,peakIter);
				}
				else
					last_del.clear();
			}
		}
		//train.at(i).peaks.clear();
		//train.at(i).pickPeaks(cutoff,window,peakIter);
	}
}

void iface::convertToRelative()
{
	writeToLast(1);
	clearPeaks(false);
	double max = 0;
	double min = 999999;
	for (unsigned int i =0; i < train.size(); ++i){
		for (unsigned int j = 0; j < train.at(i).input.size(); ++j){
			if (train.at(i).input.at(j) < min)
				min = train.at(i).input.at(j);
		}
		for (unsigned int j = 0; j < train.at(i).input.size(); ++j){
			train.at(i).input.at(j) -= min;
		}
		for (unsigned int j = 0; j < train.at(i).input.size(); ++j){
			if (train.at(i).input.at(j) > max)
				max = train.at(i).input.at(j);
		}
		for (unsigned int j = 0; j < train.at(i).input.size(); ++j){
			train.at(i).input.at(j) /= max;
		}
		max = 0;
		min = 999999;
	}
	for (unsigned int j = 0; j < avg.input.size(); ++j){
		if (avg.input.at(j) < min)
			min = avg.input.at(j);
	}
	for (unsigned int j = 0; j < avg.input.size(); ++j){
		avg.input.at(j) -= min;
	}
	for (unsigned int j = 0; j < avg.input.size(); ++j){
		if (avg.input.at(j) > max)
			max = avg.input.at(j);
	}
	for (unsigned int j = 0; j < avg.input.size(); ++j){
		avg.input.at(j) /= max;
	}	
}

void iface::printSpec(vector<double> spec)
{
	ofstream p("spec.csv");
	for (unsigned int i = 0; i < spec.size(); ++i)
	{
		p << spec.at(i) << endl;
	}
	p.close();

}

void iface::printSpec(vector<int> spec)
{
	ofstream p("peaks.csv");
	for (unsigned int i = 0; i < spec.size(); ++i)
	{
		p << spec.at(i) << endl;
	}
	p.close();

}

void iface::writeToBackup()
{
	backup = train;
	avg_backup = avg;
	backup_mz_val = mz_val;
}

void iface::copyFromBackup()
{
	train = backup;
	avg = avg_backup;
	mz_val = backup_mz_val;
}

void iface::writeToLast(int type)
{
	lastType = type;
	last = train;
	avg_last = avg;
	last_mz_val = mz_val;
}

int iface::copyFromLast()
{
	train = last;
	avg = avg_last;
	mz_val = last_mz_val;
	return lastType;
}

int iface::getXIndex(double x)
{
	if (x < mz_val.front())
		return 0;
	if (x > mz_val.back())
		return mz_val.size()-1;
	register int i = 0;
	while (i < mz_val.size() && mz_val.at(i) < x){
		++i;
	}
	return i;
}

void iface::reverseSpec()
{
	avg.reverseInput();
	for (unsigned int i = 0; i < train.size(); ++i){
		train.at(i).reverseInput();
	}
}

int iface::removeSpec(int i)
{
	if (i < 0 || i >= train.size()){
		return 0;
	}
	else{
		writeToLast(4);
		train.erase(train.begin()+i);
		for (int j = i; j < train.size(); ++j){
			train.at(j).pos = j;
		}
	}
	return 1;
}

int iface::moveSpec(int oldp, int newp)
{
	if (oldp < 0 || oldp > train.size() || newp < 0 || newp > train.size()){
		return 0;
	}
	else{
		writeToLast(5);
		data tmp = train.at(oldp);
		train.erase(train.begin()+oldp);
		train.insert(train.begin()+newp,tmp);
		for (int i = 0; i < train.size(); ++i){
			train.at(i).pos = i;
		}
	}
	return 1;
}

int iface::saveData(char* filename)
{
	if (train.empty())
		return 0;
	ofstream out(filename,ios::trunc);
	out << "+625R@!"<<endl;
	for (unsigned int i = 0; i < train.size(); ++i){	
		out<<'+'<<train.at(i).name<<','<<train.at(i).cls<<','<<train.at(i).desc<<endl;
	}
	out.close();
	return 1;
}

int iface::readData(char* filename)
{
	ifstream file((const char*)filename);
	char buffer[65536] = "";
	if (! file.is_open()){
		return 0;
	}
	if (! file.eof()){
		file.getline(buffer,65536);
		string test = buffer;
		if (test == "+625R@!"){
			file.getline(buffer,65536);
			int i = 0;
			while (!file.eof() && buffer[0] == '+'){
				test = buffer;
				data newData;
				i = 1;
				newData.name = strtok(buffer,",");
				string tmp = strtok(NULL,",");
				newData.pos = atoi(tmp.c_str());
				newData.desc = strtok(NULL,",");
				newData.mz_val = &mz_val;
				train.push_back(newData);
				file.getline(buffer,65536);
			}
		}
		else{
			file.close();
			return -1;
		}
	}
	writeToBackup();
	file.close();
	return 1;
}

void iface::binSpectra(double binsize)
{
	writeToLast(6);
	vector <double> newMZ;
	if (avg.input.size() != 0){
		avg.binData(binsize);
	}
	for (unsigned int i =0; i < train.size(); ++i){
		newMZ = train.at(i).binData(binsize);
	}
	mz_val = newMZ;
}

void iface::smoothSpec(int pass)
{
	writeToLast(7);
	for (unsigned int i =0; i < train.size(); ++i){
		train.at(i).SGsmooth(pass);

	}
	if (avg.input.size() != 0)
		avg.SGsmooth(pass);
}

void iface::mergeAvg()
{
	if (avg.input.empty()){
		sumSpec();
		return;
	}
	if (avg.avgOf == 0 || avg.input.size() == 0)
		return;
	vector <double> mAvg;
	mAvg = avg.input;
	for (unsigned int i = 0; i < mz_val.size(); ++i){
		mAvg.at(i)*=avg.avgOf;
	}
	avg.avgOf += train.size();
	avg_last = avg;
	for (unsigned int i = 0; i < train.size(); ++i){		
		for (unsigned int j = 0; j < train.at(i).input.size(); ++j){
			mAvg.at(j)+=train.at(i).input.at(j);
		}
	}
	for (unsigned int j = 0; j < train.at(0).input.size(); ++j){
		mAvg.at(j)/=avg.avgOf;
	}
	avg.input = mAvg;
	avg.mz_val = &mz_val;
	if (avg_backup.input.empty())
		avg_backup = avg;
	if (avg_last.input.empty())
		avg_last = avg;	
}

void iface::scale(double factor)
{
	if (factor == 0 || train.size() == 0)
		return;
	writeToLast(8);
	for (unsigned int i = 0; i < train.size(); ++i){
		train.at(i).scale(factor);
	}
	if (avg.input.size() != 0)
		avg.scale(factor);
}

void iface::subtractBase(int subVal)
{
	if (train.size() == 0)
		return;
	writeToLast(9);
	for (unsigned int i = 0; i < train.size(); ++i){
		train.at(i).subtractBase(subVal);
	}
	if (avg.input.size() != 0)
		avg.subtractBase(subVal);
}

void iface::addPeak(double mz)
{
	if (train.size() == 0)
		return;
	int mpos = getXIndex(mz);
	if (mpos == -1)
		return;
	writeToLast(10);
	for (unsigned int i = 0; i < train.size(); ++i){
		train.at(i).addPeak(mpos);
		train.at(i).manualPeaks.push_back(mpos);
	}
	if (avg.input.size() != 0){
		avg.addPeak(mpos);
		avg.manualPeaks.push_back(mpos);
	}
}

void iface::removePeaks(double minmz, double maxmz)
{
	if (train.size() == 0)
		return;
	writeToLast(11);
	for (unsigned int i = 0; i < train.size(); ++i){
		train.at(i).removePeaks(minmz,maxmz);
	}
	if (avg.input.size() != 0)
		avg.removePeaks(minmz,maxmz);
}

vector <double> iface::calInsert(int inspos, int lastp, int nextp, int numIns, int samp)
{
	vector <double> res;
	if (inspos + numIns < 0 || inspos - numIns-1 >=train.at(samp).input.size() ){
		for (int i = 0; i < numIns; ++i){
			res.push_back(train.at(samp).input.at(inspos));
		}
		return res;
	}

	int add = 0;
	if (numIns%2 != 0)
		add = 1;
	for (int i = 0; i < (numIns*0.5)+add; ++i){
		//sGrad*=0.25;
		//sOff = train.at(samp).input.at(start)-(sGrad*mz_val.at(start));
	//	res.push_back(sGrad*mz_val.at(inspos+i)+sOff);
		res.push_back(train.at(samp).input.at((int)(inspos-floor((double)(i)))));
	//	res.push_back(train.at(samp).input.at(inspos)*(1-(i*0.01)));
	}
	for (int i = 0; i < (numIns*0.5); ++i){
		//eGrad*=0.25;
		//eOff = train.at(samp).input.at(end-numIns)-(eGrad*mz_val.at(end));
		//res.push_back(eGrad*mz_val.at(inspos+i)+eOff);
		res.push_back(train.at(samp).input.at((int)(inspos+floor((double)(i)))+1));
	//	res.push_back(train.at(samp).input.at(inspos)*(1-(((numIns*0.5)-i)*0.01)));
	}
	return res;
}

void iface::smooth(int start, int end, int in)
{
	//static const int l_filter[] = {-2,3,6,7,6,3,-2};
	//static const int m_filter[] = {-21,-6,7,18,27,34,39,42,43,42,39,34,27,18,7,-6,-21};
	//static const int h_filter[] = {-253,-138,-33,62,147,222,287,322,387,422,447,462,467,
	//				         462,447,422,387,322,287,222,147,62,-33,-138-253};
	int filter[50];
	int h;
	int norm;
	//if (pass <= 5){
	const int l_filter[] = {-2,3,6,7,6,3,-2};
	h = 3;
	norm = 21;
	for (unsigned int i = 0; i < h*2+1; ++i)
		filter[i] = l_filter[i];
	//}
	//else if (pass > 5 && pass <= 10){
	//	const int m_filter[] = {-21,-6,7,18,27,34,39,42,43,42,39,34,27,18,7,-6,-21};
	//	h = 8;
	//	norm = m_norm;
	//	for (unsigned int i = 0; i < h*2+1; ++i)
	//		filter[i] = m_filter[i];
	//}
	//else{
	//	const int h_filter[] = {-253,-138,-33,62,147,222,287,322,387,422,447,462,467,
	//				         462,447,422,387,322,287,222,147,62,-33,-138,-253};
	//	h = 12;
	//	norm = h_norm;
	//	for (int i = 0; i < h*2+1; ++i)
	//		filter[i] = h_filter[i];
	//}
	if (start -h < 0 || end +h >= train.at(in).input.size())
		return;
	vector <double> sInput;
	//for (unsigned int i = 0; i < h; ++i){
	//	sInput.push_back(train.at(in).input.at(i));
	//}
	double sum = 0;
	for (unsigned int i = start; i < end; ++i){
		sum = 0;
		for (int j = -h; j < h+1; ++j){
			sum += filter[j+h]*(train.at(in).input.at(i+j));
		}
		sInput.push_back(sum/norm);
	}
	//for (unsigned int i = 0; i < h; ++i){
	//	sInput.push_back(train.at(in).input.at(train.at(in).input.size()-h));
	//}
	for (unsigned int i = start+10; i < end-10; ++i){
		train.at(in).input.at(start+10) = sInput.at(i-start);
	}	
}
	
void iface::scalePositive()
{
	if (train.size() == 0)
		return;
	writeToLast(11);
	if (avg.input.size() != 0){
		avg.scalePositive();
	}
	for (unsigned int i = 0; i < train.size(); ++i){
		train.at(i).scalePositive();
	}
}

void iface::removeNegative()
{
	if (train.size() == 0)
		return;
	writeToLast(12);
	if (avg.input.size() != 0){
		avg.removeNegative();
	}
	for (unsigned int i = 0; i < train.size(); ++i){
		train.at(i).removeNegative();
	}
}

int iface::combineSpec(vector <int> samp)
{
	if (samp.empty())
		return 0;
	data combine;
	combine.mz_val = &mz_val;
	combine.pos = 0;
	combine.input.assign(mz_val.size(),0);
	for (unsigned int i = 0; i < samp.size(); ++i){
		for (unsigned int j = 0; j < mz_val.size(); ++j){
			combine.input.at(j) += train.at(samp.at(i)-1).input.at(j);
		}
	}
	for (unsigned int j = 0; j < mz_val.size(); ++j){
		combine.input.at(j) /= samp.size();
	}
	combine.name = "Combine";
	
	train.insert(train.begin(),combine);
//	train.push_back(combine);

	return 1;
}

int iface::subtractSpec(int spec1, int spec2)
{
	if (spec1 < 0 || spec1>train.size() || spec2 < 0 || spec2 > train.size()){
		return 0;
	}
	data sub;
	sub.mz_val = &mz_val;
	sub.pos = 0;
	for (unsigned int j = 0; j < mz_val.size(); ++j){
		sub.input.push_back(train.at(spec1-1).input.at(j)-train.at(spec2-1).input.at(j));
	}
	sub.name = "Subtract";
	train.insert(train.begin(),sub);
	return 1;
}

int iface::subtractAvgSpec(int spec1)
{
	if (spec1 < 0 || spec1>train.size()){
		return 0;
	}
	data sub;
	sub.mz_val = &mz_val;
	sub.pos = 0;
	for (unsigned int j = 0; j < mz_val.size(); ++j){
		sub.input.push_back(avg.input.at(j)-train.at(spec1-1).input.at(j));
	}
	sub.name = "Subtract";
	train.insert(train.begin(),sub);
	return 1;
}

double iface::cal_dist(int spec1, int spec2)
{
       register double total_dist = 0;
       for (register unsigned int i = 0; i < train.at(spec1).input.size(); ++i)
       {
       	   total_dist += pow((double)(train.at(spec1).input.at(i)-train.at(spec2).input.at(i)),2);
       }
       return sqrt(total_dist);
}

double iface::cal_R(int spec1, int spec2)
{
	double sumAB = 0;
	double sumA2 = 0;
	double sumB2 = 0;
	double sumA = 0;
	double sumB = 0;
	double N = mz_val.size();
	data * second;
	data * first = &train.at(spec1);
	if (spec2 == -1)
		second = &avg;
	else
		second = &train.at(spec2);
	for (register unsigned int i = 0; i < N; ++i)
	{
		sumAB += (first->input.at(i))*(second->input.at(i));
		sumA += first->input.at(i);
		sumB += second->input.at(i);
		sumA2 += (first->input.at(i))*(first->input.at(i));
		sumB2 +=  (second->input.at(i))*(second->input.at(i));
	}
	double bot1 = (N*sumA2)-(sumA*sumA);
	double bot2 = (N*sumB2)-(sumB*sumB);
	if (bot1 <= 0)
		return 0;
	if (bot2 <= 0)
		return 0;
	double r = ((N*sumAB)-(sumA*sumB))/sqrt((bot1)*(bot2));
	return (r*r);
}

double iface::cal_cov(int spec1, int spec2, int start, int end)
{
	int N = end-start;
	if (N < 0)
		return 0;
	data * second;
	data * first = &train.at(spec1);
	if (spec2 == -1)
		second = &avg;
	else
		second = &train.at(spec2);
	double meanA = 0;
	double meanB = 0;
	double sumAB = 0;
    for (register unsigned int i = start; i < end; ++i)
    {
		meanA += first->input.at(i);
		meanB += second->input.at(i);
    }
	meanA/=N;
	meanB/=N;
    for (register unsigned int i = start; i < end; ++i)
    {
		sumAB += (first->input.at(i)-meanA)*(second->input.at(i)-meanB);
    }
    return fabs(sumAB/N);
}

double iface::cal_partition_sca(int spec1, int spec2, int psize)
{
	double total = 0;
	double sumAB = 0;
	double sumA2 = 0;
	double sumB2 = 0;
	int parts = (int)floor((double)(train.at(spec1).input.size())/psize);
	if (parts == 0)
		parts = 1;
	int max;
	for (unsigned int i = 0;i < parts; ++i){
		sumAB = 0;
		sumA2 = 0;
		sumB2 = 0;
		if (i == parts-1)
			max = train.at(spec1).input.size();
		else
			max = psize*(i+1);
		for (unsigned int j = psize*i; j < max; ++j)
		{
			sumAB += train.at(spec1).input.at(j)*train.at(spec2).input.at(j);
			sumA2 += train.at(spec1).input.at(j)*train.at(spec1).input.at(j);
			sumB2 += train.at(spec2).input.at(j)*train.at(spec2).input.at(j);
		}
		total += acos(sumAB/sqrt(sumA2*sumB2));
	}
	return total;
}

double iface::cal_sca(int spec1, int spec2)
{
	double sumAB = 0;
	double sumA2 = 0;
	double sumB2 = 0;
    for (register unsigned int i = 0; i < train.at(spec1).input.size(); ++i)
    {
		sumAB += train.at(spec1).input.at(i)*train.at(spec2).input.at(i);
		sumA2 += train.at(spec1).input.at(i)*train.at(spec1).input.at(i);
		sumB2 += train.at(spec2).input.at(i)*train.at(spec2).input.at(i);
    }
    return acos(sumAB/sqrt(sumA2*sumB2)); //=CosT (cos0 is identical, cos90 is most different)
}

double iface::cal_si(int spec1, int spec2)
{
	double total = 0;
    for (register unsigned int i = 0; i < train.at(spec1).input.size(); ++i)
    {
		if (train.at(spec1).input.at(i) == 0 && train.at(spec2).input.at(i) == 0)
			total += 0;
		else
			total += pow(((fabs(train.at(spec1).input.at(i)-train.at(spec2).input.at(i))/(double)(train.at(spec1).input.at(i)+train.at(spec2).input.at(i))))*100,2);
    }
	return sqrt(total/train.at(spec1).input.size());
}

double iface::cal_sca(int spec, int start, int end, int shift)
{
	double sumAB = 0;
	double sumA2 = 0;
	double sumB2 = 0;
    for (register unsigned int i = start; i < end; ++i)
    {
		sumAB += train.at(spec).input.at(i)*avg.input.at(i+shift);
		sumA2 += train.at(spec).input.at(i)*train.at(spec).input.at(i);
		sumB2 += avg.input.at(i)*avg.input.at(i+shift);
    }
    return acos(sumAB/sqrt(sumA2*sumB2)); //=CosT (cos0 is identical, cos90 is most different)
}

double iface::cal_dist(int spec, int start, int end, int shift)
{
       register double total_dist = 0;
       for (register unsigned int i = start; i < end; ++i)
       {
       	   total_dist += pow((double)(train.at(spec).input.at(i)-avg.input.at(i+shift)),2);
       }
       return sqrt(total_dist);
}

double iface::cal_R(int spec1, int spec2, int start, int end, int shift)
{
	double sumAB = 0;
	double sumA2 = 0;
	double sumB2 = 0;
	double sumA = 0;
	double sumB = 0;
	double N = end-start;
	double meanA = 0;
	double meanB = 0;
	data * second;
	data * first = &train.at(spec1);
	if (spec2 == -1)
		second = &avg;
	else
		second = &train.at(spec2);
	for (register unsigned int i = start; i < end; ++i)
	{
		sumAB += (first->input.at(i))*(second->input.at(i+shift));
		sumA += first->input.at(i);
		sumB += second->input.at(i+shift);
		sumA2 += (first->input.at(i))*(first->input.at(i));
		sumB2 +=  (second->input.at(i+shift))*(second->input.at(i+shift));
	}
	double bot1 = (N*sumA2)-(sumA*sumA);
	double bot2 = (N*sumB2)-(sumB*sumB);
	if (bot1 <= 0)
		return 0;
	if (bot2 <= 0)
		return 0;
	double r = ((N*sumAB)-(sumA*sumB))/sqrt((bot1)*(bot2));
	return (r*r);
}

void iface::preshiftSpec(int win, int segs, double low, double up)
{
	if (avg.input.size() == 0)
		return;
	int start = getXIndex(low);
	int end = getXIndex(up);
	vector <int> pos;
	double bestSCA = 90;
	double secbestSCA = 90;
	vector <double> storedel;
	vector <double> curdel;
	int	frame = win;
	writeToLast(13);

	for (unsigned int i = 0; i < train.size(); ++i){
		pos = fragmentate(i,segs, start, end);
		for (unsigned int j = 0; j < pos.size()-1; ++j){
			bestSCA = 99999;
			secbestSCA = 99999;
			int bestShift = 0;
			int secbestShift = 0;
			frame = (int)floor((double)(win)/2);
			int k = -frame;
			int curstart = pos.at(j);
			int curend = pos.at(j+1);
			double curSCA;
			int incr = (int)log((float)(win));
			if (incr < 0) incr = 1;
			bool end = false;
			while (k <= frame && !end){
				curstart = pos.at(j);
				curend = pos.at(j+1);
				if (curstart+k < 0){
					curstart = pos.at(j)-k;
				}
				if (curend+k >= train.at(i).input.size()){
					curend = pos.at(j+1)-k;
				}
				//if (fitMethod == 0)
				//	curSCA=cal_dist(i,curstart,curend,k);
				//else
					curSCA=1-cal_R(i,-1,curstart,curend,k);
				if (curSCA < bestSCA){
					secbestSCA = bestSCA;
					secbestShift = bestShift;
					bestSCA = curSCA;
					bestShift = k;
				}
				else if (curSCA < secbestSCA){
					secbestSCA = curSCA;
					secbestShift = k;
				}
				k+=incr;
				if (k > frame){
					k = frame;
					end = true;
				}
			}
			if (secbestShift < bestShift){
				k = secbestShift+1;
				if (bestShift+incr<frame)
					frame = bestShift+incr;
				else
					frame = bestShift;
			}
			else{
				if (bestShift-incr<-frame)
					k = bestShift-incr;
				else
					k = bestShift+1;
				frame = secbestShift;
			}
			while (k < frame){
				curstart = pos.at(j);
				curend = pos.at(j+1);
				if (curstart+k < 0){
					curstart = pos.at(j)-k;
				}
				if (curend+k >= train.at(i).input.size()){
					curend = pos.at(j+1)-k;
				}
				//if (fitMethod == 0)
				//	curSCA=cal_dist(i,curstart,curend,k);
				//else
					curSCA=1-cal_R(i,-1,curstart,curend,k);
				if (curSCA < bestSCA){
					bestSCA = curSCA;
					bestShift = k;
				}
				k++;
			}
			if (bestShift <=0)
				storedel.clear();
			curdel.clear();
			// Do the shifting
			while (bestShift != 0){
				if (bestShift < 0){
					train.at(i).input.erase(train.at(i).input.begin()+pos.at(j));
					train.at(i).input.insert(train.at(i).input.begin()+pos.at(j+1),train.at(i).input.at(pos.at(j+1)-1));
					bestShift++;
				}
				else{
					if (!storedel.empty()){
						train.at(i).input.insert(train.at(i).input.begin()+pos.at(j),storedel.back());
						storedel.pop_back();
					}
					else
						train.at(i).input.insert(train.at(i).input.begin()+pos.at(j),train.at(i).input.at(pos.at(j)));
					curdel.push_back(train.at(i).input.at(pos.at(j+1)));
					train.at(i).input.erase(train.at(i).input.begin()+pos.at(j+1));
					bestShift--;
				}
			}
			storedel = curdel;
		}
	}
}

void iface::rafftAlign(double low, double up, float scale, int shift,int lookahead)
{
	if (avg.input.size() == 0)
		return;
	writeToLast(14);
	int start = getXIndex(low);
	int end = getXIndex(up);
	if (scale == 0)
		scale = 1;
	if (scale != 1){
		for (unsigned int i =0; i < avg.input.size(); ++i){
			if (avg.input.at(i) < 0)
				avg.input.at(i) = 0;
			avg.input.at(i) = pow((double)(avg.input.at(i)),(double)(1/(double)scale));
		}
	}
	for (unsigned int i = 0; i < train.size(); ++i){
		if (scale != 1){
			for (unsigned int j =0; j < avg.input.size(); ++j){
				if (train.at(i).input.at(j) < 0)
					train.at(i).input.at(j) = 0;
				train.at(i).input.at(j) = pow((double)(train.at(i).input.at(j)),double(1/(double)scale));
			}
		}
		fftRecurAlign(i,start,end, shift,lookahead);
		if (scale != 1){
			for (unsigned int j =0; j < avg.input.size(); ++j){
				train.at(i).input.at(j) = pow((double)(train.at(i).input.at(j)),double(scale));
			}
		}
	}
	if (scale != 1){
		for (unsigned int i =0; i < avg.input.size(); ++i){
			avg.input.at(i) = pow((double)(avg.input.at(i)),double(scale));
		}
	}
}

void iface::fftRecurAlign(int spec, int start, int end, int shift, int lookahead)
{
	int M = end-start;
	if (M < 10)
		return;
	int lag = fftCorr(spec,-1,start,end, shift);
	if (lag == 0 && lookahead == 0)
		return;
	if (lag == 0)
		lookahead--;
	if (lag < M)
		doShift(spec,start,end,lag);
	int mid = findMidMin(spec, start, end);
	fftRecurAlign(spec,start,mid, shift, lookahead);
	fftRecurAlign(spec,mid,end, shift, lookahead);
	return;
}
	

void iface::pafftAlign(int segsize, double low, double up, float scale, int shift)
{
	if (avg.input.size() == 0)
		return;
	int start = getXIndex(low);
	int end = getXIndex(up);
	vector <int> pos;
	vector <double> storedel;
	vector <double> curdel;
	writeToLast(13);
	int bestShift;
	if (scale == 0)
		scale = 1;
	if (scale != 1){
		for (unsigned int i =0; i < avg.input.size(); ++i){
			if (avg.input.at(i) < 0)
				avg.input.at(i) = 0;
			avg.input.at(i) = pow((double)(avg.input.at(i)),1/(double)scale);
		}
	}
	for (unsigned int i = 0; i < train.size(); ++i){
		if (scale != 1){
			for (unsigned int j =0; j < avg.input.size(); ++j){
				if (train.at(i).input.at(j) < 0)
					train.at(i).input.at(j) = 0;
				train.at(i).input.at(j) = pow((double)(train.at(i).input.at(j)),1/(double)scale);
			}
		}
		pos = fragmentate(i,segsize, start, end);
		for (unsigned int j = 0; j < pos.size()-1; ++j){
			int curstart = pos.at(j);
			int curend = pos.at(j+1);
			bestShift=fftCorr(i,-1,curstart,curend,shift);
			curdel.clear();
			// Do the shifting
			while (bestShift != 0){
				if (bestShift < 0){
					train.at(i).input.erase(train.at(i).input.begin()+pos.at(j));
					train.at(i).input.insert(train.at(i).input.begin()+pos.at(j+1),train.at(i).input.at(pos.at(j+1)-1));
					bestShift++;
				}
				else{
					if (!storedel.empty()){
						train.at(i).input.insert(train.at(i).input.begin()+pos.at(j),storedel.back());
						storedel.pop_back();
					}
					else
						train.at(i).input.insert(train.at(i).input.begin()+pos.at(j),train.at(i).input.at(pos.at(j)));
					curdel.push_back(train.at(i).input.at(pos.at(j+1)));
					train.at(i).input.erase(train.at(i).input.begin()+pos.at(j+1));
					bestShift--;
				}
			}
			storedel = curdel;
		}
		if (scale != 1){
			for (unsigned int j =0; j < avg.input.size(); ++j){
				train.at(i).input.at(j) = pow((double)(train.at(i).input.at(j)),(double)scale);
			}
		}
	}
	if (scale != 1){
		for (unsigned int i =0; i < avg.input.size(); ++i){
			avg.input.at(i) = pow((double)(avg.input.at(i)),(double)scale);
		}
	}
}

vector <int> iface::fragmentate(int spec, int segsize, int start, int end)
{
	vector <int> pos;
	pos.push_back(start);
	int curStaPos = start;
	int curEndPos = start;
	data *curSpec = &train.at(spec);
	while (curStaPos < end){
		curEndPos = curStaPos+segsize+segsize/2;
		if (curEndPos >= end){
			curStaPos = end;
		}
		else{
			int minpos = curSpec->getMinYpos(curStaPos+segsize,curEndPos);
			pos.push_back(minpos);
			curStaPos += minpos;
		}
	}
	pos.push_back(end);
	return pos;
}

void iface::genCorr(int win)
{
	if (avg.input.size() == 0)
		return;
	char buf[256]="";
	int end = 0;
	int start = 0;
	int range = (int) ceil((double(win))/2);
	vector <data> corrSet;
	vector <double> avgin;
	avgin.assign(mz_val.size(),0);
	writeToLast(14);
	int tsize = train.size();
	for (unsigned int i = 0; i < tsize; ++i){
		for (unsigned int j = 0; j < mz_val.size(); ++j){
			start = j -range;
			end = j+range;
			if (start < 0)
				start = 0;
			if (end > mz_val.size())
				end = mz_val.size();
			avgin.at(j)+=cal_R(i,-1,start,end,0);
		}
	}

	data avgCorr;
	avgCorr.mz_val = &mz_val;
	avgCorr.pos = 1;
	avgCorr.name = "Corr Avg";
	avgCorr.input = avgin;
	double test[10000];
	for (unsigned int j = 0; j < mz_val.size(); ++j){
		avgCorr.input.at(j) /= tsize;
		test[j]=10;
	}
	train.insert(train.begin(),avgCorr);
}

int iface::fftCorr(int spec1, int spec2, int start, int end, int shift)
{
	if (start >= end)
		return 0;
	vector <double> *input1;
	vector <double> *input2;
	input1 = &(train.at(spec1).input);
	if (spec2 == -1)
		input2 = &(avg.input);
	else
		input2 = &(train.at(spec2).input);
	int pos = 0;
	int M = end-start;
	for (unsigned int i=start; i < end; ++i){
		in1[pos]=input1->at(i);
		in2[pos]=input2->at(i);
		pos++;
	}
	int Esize = M;
	int k = 0;
	while (Esize > pow((double)(2),k)){
		++k;
	}
	Esize = (int)pow((double)(2),k);
	if (Esize > 131072)
		Esize = 131072;
	FFTReal  fft_object(Esize);
	int diff = Esize-M;
	for (unsigned int i=0; i < diff; ++i){
		in1[M+i]=in2[M+i]=0;
	}
	M+=diff;
	int hm=M/2;
	fft_object.do_fft(out1,in1);
	fft_object.do_fft(out2,in2);
	in1[0] = out1[0]*out2[0];
	in1[hm]= out1[hm]*out2[hm];
	for(unsigned int i = 1; i < hm; ++i){
		in1[i] = (out1[i]*out2[i])+(out1[(hm)+i]*out2[(hm)+i]);
		in1[(hm)+i] = (out1[i]*(out2[(hm)+i]))-(out2[i]*(out1[(hm)+i]));
	}
	fft_object.do_ifft(in1,in2);
	fft_object.rescale (in2);  
	int maxpos = 0;
	double max=-1;
	for (unsigned int i = 0; i < shift; ++i){
		if (in2[i] >max){
			max = in2[i];
			maxpos = i;
		}
		if (in2[M-i-1] >max){
			max = in2[M-i-1];
			maxpos = M-i-1;
		}
	}
	if (maxpos < M/2)
		return maxpos;
	return maxpos-M;
}

int iface::findMidMin(int spec, int start, int end)
{
	int mid = (int)floor((double)(end-start)/2);
	double min = 99999;
	int minpos = mid;
	vector <double>* input = &train.at(spec).input;
	for (unsigned int i = 0; i < (end-start)/8;++i){
		if (input->at(mid-i) < min){
			min = input->at(mid-i);
			minpos = mid-i;
		}
		if (input->at(mid+i) < min){
			min = input->at(mid+i);
			minpos = mid+i;
		}
	}
	return minpos+start;
}

void iface::doShift(int spec, int start, int end, int lag)
{
	vector <double>* input = &train.at(spec).input;
	while (lag != 0){
		if (lag < 0){
			input->erase(input->begin()+start);
			input->insert(input->begin()+end,input->at(end-1));
			lag++;
		}
		else{
			input->insert(input->begin()+start,input->at(start));
			input->erase(input->begin()+end);
			lag--;
		}
	}
}

vector <double> iface::InterpolXScale(vector <double> *inScale, vector<double> *input, int orgsize)
{
	vector <double> res;
	//vector <double> tmp[1000];
	int i = 0;
	register int j = 0;
	if (mz_val.front() > inScale->front()){
		double old_front = mz_val.front();
		while (j < inScale->size() && old_front > inScale->at(j)){
		//	mz_val.insert(mz_val.begin()+j,inScale->at(j));
			res.push_back(input->at(j));
			++j;
		}
		i = j;
		mz_val.insert(mz_val.begin(),j,0);
		for (unsigned int k = 0; k < j; ++k)
			mz_val.at(k)=inScale->at(k);
		for (unsigned int m= 0; m < orgsize; ++m){
			//tmp[m].assign(j,0.0000001);
			train.at(m).input.insert(train.at(m).input.begin(),j,0.00000001);
		}
	}
	else{
		while (inScale->front() > mz_val.at(i)){
			res.push_back(0.00000001);
			++i;
		}
		//res.back()=input->front()/2;
	}
	vector <double> tmpPos;
	while (j+1 < inScale->size() && inScale->at(j+1) < mz_val.back()){
		tmpPos.clear();
		while (inScale->at(j+1) > mz_val.at(i)){
			tmpPos.push_back(mz_val.at(i));	
			++i;
		}
		double grad = (input->at(j+1)-input->at(j))/(inScale->at(j+1)-inScale->at(j));
		double intcep = input->at(j)-grad*inScale->at(j);
		for (unsigned int k = 0; k < tmpPos.size(); ++k){
			res.push_back(grad*tmpPos.at(k)+intcep);
		}
		++j;
	}
	if (mz_val.back() < inScale->back()){
		while (j < inScale->size()){
			mz_val.push_back(inScale->at(j));
			for (unsigned int m= 0; m < orgsize; ++m){
				train.at(m).input.push_back(0.00000001);
			}
			res.push_back(input->at(j));
			++j;
		}
	}
	else{
		while (res.size() < mz_val.size()){
			res.push_back(0.00000001);
		}
	}
	while (res.size() < mz_val.size())
		res.push_back(res.back());
	return res;
}

void iface::convertPrevXScale(vector<double> inScale)
{
	for (unsigned int i = 0; i < this->train.size()-1; ++i){
		int k = 0;
		vector <double> tmp;
		for (unsigned int j = 0; j < inScale.size(); ++j){
			double cur = 0;
			while (k <mz_val.size() && mz_val.at(k) < inScale.at(j)+0.5){
				cur+=train.at(i).input.at(k);
				++k;
			}
			tmp.push_back(cur);
		}
		train.at(i).input = tmp;
	}
	mz_val= inScale;
}

vector <double> iface::convertCurXScale(vector <double> *inScale, vector <double> *input)
{
	vector <double> res;
	int k = 0;
	for (unsigned int j = 0; j < mz_val.size(); ++j){
		double cur = 0;
		while (k <inScale->size() && inScale->at(k) < mz_val.at(j)+0.5){
			cur+=input->at(k);
			++k;
		}
		res.push_back(cur);
	}
	return res;
}

vector <double> iface::convertBothXScale(vector <double> *inScale, vector <double> *input)
{
	vector <double> old_mz = mz_val;
	vector <double> new_mz(mz_val.size()+inScale->size());
	merge(old_mz.begin(),old_mz.end(),inScale->begin(),inScale->end(),new_mz.begin());
	for (unsigned int i = 0; i < new_mz.size()-1; ++i){
		if (new_mz.at(i) == new_mz.at(i+1))
			new_mz.erase(new_mz.begin()+i);
	}
	for (unsigned int i = 0; i < this->train.size()-1; ++i){
		int k = 0;
		vector <double> tmp;
		for (unsigned int j = 0; j < new_mz.size(); ++j){
			double cur = 0;
			while (k <mz_val.size() && mz_val.at(k) <= new_mz.at(j)){
				cur+=train.at(i).input.at(k);
				++k;
			}
			tmp.push_back(cur);
		}
		train.at(i).input = tmp;
	}
	mz_val= new_mz;
	vector <double> res;
	int k = 0;
	for (unsigned int j = 0; j < mz_val.size(); ++j){
		double cur = 0;
		while (k <inScale->size() && inScale->at(k) <= mz_val.at(j)){
			cur+=input->at(k);
			++k;
		}
		res.push_back(cur);
	}
	return res;
}

void iface::cropSpectra(int startPos, int endPos)
{
	vector <double> tempmz;
	vector <double> inmz;
	double maxy;
	for (unsigned int i = 0; i < train.size(); ++i){
		inmz.clear();
		maxy = train.at(i).getMaxY(startPos,endPos);
		for (unsigned int j = startPos; j < endPos; ++j){
			if (i == 0)
				tempmz.push_back(mz_val.at(j));
			inmz.push_back(train.at(i).input.at(j));
		}
		train.at(i).input = inmz;
	}
	mz_val = tempmz;
}

char* iface::itoa(int val, int base)
{
	if (val == 0)
		return "0";
	static char buf[32]={0};
	int i = 30;
	for (; val &&i; --i, val/=base)
		buf[i]="0123456789abcdef"[val%base];
	return &buf[i+1];
}

