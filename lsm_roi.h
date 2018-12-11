#ifndef LSM_ROI_H
#define LSM_ROI_H
#include <fstream>
struct lsm_roihead{
    int32_t s32NumberDrawingElements;
    int32_t s32Size;
    int32_t s32linewidth;
    int32_t s32measure;
    int32_t s32reserved1;
    int32_t s32reserved2;
    int32_t s32color;
    int32_t s32valid;
    int32_t s32knotwidth;
    int32_t s32catcharea;
    int32_t s32fontheight;
    int32_t s32fontwidth;
    int32_t s32fontescapement;
    int32_t s32fontorientation;
    int32_t s32fontweight;
    int32_t s32fontitalic;
    int32_t s32fontunderline;
    int32_t s32fontstrikeout;
    int32_t s32fontcharset;
    int32_t s32fontoutprecision;
    int32_t s32fontclipprecision;
    int32_t s32fontquality;
    int32_t s32fontpitchandfamily;
    uint16_t u16fontfacename[32];
    uint8_t u8measureflags[10];
    int32_t s32reserved[7];
    int8_t s8reserved[2];
    void read_struct(std::ifstream &ifobj);
};

void lsm_roihead::read_struct(std::ifstream &ifobj){
    ifobj.read( (char*) &s32NumberDrawingElements , sizeof(int32_t) );
    ifobj.read( (char*) &s32Size , sizeof(int32_t) );
    ifobj.read( (char*) &s32linewidth , sizeof(int32_t) );
    ifobj.read( (char*) &s32measure , sizeof(int32_t) );
    ifobj.read( (char*) &s32reserved1 , sizeof(int32_t) );
    ifobj.read( (char*) &s32reserved2 , sizeof(int32_t) );
    ifobj.read( (char*) &s32color , sizeof(int32_t) );
    ifobj.read( (char*) &s32valid , sizeof(int32_t) );
    ifobj.read( (char*) &s32knotwidth , sizeof(int32_t) );
    ifobj.read( (char*) &s32catcharea , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontheight , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontwidth , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontescapement , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontorientation , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontweight , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontitalic , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontunderline , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontstrikeout , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontcharset , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontoutprecision , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontclipprecision , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontquality , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontpitchandfamily , sizeof(int32_t) );
    ifobj.read( (char*) u16fontfacename , 32*sizeof(uint16_t));
    ifobj.read( (char*) u8measureflags , 10*sizeof(unsigned char));
    ifobj.read( (char*) s32reserved , 7*sizeof(int32_t));
    ifobj.read( (char*) s8reserved , 2*sizeof(char));
}

struct lsm_drawElemHead{
    int32_t s32type;
    int32_t s32size;
    int32_t s32linewidth;
    int32_t s32measure;
    double f64addittextstartpointx;
    double f64addittextstartpointy;
    int32_t s32color;
    int32_t s32valid;
    int32_t s32knotwidth;
    int32_t s32catcharea;
    int32_t s32fontheight;
    int32_t s32fontwidth;
    int32_t s32fontescapement;
    int32_t s32fontorientation;
    int32_t s32fontweight;
    int32_t s32fontitalic;
    int32_t s32fontunderline;
    int32_t s32fontstrikeout;
    int32_t s32fontcharset;
    int32_t s32fontoutprecision;
    int32_t s32fontclipprecision;
    int32_t s32fontquality;
    int32_t s32fontpitchandfamily;
    uint16_t u16fontfacename[32];
    int32_t s32disabled;//uint16_t u16disabled; //not
    int32_t s32notmoveable;
    int32_t s32reserved[8];
    void read_struct(std::ifstream &ifobj);
};//size 204


void lsm_drawElemHead::read_struct(std::ifstream &ifobj){
    ifobj.read( (char*) &s32type , sizeof(int32_t) );
    ifobj.read( (char*) &s32size , sizeof(int32_t) );
    ifobj.read( (char*) &s32linewidth , sizeof(int32_t) );
    ifobj.read( (char*) &s32measure , sizeof(int32_t) );
    ifobj.read( (char*) &f64addittextstartpointx , sizeof(double) );
    ifobj.read( (char*) &f64addittextstartpointy , sizeof(double) );
    ifobj.read( (char*) &s32color , sizeof(int32_t) );
    ifobj.read( (char*) &s32valid , sizeof(int32_t) );
    ifobj.read( (char*) &s32knotwidth , sizeof(int32_t) );
    ifobj.read( (char*) &s32catcharea , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontheight , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontwidth , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontescapement , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontorientation , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontweight , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontitalic , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontunderline , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontstrikeout , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontcharset , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontoutprecision , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontclipprecision , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontquality , sizeof(int32_t) );
    ifobj.read( (char*) &s32fontpitchandfamily , sizeof(int32_t) );
    ifobj.read( (char*) u16fontfacename , 32*sizeof(uint16_t));
    ifobj.read( (char*) &s32disabled , sizeof(int32_t) );
    ifobj.read( (char*) &s32notmoveable , sizeof(int32_t) );
    ifobj.read( (char*) s32reserved , 8*sizeof(int32_t));
}




struct rectangle{
uint32_t u32numberKnots;
double f64knot1x;
double f64knot1y;
double f64knot2x;
double f64knot2y;
};

//struct ellipse{
//uint32_t u32numberknots;
//double f64knot1x;
//double f64knot1y;
//double f64knot2x;
//double f64knot2y;
//double f64knot3x;
//double f64knot3y;
//};
//struct circle{
//uint32_t u32numberknots;
//double f64knot1x;
//double f64knot1y;
//double f64knot2x;
//double f64knot2y;
//};

#endif // LSM_ROI_H
