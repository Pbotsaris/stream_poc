


int main() {

uint8_t inbuf[SOME_BUFFER_SIZE];
int size = fread(inbuf, 1, SOME_BUFFER_SIZE, file);

while(size > 0){

    int decode_length = av_ffmpeg_decoder(data);

    data += decoded_length; 
    size -= decoded_length; 

   if(size < SOME_TRESHOLD_SIZE){
     // let's do some acrobatics!
     memmove(inbuf, data, data_size);
     data = inbuf;
     len = fread(data + data_size, 1, SOME_BUFF_LENGTH - data_size, f);

     if(len > 0){ 
       data_size += len;
     }
   }

}
