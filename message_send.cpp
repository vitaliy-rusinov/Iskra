//ERROR CODES

#define ERROR_SUCCESS 0
#define INVALID_TRK_NO 1
#define INVALID_COMMAND 2
#define INVALID_PRICE 3
#define INVALID_VOLUME 4

//typedef unsigned long int uint32_t; // platform-dependent type definitions
//typedef unsigned short int uint16_t; // platform-dependent type definitions


// iskra_message_write
//Procedure that forms a zero-terminated 24-byte message buffer in accordance with Iskra protocol.

int iskra_message_write(char TRK_NO, char Command, uint32_t Price, uint32_t Volume, uint16_t Status, char buf[24] ) {
    
     // Out of bounds checks
    
    if(TRK_NO>128) return INVALID_TRK_NO;
    if(Command>0xF) return INVALID_COMMAND;
    if(Price>999999) return INVALID_PRICE;
    if(Volume>999999) return INVALID_VOLUME;
    
    buf[0]=1; // SOH 1h
    
    buf[1]='0'+TRK_NO/16; //TRK_NO HIGH BYTE
    
    unsigned char tmp2=TRK_NO%16; // TRK_NO LOW BYTE
    buf[2]=tmp2>0x9?'A'+tmp2-0xA:'0'+tmp2;	// TRK_NO LOW BYTE
    
    buf[3]=Command>0x9?'A'+Command-0xA:'0'+Command; //Command
    
    buf[4]=2; // STX 2h

    //PRICE (6 digits) BYTES 5,6,7,8,9,10 
    
        uint32_t tmp=Price;
        
        for(char c1=10;c1>5;c1--){
        buf[c1]=tmp%10+'0';
        tmp/=10;
        }
        
        buf[5]=tmp +'0';
    
    //ENDPRICE

    //VOLUME (6 digits) BYTES 11,12,13,14,15,16
    
        tmp=Volume;
        for(char c1=16;c1>12;c1--){
        
        buf[c1]=tmp%10+'0';
        tmp/=10;
        }
        
        buf[11]=tmp+'0';

    // END VOLUME

    //STATUS (bytes 17,18,19,20)
    
        uint16_t tmp3=Status;
        
        for(char c1=20;c1>17;c1--){
            tmp2=(tmp3%16);
            buf[c1]=tmp2>0x9?'A'+tmp2-0xA:tmp2+'0'; 
            tmp3/=16;
        }
        
        tmp2=tmp3%16;
        buf[17]=tmp2>0x9?'A'+tmp2-0xA:tmp2+'0';

    //END_STATUS

    buf[21]=0x3; //ETX 3h

    //CRC

        unsigned char crc=buf[1];
        for(tmp2=2; tmp2<22; tmp2++)crc^=buf[tmp];
        buf[22]=crc;

    //END_CRC

    buf[23]='\0'; //END OF LINE
    return ERROR_SUCCESS;
}