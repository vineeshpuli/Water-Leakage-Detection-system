#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include<ArduinoJson.h>
#include <Wire.h>
void bytesToInt(byte b0, byte b1);       //converts bytes to int
int a; 
float c,e;
float d,f;
//#define FIREBASE_HOST "sm-project-83dc1-default-rtdb.firebaseio.com"             // the project name address from firebase id
//#define FIREBASE_AUTH "emuApb1zJztBDzN1p3JBiuNHA7kRZio56dLSewhu"       // the secret key generated from firebase
// Ensure correct credentials to connect to your WiFi Network.
char ssid[] = "IIITS_Student";
char pass[] = "iiit5@2k18";



// Ensure that the credentials here allow you to publish and subscribe to the ThingSpeak channel.
#define channelID 1694525
const char mqttUserName[] = "DhAyCTwjHxkOFxkUDjMZNCk"; 
const char clientID[] = "DhAyCTwjHxkOFxkUDjMZNCk";
const char mqttPass[] = "RVRAAX64cGSH/BpOyc33Eldv";

#define mqttPort 1883
WiFiClient client;

const char* server = "mqtt3.thingspeak.com";
int status = WL_IDLE_STATUS; 
long lastPublishMillis = 0;
int connectionDelay = 1;
int updateInterval = 15;
PubSubClient mqttClient( client );

void connectWifi()
{
  Serial.print(F("Connecting to "));
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}

void mqttConnect() {
  // Loop until connected.
  while ( !mqttClient.connected() )
  {
    // Connect to the MQTT broker.
    if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
      Serial.print( "MQTT to " );
      Serial.print( server );
      Serial.print (" at port ");
      Serial.print( mqttPort );
      Serial.println( " successful." );
    } else {
      Serial.print( "MQTT connection failed, rc = " );
      Serial.print( mqttClient.state() );
      Serial.println( " Will try again in a few seconds" );
      delay( connectionDelay*1000 );
    }
  }
}

void setup() {
  Serial.begin( 9600 );
  // Delay to allow serial monitor to come up.
  
  delay(500);
  Wire.begin(D1, D2);
  
  // Connect to Wi-Fi network.
  connectWifi();
 // Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
   //Configure the MQTT client
  mqttClient.setServer( server, mqttPort ); 
  
  //Set the MQTT message handler function.
  //mqttClient.setCallback( mqttSubscriptionCallback );
  
  //Set the buffer to handle the returned JSON. NOTE: A buffer overflow of the message buffer will result in your callback not being invoked.
  //mqttClient.setBufferSize( 2048 );
}
/*
 Function to handle messages from MQTT subscription.
void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int length ) {
  // Print the details of the message that was received to the serial monitor.
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

// Subscribe to ThingSpeak channel for updates.
void mqttSubscribe( long subChannelID ){
  String myTopic = "channels/"+String( subChannelID )+"/subscribe/fields/field1";
  String myTopic1 = "channels/"+String( subChannelID )+"/subscribe/fields/field2";
  String myTopic2 = "channels/"+String( subChannelID )+"/subscribe/fields/field3";
  mqttClient.subscribe(myTopic.c_str());
  mqttClient.subscribe(myTopic1.c_str());
  mqttClient.subscribe(myTopic2.c_str());
}
*/
// Publish messages to a ThingSpeak channel.
void mqttPublish(long pubChannelID, String message) {
  String topicString ="channels/" + String( pubChannelID ) + "/publish";
  mqttClient.publish( topicString.c_str(), message.c_str() );
}

void loop() {
  if ( (millis() - lastPublishMillis) > updateInterval*100)
  {
  //Firebase.setString("sm/Rate1", "hello");
  Wire.requestFrom(8, 8); /* request & read data of size 13 from slave */
 while(Wire.available()){
    byte b0 = Wire.read();
    byte b1 = Wire.read();
  bytesToInt(b0, b1); 
  c=(float)a/100;
  String rate1=String(c)+ String("L/M");
  //Firebase.setString("/KVV/a", "hello1");  
  Serial.println("hello1");
  Serial.println(c);
     b0 = Wire.read();
     b1 = Wire.read();
  bytesToInt(b0, b1); 
  d=(float)a/100;
  String volume=String(d)+ String("L");
  //Firebase.setString("/KVV/b", "hello2");  
  Serial.println("hello2");
  Serial.println(d);
  b0 = Wire.read();
     b1 = Wire.read();
  bytesToInt(b0, b1); 
  e=(float)a/100;
  String rate2=String(e)+ String("L");
  //Firebase.setString("/KVV/c/","hello3");  
  Serial.println("hello3");
  Serial.println(e);
  b0 = Wire.read();
  b1 = Wire.read();
  bytesToInt(b0, b1); 
  f=a;
 }
   //Reconnect to WiFi if it gets disconnected.
  if (WiFi.status() != WL_CONNECTED) {
      connectWifi();
  
  delay(500);
  }
  
   //Connect if MQTT client is not connected and resubscribe to channel updates.
  if (!mqttClient.connected()) {
     mqttConnect(); 
     //mqttSubscribe( channelID );
  }
  
  // Call the loop to maintain connection to the server.
  mqttClient.loop(); 
  
  // Update ThingSpeak channel periodically. The update results in the message to the subscriber.
   
    mqttPublish( channelID, (String("field1=")+String(e))+"&"+(String("field2=")+String(c))+"&"+(String("field3=")+String(d))+"&"+(String("field4=")+String(f)) );
    /*
    delay(10);
    mqttPublish( channelID, (String("field2=")+String(e)) );
    delay(10);
    mqttPublish( channelID, (String("field3=")+String(d)) );
    delay(10);
    mqttPublish( channelID, (String("field4=")+String(f)) );
    //delay(2000);
    */
    Serial.print("Flow rate-1:");
    Serial.println(e);
    Serial.print("Flow rate-2:");
    Serial.println(c);
    Serial.print("Volume:");
    Serial.println(d);
    Serial.print("Status:");
    Serial.println(f);
    lastPublishMillis = millis();
  }
}



void bytesToInt(byte b0, byte b1)
{
  a = (b0 << 8) | (b1);            //using Bitwise operators
}
