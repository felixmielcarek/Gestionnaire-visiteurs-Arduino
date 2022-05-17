//pour l'écran
#include "TFT_eSPI.h"
TFT_eSPI tft;
//pour l'heure
#include "RTC_SAMD51.h"
#include "DateTime.h"
RTC_SAMD51 rtc;
DateTime now = DateTime(F(__DATE__), F(__TIME__));

//initialisation des variables gloabales
bool fondVert=true;
int affl=0;
int visit=0;
const int afflMax=11;
bool messNonLu=false; //il y a un message non lu
bool messagerie=false;  //il y a au moins un message qui a été reçu
String mess;
bool testSerial=true; //test si la communication série fonctionne
bool testSerialHeure=false; //test si l'heure a été affiché au quart d'heure précédent 
 
void setup() {
  Serial.begin(115200); //initialise la communication série
  rtc.begin();

  //initialise les boutons du WIO
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  pinMode(WIO_BUZZER, OUTPUT);  //initialise le buzzer du WIO

  //configure l'écran du WIO
  tft.begin();
  tft.setRotation(3); //initialise le coin de l'écran de coordonnées (0,0)
  tft.setTextColor(TFT_BLACK);  //change la couleur du texte
  tft.setTextSize(3); //change la taille du texte

  //test si l'USB est bien branché au WIO
  while(!Serial)
  {
    if(testSerial)
    {
      tft.fillScreen(TFT_GREEN);
      tft.setCursor(2,10);
      tft.print("Need serial com");
      testSerial=false;
    }
  }

  //initialisation de l'heure
  rtc.adjust(now);  //initialise l'horloge du Wio avec celle du PC
  now = rtc.now();

  tft.fillScreen(TFT_GREEN);
  initAffich();  
}

//affichage du rectangle bleu en haut
void initMenu()
{
  tft.fillRect(0,0,320,45,TFT_BLUE);
  tft.drawChar(10,10,'+',TFT_BLACK,TFT_BLUE,3);
  tft.drawChar(80,10,'-',TFT_BLACK,TFT_BLUE,3);
  tft.setCursor(150,10);  //positionne le curseur aux coordonnées (150,10)
  tft.print("RAZ");
  heure();
}

//affichage de l'heure en haut à droite
void heure()
{
  tft.fillRect(215,0,400,45,TFT_BLUE);
  now = rtc.now();
  tft.setCursor(220,10);
  if(now.hour()<10) //affiche un zéro si l'heure ne prend qu'un chiffre
  {
    tft.print('0');
  }
  tft.print(now.hour(), DEC);
  tft.print(':');
  if(now.minute()<10)
  {
    tft.print('0');
  }
  tft.print(now.minute(), DEC);

  if(now.minute()==0 || now.minute()==15 || now.minute()==30 || now.minute()==45)
  {
    if(!testSerialHeure)
    {
      heureSerial();
      testSerialHeure=true;
    }    
  }
  if(now.minute()==2 || now.minute()==17 || now.minute()==32 || now.minute()==47)
  {
    testSerialHeure=false;
  }
}

//affichage de l'heure dans la console
void heureSerial()
{
  Serial.println(F("----------"));
    
  Serial.print(F("Heure : "));
  if(now.hour()<10)
  {
    Serial.print(F("0"));
  }
  Serial.print(now.hour(), DEC);
  Serial.print(F(":"));
  if(now.minute()<10)
  {
    Serial.print(F("0"));
  }
  Serial.println(now.minute(), DEC);

  Serial.print(F("Visiteurs : "));
  Serial.println(visit);
  Serial.print(F("Affluence : "));
  Serial.println(affl);
}

//change le fond de vert à rouge ou inversement
void majFond()
{
  if(fondVert)
  {
    tft.fillScreen(TFT_RED);
    fondVert=false;
  }
  else
  {
    tft.fillScreen(TFT_GREEN);
    fondVert=true;
  }
  initAffich();
}

//met le texte de la même couleur que le fond (pour effacer message)
void majTxtFond()
{
  if(fondVert)
  {
    tft.setTextColor(TFT_GREEN);
  }
  else
  {
    tft.setTextColor(TFT_RED);
  }
}

//affichage principal
void initAffich()
{
  tft.setCursor(2,50);
  tft.print("Visiteurs : ");  
  tft.setCursor(2,80);
  tft.print("Affluence : ");
  if(messNonLu)
  {
    message();
  }
  majCompteur(affl,visit);
  initMenu();
}

//met à jour les compteurs
//prends en paramètre la nouvelle valeur de l'affluence et la nouvelle valeur des visiteurs
void majCompteur(int newA,int newV)
{  
  //efface les valeurs
  majTxtFond();  
  tft.setCursor(220,50);
  tft.print(visit);
  tft.setCursor(220,80);
  tft.print(affl);

  //ecrit les valeurs
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(220,50);
  tft.print(newV);
  tft.setCursor(220,80);
  tft.print(newA);

  affl=newA;
  visit=newV;
}

//remet à 0 les 2 compteurs
void raz()
{
  //confirmation de 5 sec
  for(int i=0; i<5000; i=i+100)
  {
    tft.setCursor(100,200);
    tft.print("Confirmez ?");
    
    //remise à zéro
    if(digitalRead(WIO_5S_PRESS)==LOW)
    {
      tft.fillScreen(TFT_GREEN);
      fondVert=true;
      initAffich();
      majCompteur(0,0);
      //affichage dans la console
      Serial.println(F("----------"));
      Serial.println(F("Remise des compteurs à zéro !"));
      heureSerial();
      break;
    }
    
    //nouvelle entrée
    if(digitalRead(WIO_KEY_C)==LOW)
    {
      entree();
      tft.setCursor(100,200);
      tft.print("Confirmez ?");
    }
    
    //sortie d'un visiteur
    if(digitalRead(WIO_KEY_B)==LOW)
    {
      sortie();
      tft.setCursor(100,200);
      tft.print("Confirmez ?");
    }
    delay(100);
  }

  majTxtFond();
  tft.setCursor(100,200);
  tft.print("Confirmez ?");
  tft.setTextColor(TFT_BLACK);
}

//comptabilise une entree
void entree()
{
  majCompteur(affl+1,visit+1);
  
  //si on dépasse l'affluence max
  if(affl==afflMax)
  {
    majFond();
    initAffich();
    analogWrite(WIO_BUZZER, 128);
    delay(500);
    analogWrite(WIO_BUZZER, 0);
  }
}

//comptabilise une sortie
void sortie()
{
  //si on redescend en dessous de l'affluence max
  if(affl==afflMax)
  {
    majFond();
  }
  //si il n'y a pas de visiteurs
  if(affl!=0)
  {
    majCompteur(affl-1,visit);
  }
}

//affichage notification d'un message
void message()
{
  tft.setCursor(2,110);
  tft.print("Nouveau message");
}

//gestion de l'affichage du message
void menuMess()
{
  if(messagerie)
  {
    affichMess();
    messNonLu=false;
    if(fondVert)
    {
      tft.fillScreen(TFT_GREEN);
    }
    else
    {
      tft.fillScreen(TFT_RED);
    }
    initAffich();
  }
}

//affichage du dernier message reçu
void affichMess()
{
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(2,10);
  tft.print(mess);
  tft.setCursor(2,200);
  tft.print("DOWN pour sortir");

  //minuteur de 30 secondes
  for(int i=0; i<30000; i=i+100)
  {
    if(digitalRead(WIO_5S_DOWN)==LOW)
    {
      return;
    }
    delay(100);
  }  
}

void loop() {
  //affichage heure
  heure();
  
  //si un message est reçu
  if (Serial.available())
  {
    analogWrite(WIO_BUZZER, 50);
    delay(250);
    analogWrite(WIO_BUZZER, 100);
    delay(250);
    analogWrite(WIO_BUZZER, 150);
    delay(250);
    analogWrite(WIO_BUZZER, 0);
    messNonLu=true;
    messagerie=true;
    mess = Serial.readString();
    message();
  }

  //affichage du dernier message
  if(digitalRead(WIO_5S_UP)==LOW)
  {
    menuMess();    
  }
  
  //remise à zéro
  if(digitalRead(WIO_KEY_A)==LOW)
  {
    raz();
  }
  
  //nouvelle entrée
  if(digitalRead(WIO_KEY_C)==LOW)
  {
    entree();
  }
  
  //sortie d'un visiteur
  if(digitalRead(WIO_KEY_B)==LOW)
  {
    sortie();
  }
  delay(80);
}
