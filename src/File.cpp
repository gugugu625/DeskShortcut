#include "Include.h"

String readFile(fs::FS &fs, const char * path){
    //Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return "";
    }

    Serial.println("- read from file");
    String res = "";
    while(file.available()){
        char c = file.read();
        //Serial.print(c);
        res += c;
    }
    file.close();
    //Serial.println("");
    //Serial.println(res);
    return res;
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

String ReadConfigFromFile(){
    return readFile(FFat,CONFIG_PATH);
}

void WriteConfigToFile(String message){
    writeFile(FFat,CONFIG_PATH,message.c_str());
}