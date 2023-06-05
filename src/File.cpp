#include "Include.h"
/*
从文件中读文件，详情请见官方示例
*/
String readFile(fs::FS &fs, const char * path){
    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return "";
    }
    String res = "";
    while(file.available()){
        char c = file.read();
        res += c;
    }
    file.close();
    return res;
}
/*
向文件中写文件，详情请见官方示例
*/
void writeFile(fs::FS &fs, const char * path, const char * message){
    //Serial.printf(message);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        ;
    } else {
        Serial.println("- write failed");
    }
    file.close();
}
/*
从配置文件中读字符串
*/
String ReadConfigFromFile(){
    return readFile(FFat,CONFIG_PATH);
}
/*
写入配置文件到字符串
*/
void WriteConfigToFile(String message){
    writeFile(FFat,CONFIG_PATH,message.c_str());
}