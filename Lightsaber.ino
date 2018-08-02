#include <LWiFi.h>
#include <WiFiClient.h>
// #include <LWebServer.h>

/* Set these to your desired credentials. */
const char *ssid = "Lightsaber_J";
const char *password = "10101010";

int led_pin[10] = {6,7,8,9,10,11,12,13,14,15};
int light[10] = {255,180,120,70,30,10,5,0,0,0};
int sw=1, pos=0;

WiFiServer server(80);

void setup() {
    for(int i=0;i<10;i++) {
        pinMode(led_pin[i], OUTPUT);
        digitalWrite(led_pin[i], LOW);
    }
    
    Serial.begin(115200);
    Serial.println();
    Serial.print("Configuring access point...");

    /* You can remove the password parameter if you want the AP to be open. */
    WiFi.softAP(ssid, password);
    IPAddress myIP = WiFi.softAPIP();
    Serial.println("AP ready.");
    Serial.print("Connect to AP and visit http://");
    Serial.println(myIP);
    server.begin();
    
    for(int i=0;i<10;i++) {
        digitalWrite(led_pin[i], HIGH);
        delay(30);
    }
    delay(50);
}

void loop() {
    WiFiClient client = server.available();   // listen for incoming clients

	if (client) {
		//Serial.println("new client");
		String currentLine = "";
		while (client.connected()) {
			if (client.available()) {
				char c = client.read();
				//Serial.write(c);
				//client.flush();
				
				if (c == '\n') {
					if (currentLine.length() == 0) {
						client.println("HTTP/1.1 200 OK");
						client.println("Content-type:text/html");
						client.println();

						// the content of the HTTP response follows the header:
						/*client.print("Click <a href=\"/swNum0\">here</a> turn the LED on State 0<br>");
						client.print("Click <a href=\"/swNum1\">here</a> turn the LED on State 1<br>");
						client.print("Click <a href=\"/swNum2\">here</a> turn the LED on State 2<br>");
						client.print("Click <a href=\"/swNum3\">here</a> turn the LED on State 3<br>");
						client.print("Click <a href=\"/swNum4\">here</a> turn the LED on State 4<br>");
						*/
						// The HTTP response ends with another blank line:
						client.println();
						break;
					} else {    // if you got a newline, then clear currentLine:
						currentLine = "";
					}
				} else if (c != '\r') {  // if you got anything else but a carriage return character,
					currentLine += c;      // add it to the end of the currentLine
				}
			}
			if(currentLine.indexOf("swNum") != -1) {
				pos = currentLine.indexOf("swNum") + 5;
    		sw = currentLine.substring(pos, pos+1).toInt();
			}
			
		}
		Serial.println(sw);
		// close the connection:
		client.stop();
		//Serial.println("client disonnected");
	}
	switch (sw) {
		case 0:
	    for(int i=0;i<10;i++) {
	    	analogWrite(led_pin[i], 0);
	    }
	    break;
	    
	  case 1:
	    for(int i=0;i<10;i++) {
	    	analogWrite(led_pin[i], 255);
	    }
	    break;
	    
	  case 2:
	    for(int i=0;i<10;i++) {
	    	analogWrite(led_pin[i], 0);
	    }
	    delay(100);
	    for(int i=0;i<10;i++) {
	    	analogWrite(led_pin[i], 255);
	    	delay(30);
	    }
	    break;
	    
	  case 3:
	    for(int i=0;i<10;i++) {
		  	analogWrite(led_pin[i], 0);
		  }
		  delay(200);
			for(int i=0;i<10;i++) {
		  	analogWrite(led_pin[i], 255);
		  }
		  delay(200);
	    break;
	    
	  case 4:
	    for(int i=0;i<10;i++) {
		  	analogWrite(led_pin[i], 0);
		  }
		  delay(200);
			for(int i=0;i<10;i++) {
		  	analogWrite(led_pin[i], 255);
		  	delay(30);
		  	analogWrite(led_pin[i], 0);
		  }
	    break;

	  case 5:
	    for(int i=0;i<255;i++) {
				for(int j=0;j<10;j++) {
					analogWrite(led_pin[j], i);
				}
				delay(5);
			}
			for(int i=255;i>0;i--) {
				for(int j=0;j<10;j++) {
					analogWrite(led_pin[j], i);
				}
				delay(5);
			}
			break;
	  
	  case 6:
			for(int i=0;i<10;i++) {
		    for(int j=0;j<10;j++) {
	        analogWrite(led_pin[j], light[abs(i-j)]);
		    }
		    delay(50);
			}
			for(int i=9;i>0;i--) {
		    for(int j=0;j<10;j++) {
	        analogWrite(led_pin[j], light[abs(i-j)]);
		    }
		    delay(50);
			}
			break;
			
	  default:
	    for(int i=0;i<10;i++) {
	    	analogWrite(led_pin[i], 255);
	    }
	    break;
	}
}


