#ifndef LSMSTRUCT_H
#define LSMSTRUCT_H
#include <string>
#include <vector>

//header = {lsmHeader, byteOrder:0US, lsmId:0US, offset:0UL}
struct lsmHeader {
           uint16_t byteOrder=0;
          uint16_t lsmId=0;
           uint32_t offset=0;
        };

//entry = {entry, tag:0US, type:0US, length:0UL, value:0UL}
struct lsmEntry {
          uint16_t tag=0;
          uint16_t type=0;
           uint32_t length=0;
           uint32_t value=0;
        };

struct lsmInfo {
           uint32_t u32MagicNumber= 0;
           int32_t s32StructureSize= 0;
           int32_t s32DimensionX=0;
           int32_t s32DimensionY=0;
           int32_t s32DimensionZ=0;
           int32_t s32DimensionChannels=0;
           int32_t s32DimensionTime=0;
           int32_t s32DataType=0;
           int32_t s32ThumbnailX=0;
           int32_t s32ThumbnailY=0;
           double f64VoxelSizeX= 0;
           double f64VoxelSizeY= 0;
           double f64VoxelSizeZ= 0;
           double f64OriginX= 0;
           double f64OriginY= 0;
           double f64OriginZ= 0;
          uint16_t u16ScanType= 0;
          uint16_t u16SpectralScan= 0;
           uint32_t u32DataType= 0;
           uint32_t u32OffsetVectorOverlay= 0;
           uint32_t u32OffsetInputLut= 0;
           uint32_t u32OffsetOutputLut= 0;
           uint32_t u32OffsetChannelColors= 0;
           double f64TimeInterval= 0;
           uint32_t u32OffsetChannelDataTypes= 0;
           uint32_t u32OffsetScanInformation= 0;
           uint32_t u32OffsetKsData= 0;
           uint32_t u32OffsetTimeStamps= 0;
           uint32_t u32OffsetEventList= 0;
           uint32_t u32OffsetRoi= 0;
           uint32_t u32OffsetBleachRoi= 0;
           uint32_t u32OffsetNextRecording= 0;
           double f64DisplayAspectX= 0;
           double f64DisplayAspectY= 0;
           double f64DisplayAspectZ= 0;
           double f64DisplayAspectTime= 0;
           uint32_t u32OffsetMeanOfRoisOverlay= 0;
           uint32_t u32OffsetTopoIsolineOverlay= 0;
           uint32_t u32OffsetTopoProfileOverlay= 0;
           uint32_t u32OffsetLinescanOverlay= 0;
           uint32_t u32ToolbarFlags= 0;
           uint32_t u32OffsetChannelWavelength= 0;
           uint32_t u32Reserved[69];
        };

struct imageEntry{
   uint16_t bit_deepth;
    uint32_t byte_size;
    uint32_t offset;
    uint32_t width;
    uint32_t height;
};


class lsmStack{
public:
   uint16_t bit_deepth;
    uint32_t dir_num;
    uint32_t chn_num;
    uint32_t width;
    uint32_t height;
    uint32_t byte_size;
    std::vector<char> pixels;

    lsmStack(   uint16_t bit_deepth_,
                   uint32_t dir_num_,
                   uint32_t chn_num_,
                   uint32_t width_,
                   uint32_t height_,
                   uint32_t byte_size_,
                   std::vector<char> &pixels_){
        bit_deepth=bit_deepth_;
        dir_num = dir_num_;
        chn_num = chn_num_;
        width = width_;
        height = height_;
        byte_size = byte_size_;
        pixels.swap(pixels_);
    }
    char * get_image(uint32_t dir_index,uint32_t chn_index){
            if(dir_index >= dir_num || chn_index >= chn_num)
                return NULL;
            return pixels.data()+(dir_index*chn_num+chn_index)*byte_size;
    }
};

class dimStack{
public:
    dimStack(){image = NULL;info = NULL;}
    dimStack(lsmStack *image_,lsmInfo *info_){
        image = image_;
        info = info_;
    }
    ~dimStack(){
        if(image != NULL) delete image;
        if(info !=NULL) delete info;
    }
    uint32_t get_width(){return image->width;}
    uint32_t get_height(){return image->height;}
    char * get_image(uint32_t x=0,uint32_t y=0,uint32_t z=0,uint32_t t=0,uint32_t c=0);
    uint32_t get_dirIndex(uint32_t x=0, uint32_t y=0, uint32_t z=0, uint32_t t=0, uint32_t c=0);
private:
    lsmStack *image;
    lsmInfo *info;

};


struct lsm_eventListEntry{
    int32_t u32SizeEventListEntry;
    double f64Time;
    int32_t u32EventType;
    std::string u8Description;
};

lsmHeader * lsm_header(const std::string &filename);
lsmInfo* lsm_info(const std::string &filename,const lsmHeader &header);
lsmStack *lsm_image(const std::string &filename, const lsmHeader &header);
std::vector<std::vector<imageEntry>> lsmImageEntry(const std::string &filename,const lsmHeader &header);
std::vector<double> get_timestamp(const std::string &filename,const lsmInfo &info);
void get_eventlist(const std::string &filename,const lsmInfo &info);
void get_channelColor(const std::string &filename,const lsmInfo &info);
void get_channelWaveLength(const std::string &filename,const lsmInfo &info);
void get_ScanInfor(const std::string &filename,const lsmInfo &info);
void get_bleeachROi(const std::string &filename,const lsmInfo &info);
#endif // LSMSTRUCT_H
