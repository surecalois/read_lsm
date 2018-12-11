#ifndef OPENLSM_CPP
#define OPENLSM_CPP

#include "openlsm.h"
#include "lsm_roi.h"
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>

lsmHeader * lsm_header(const std::string &filename){
    lsmHeader *header = new lsmHeader;
    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
    lsmfile.seekg(0,std::ios::beg);
    lsmfile.read((char*)header,sizeof(lsmHeader));
    lsmfile.close();
    if(header->lsmId != 42) {
		std::cout<<"something is wrong in the header!"<<std::endl;
		return nullptr;
	}
    return header;
}

lsmInfo* lsm_info(const std::string &filename,const lsmHeader &header){
    lsmInfo *info = new lsmInfo;
    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
    lsmEntry entry;
    uint32_t cz_private_tag=0;
    unsigned short tagNum;

    lsmfile.seekg(header.offset,std::ios::beg);
    lsmfile.read((char*)&tagNum,sizeof(unsigned short));

        for(int ii=0;ii<tagNum;++ii){ //cz_private_tag only exist in the first directory
            lsmfile.read((char*)&entry,sizeof(lsmEntry));
            if(entry.tag==34412)
                cz_private_tag = entry.value;
        }
    lsmfile.seekg(cz_private_tag,std::ios::beg);
    lsmfile.read((char*)info,sizeof(lsmInfo));
    lsmfile.close();
    return info;
}

std::vector<std::vector<imageEntry>> lsmImageEntry(const std::string &filename,const lsmHeader &header)
{

    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);

    lsmEntry entry;
    std::vector<lsmEntry> bit_deepth;
    std::vector<lsmEntry> byte_size;
    std::vector<lsmEntry> image_offset;
    std::unordered_map<unsigned short,lsmEntry> dir_entry;

    uint32_t offset = header.offset;
    unsigned short tagNum = 0;
    unsigned short chNum = 0;
    std::vector<uint32_t> image_w;// = 0;
    std::vector<uint32_t> image_h;// = 0;

    std::pair<std::unordered_map<unsigned short,lsmEntry>::iterator,bool> ret;

    while(offset != 0 && lsmfile){

        lsmfile.seekg(offset,std::ios::beg);
        lsmfile.read((char*)&tagNum,sizeof(unsigned short));

            for(int ii=0;ii<tagNum;++ii){
                lsmfile.read((char*)&entry,sizeof(lsmEntry)); //lsmEntry is 12 bytes
                ret=dir_entry.insert(std::pair<unsigned short,lsmEntry>(entry.tag,entry));
                if(!ret.second) std::cout<<"reapeated tag "<<entry.tag<<" already exist!"<<std::endl;
            }

        lsmfile.read((char*)&offset,sizeof(uint32_t));
        if(dir_entry[254].value == 0){
            image_w.push_back(dir_entry[256].value);
            image_h.push_back(dir_entry[257].value);
            bit_deepth.push_back(dir_entry[258]);
            image_offset.push_back(dir_entry[273]);
            byte_size.push_back(dir_entry[279]);
            chNum = dir_entry[277].value;
        }
        dir_entry.clear();
    }

    uint32_t dir_num = bit_deepth.size();//or the other two containers
    auto p_bit = bit_deepth.begin();
    auto p_byte = byte_size.begin();
    auto p_ima = image_offset.begin();
    std::vector<std::vector<imageEntry>> image_entry(dir_num,std::vector<imageEntry>(chNum));

    unsigned short *bit = new unsigned short [p_bit->length];
    uint32_t *byte = new uint32_t [p_byte->length];
    uint32_t *ima_addrs = new uint32_t [p_ima->length];

    //if it is a single channel the entry value is value, otherwise the entry value is offset
    if(chNum == 1){
        for(uint32_t ii = 0; ii < dir_num; ++ii,++p_bit,++p_byte,++p_ima){

            bit[0]=p_bit->value;
            byte[0]=p_byte->value;
            ima_addrs[0]=p_ima->value;

            image_entry[ii][0].bit_deepth = bit[0];
            image_entry[ii][0].byte_size = byte[0];
            image_entry[ii][0].offset = ima_addrs[0];

            for(uint32_t jj=0;jj< chNum;jj++){
                image_entry[ii][jj].bit_deepth = bit[jj];
                image_entry[ii][jj].byte_size = byte[jj];
                image_entry[ii][jj].offset = ima_addrs[jj];
                image_entry[ii][jj].width = image_w[ii];
                image_entry[ii][jj].height = image_h[ii];
            }
        }

    }else{

        for(uint32_t ii = 0; ii < dir_num; ++ii,++p_bit,++p_byte,++p_ima){

            lsmfile.seekg(p_bit->value,std::ios::beg); //in the directory entry, value is offset
            lsmfile.read((char*)bit,sizeof(unsigned short)*p_bit->length);

            lsmfile.seekg(p_byte->value,std::ios::beg);
            lsmfile.read((char*)byte,sizeof(uint32_t)*p_byte->length);

            lsmfile.seekg(p_ima->value,std::ios::beg);
            lsmfile.read((char*)ima_addrs,sizeof(uint32_t)*p_ima->length);

            for(uint32_t jj=0;jj< chNum;jj++){
                image_entry[ii][jj].bit_deepth = bit[jj];
                image_entry[ii][jj].byte_size = byte[jj];
                image_entry[ii][jj].offset = ima_addrs[jj];
                image_entry[ii][jj].width = image_w[ii];
                image_entry[ii][jj].height = image_h[ii];
            }
        }

    }
    std::cout<<"diretory number: "<<dir_num<<"\tchannel number: "<<chNum<<std::endl;
    std::cout<<"image width: "<<image_w[0]<<"\timage height: "<<image_h[0]<<std::endl;

    lsmfile.close();
    delete [] bit;
    delete [] byte;
    delete [] ima_addrs;

    return image_entry;

}

lsmStack *lsm_image(const std::string &filename,const lsmHeader &header)
{
    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
    std::vector<std::vector<imageEntry>> image_entry = lsmImageEntry(filename,header);

    unsigned short tbit_deepth = image_entry[0][0].bit_deepth;
    uint32_t tbyte_size = image_entry[0][0].byte_size;
    uint32_t dir_num = image_entry.size();
    uint32_t chn_num = image_entry[0].size();
    uint32_t image_w = image_entry[0][0].width;
    uint32_t image_h = image_entry[0][0].height;

    uint32_t dir_index = 0;
    uint32_t chn_index = 0;
    bool normal = true;

    std::vector<char> pixels(dir_num*chn_num*tbyte_size);
    char *img = pixels.data();

    for(dir_index = 0;dir_index < dir_num && normal;++dir_index){
        for(chn_index = 0;chn_index < chn_num && normal;++chn_index ){

            if(tbit_deepth != image_entry[dir_index][chn_index].bit_deepth
               || tbyte_size != image_entry[dir_index][chn_index].byte_size)
            {
                normal = false;

                std::cout<<"something wrong in "<< dir_index <<","<< chn_index <<std::endl;
                std::cout<< tbit_deepth <<","<< tbyte_size <<std::endl;
            }

            lsmfile.seekg(image_entry[dir_index][chn_index].offset,std::ios::beg);
            lsmfile.read((char*)(img+(dir_index*chn_num+chn_index)*tbyte_size),tbyte_size);
        }
    }
    std::cout<<"finish reading pixles! "<<pixels.size()<<" bytes"<<std::endl;

    lsmStack *out = normal? new lsmStack(tbit_deepth,dir_num,chn_num,image_w,image_h,tbyte_size,pixels):NULL;

    lsmfile.close();
    return out;
}

uint32_t dimStack::get_dirIndex(uint32_t x,uint32_t y,uint32_t z,uint32_t t,uint32_t c){
        uint32_t dir_index=0;
        switch(info->u16ScanType){
            case 0://normal x-y-z scan
            case 6://x-y-z-t scan
                dir_index = z+t*z;break;//HERE YOU ARE WRONG;
            case 3:
            case 4:
                dir_index = t; break;
            case 1://z-scan
            case 2:
                dir_index = 0;break;
            default:
                dir_index = 0;
        }
        return dir_index;
}

char * dimStack::get_image(uint32_t x,uint32_t y,uint32_t z,uint32_t t,uint32_t c){
        return image->get_image(get_dirIndex(x,y,z,t),c);
}

std::vector<double> get_timestamp(const std::string &filename,const lsmInfo &info)
{
    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
    lsmfile.seekg(info.u32OffsetTimeStamps,std::ios::beg);

    int32_t s32Size = 0;
    int32_t s32NumberTimeStamps = 0;
    lsmfile.read((char*)(&s32Size),sizeof(int32_t));
    lsmfile.read((char*)(&s32NumberTimeStamps),sizeof(int32_t));
   // std::cout<<s32Size<<","<<s32NumberTimeStamps<<std::endl;

    std::vector<double> f64TimeStamps(s32NumberTimeStamps);
    lsmfile.read((char*)(&f64TimeStamps[0]),s32NumberTimeStamps*sizeof(double));
    lsmfile.close();
//    for(int32_t it = s32NumberTimeStamps - 1;it >= 0;--it){
//        f64TimeStamps[it] -= f64TimeStamps[0];
//    }
    return f64TimeStamps;
}

void get_eventlist(const std::string &filename,const lsmInfo &info)
{
    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
    lsmfile.seekg(info.u32OffsetEventList,std::ios::beg);

    int32_t s32Size = 0;
    int32_t s32NumberEvents = 0;
    lsmfile.read((char*)(&s32Size),sizeof(int32_t));
    lsmfile.read((char*)(&s32NumberEvents),sizeof(int32_t));
    if(!s32NumberEvents)
    {
        std::cout<<"no events"<<std::endl;
        return;
    }else{
        std::cout<<s32NumberEvents<<" event(s)"<<std::endl;
    }
    std::vector<lsm_eventListEntry> EventList(s32NumberEvents);
    for(int32_t it = 0;it < s32NumberEvents; it++){
        uint32_t u32SizeEventListEntry =0;
        double f64Time =0;
        uint32_t u32EventType = 0;

        lsmfile.read((char*)(&u32SizeEventListEntry),sizeof(uint32_t));
        uint32_t rest = u32SizeEventListEntry - sizeof(uint32_t)*2 - sizeof(double);
        char *u8Description = new char[rest];

        lsmfile.read((char*)(&f64Time),sizeof(double));
        lsmfile.read((char*)(&u32EventType),sizeof(uint32_t));
       // std::cout<<u32SizeEventListEntry<<"->"<<rest<<", time:"<<f64Time<<", type:"<<u32EventType<<std::endl;
        int32_t d_size;
        lsmfile.read((char*)&d_size,sizeof(int32_t));
        lsmfile.read(u8Description,d_size);
       // std::cout<<u8Description<<std::endl;
        EventList[it].u32SizeEventListEntry = u32SizeEventListEntry;
        EventList[it].f64Time = f64Time;
        EventList[it].u32EventType = u32EventType;
        EventList[it].u8Description = std::string(u8Description);
        delete u8Description;
    }

    for(auto it = EventList.begin();it < EventList.end();it++){
        std::cout<<it->u8Description<<": "<<it->f64Time<<", type:"<<it->u32EventType<<std::endl;
    }

    lsmfile.close();
    //return EventList;
}


void get_channelColor(const std::string &filename,const lsmInfo &info)
{
    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
    lsmfile.seekg(info.u32OffsetChannelColors,std::ios::beg);
    int32_t colorinfo[10];//s32BlockSize,s32NumberColors,s32NumberNames,s32ColorsOffset,s32NamesOffset,others
    lsmfile.read((char*)&colorinfo,10*sizeof(int32_t));
    for(int ii = 0;ii < 10; ii++){
        std::cout<<colorinfo[ii]<<",";
    }
    std::cout<<std::endl;
    unsigned char rgba[4];//blue,green,red,alpha
    lsmfile.seekg(info.u32OffsetChannelColors+colorinfo[3],std::ios::beg);
    for(int32_t ii = 0;ii< colorinfo[1];ii++){
        lsmfile.read((char*)rgba,4);
        std::cout<<(int)rgba[0]<<","<<(int)rgba[1]<<","<<(int)rgba[2]<<","<<(int)rgba[3]<<std::endl;
    }

    lsmfile.seekg(info.u32OffsetChannelColors+colorinfo[4],std::ios::beg);
    for(int32_t ii = 0;ii< colorinfo[2];ii++){
        int32_t name_size;//the document is wrong, there is a size at the begining of name string
        lsmfile.read((char*)&name_size,sizeof(int32_t));
        char *name = new char[name_size];
        lsmfile.read(name,name_size);
        std::cout<<name<<std::endl;
        delete name;
    }


    lsmfile.close();
}


void get_channelWaveLength(const std::string &filename,const lsmInfo &info)
{
    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
    lsmfile.seekg(info.u32OffsetChannelWavelength,std::ios::beg);
    int32_t s32Channels;
    lsmfile.read((char*)&s32Channels,sizeof(int32_t));
    std::cout<<"number of channels:"<<s32Channels<<std::endl;
    double *f64StartWavelength = new double [2*s32Channels];
    lsmfile.read((char*)f64StartWavelength,sizeof(double)*2*s32Channels);
    for(int32_t ii = 0;ii<s32Channels;ii++){
        std::cout<<f64StartWavelength[ii*2]*1e9<<","<<f64StartWavelength[ii*2+1]*1e9<<std::endl;
    }
    delete f64StartWavelength;

    lsmfile.close();
}

void get_ScanInfor(const std::string &filename,const lsmInfo &info)
{
    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
    lsmfile.seekg(info.u32OffsetScanInformation,std::ios::beg);
    uint32_t u32Entry;
    uint32_t u32Type;//0:start or end,2:byte,4:int32_t,5:double
    uint32_t u32Size;
    uint32_t n_start_end=0;
    uint32_t n_ends=0;
    char *var;
    do{
        lsmfile.read((char*)&u32Entry,sizeof(uint32_t));
        lsmfile.read((char*)&u32Type,sizeof(uint32_t));
        lsmfile.read((char*)&u32Size,sizeof(uint32_t));
        if(u32Size) {
            var = new char[u32Size];
            lsmfile.read(var,u32Size);
        }
        std::cout<<std::hex<<u32Entry<<std::dec<<","<<u32Type<<","<<u32Size;
        switch(u32Type){
            case 0:
                n_start_end++;
                std::cout<<std::endl;
                break;
            case 2:
                std::cout<<",\'"<<var<<"\'"<<std::endl;
                break;
            case 4:
                std::cout<<","<<*(int32_t*)var<<std::endl;
                break;
            case 5:
                std::cout<<","<<*(double*)var<<std::endl;
                break;
        }
        if(u32Entry == 0x0FFFFFFFF) n_ends++;
        if(u32Size) delete [] var;

    }while(n_start_end != 2*n_ends);
    lsmfile.close();
}


void get_bleeachROi(const std::string &filename,const lsmInfo &info)
{
    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
    lsmfile.seekg(info.u32OffsetVectorOverlay,std::ios::beg);

    lsm_roihead roihead;
    //lsmfile.read((char*)&roihead,196);
    roihead.read_struct(lsmfile);
    std::cout<<roihead.s32NumberDrawingElements<<std::endl;
    for(int i = 0;i < 7;i++) std::cout<<roihead.s32reserved[i]<<","; std::cout<<std::endl;
    if(roihead.s32NumberDrawingElements == 0 ){
        lsmfile.close();
        return;
    }
    lsm_drawElemHead drawElemHead;
    drawElemHead.read_struct(lsmfile);

    std::cout<<drawElemHead.s32type<<","<<drawElemHead.f64addittextstartpointx<<","
             <<drawElemHead.f64addittextstartpointy<<std::endl;
    if(drawElemHead.s32type != 13 && drawElemHead.s32type != 1){
        uint32_t u32numberKnots;
        lsmfile.read((char*)&u32numberKnots,sizeof(uint32_t));//sizeof(rectangle));
        std::cout<<u32numberKnots<<std::endl;

        double *f64knotsxy = new double [u32numberKnots*2];
        lsmfile.read((char*)f64knotsxy,u32numberKnots*2*sizeof(double));
        for(uint32_t ii = 0;ii < u32numberKnots; ii++){
            std::cout<<f64knotsxy[2*ii]<<","<<f64knotsxy[2*ii+1]<<" ";
        }
        std::cout<<std::endl;
        delete [] f64knotsxy;
    }else{
        double f64PointOriginXY[2];
        lsmfile.read((char*)f64PointOriginXY,2*sizeof(double));
        int32_t text_size; //I THINK THERE SHOULD BE SIZE BEFORE ALL STRINGS;
        lsmfile.read((char*)&text_size,sizeof(int32_t));
        uint16_t *u16Text = new uint16_t [text_size];
        lsmfile.read((char*)u16Text,text_size*sizeof(uint16_t));
        delete [] u16Text;
    }

    lsmfile.close();
}

//void get_channelNameColor(const std::string &filename,const lsmInfo &info)
//{
//    std::ifstream lsmfile(filename,std::ios::binary|std::ios::in);
//    lsmfile.seekg(info.u32OffsetEventList,std::ios::beg);

//    lsmfile.close();
//}


#endif //OPENLSM_CPP
