#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "openlsm.h"
using namespace std;


int main(int argc, char *argv[])
{
	ifstream infile("/home/xx/Desktop/lsm/lsmfile_list.txt");
	
    vector<string> lsmfiles;
    
    string line;
	while (getline(infile, line)){
		 lsmfiles.push_back(line);
	}
   
    auto pointed = lsmfiles.end()-1;
    bool flag = true;
  for(vector<string>::iterator it=lsmfiles.begin();it <  lsmfiles.end() && flag;++it){
//       it = pointed;
//       flag = false;
       lsmHeader *header = lsm_header(*it);
       lsmInfo *info = lsm_info(*it,*header);
           cout<<*it<<endl;
           cout<<"header:"<<header->byteOrder<<","<<header->lsmId<<","<<header->offset<<endl;
           cout<<"magic number:"<<hex<<std::showbase<<info->u32MagicNumber<<endl;
           cout<<"scanType:"<<dec<<info->u16ScanType<<endl;
           cout<<"s32Dimensions: "
              // <<info->s32DataType<<", "
               <<info->s32DimensionX<<", "
               <<info->s32DimensionY<<", "
               <<info->s32DimensionZ<<", "
               <<info->s32DimensionTime<<", "
               <<info->s32DimensionChannels<<endl;

           vector<double> timestamps = get_timestamp(*it,*info);
          // for_each(timestamps.begin(),timestamps.end(),[](double it){cout<<it<<endl;});
          // get_channelColor(*it,*info);
          // get_channelWaveLength(*it,*info);
           //get_eventlist(*it,*info);
           cout<<"time zero:"<<timestamps[0]<<endl;
           //get_ScanInfor(*it,*info);
           //get_bleeachROi(*it,*info);
            cout<<"------------------"<<endl;
           delete info;
           delete header;
    }//end of file name for-loop

//    string filename;
//    lsmStack *image = lsm_image(filename,*header);
//    if(image != NULL)
//        cout<<image->pixels.size()<<endl;
//    else
//        cout<<"nothing!"<<endl;
//    delete image;
    return 0;
}
