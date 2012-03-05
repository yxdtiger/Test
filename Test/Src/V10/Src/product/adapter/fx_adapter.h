#ifndef FX_ADAPTER_H_
#define FX_ADAPTER_H_

#include "product_adapter.h"


namespace wmdf
{
  class ProductSource;

#pragma pack(push)  /* push current alignment to stack */
#pragma pack(1)     /* set alignment to 1 byte boundary */

  typedef struct FXDataStruct
  {
    char	chPrefix;		        //0x55
    short	nsLength;	        	//³¤¶È
    double	dTime;		      	//Ê±¼ä
    int		nDate;			        //ÈÕÆÚ
    char	szMarket[3];
    char	szWindCode[11];
    char	szWindName[15];
    bool	bSerialized;
    char	szSkyCode[11];
    char	szSkyName[15];
    float	fLastOpenInterest;	//×òÈÕ³Ö²ÖÁ¿
    float	fLastSettle;		    //½áËã¼Û
    float	fLastClose;	      	//×òÊÕ
    float	fOpen;			        //½ñ¿ª
    float	fHigh;		        	//×î¸ß
    float	fLow;			          //×îµÍ
    float	fNewPrice;	      	//×îÐÂ				//3
    float	fVolume;	        	//³É½»Á¿			//8
    float	fAmount;		        //³É½»¶î			//59
    float	fIgnore1[6];
    float	f1;
    float	fIgnore2[9];
    float	f2;
    float	fIgnore3[11];
    char	chTail;			        //0x16
  }STRUCT_FX;

#pragma pack(pop)


  class FxAdapter : public ProductAdapter
  {
  public:
    FxAdapter(ProductSource* product_source);
    virtual ~FxAdapter();
  protected:
    virtual uint32_t ParseMessage(const uint32_t length);
    void PublishMessages();
    void ClearRecords();
  private:
    std::vector<STRUCT_FX*> old_records_;
  };
}



#endif
