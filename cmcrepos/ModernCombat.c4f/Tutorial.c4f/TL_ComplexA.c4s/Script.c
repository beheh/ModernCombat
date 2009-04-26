/*-- Komplex A --*/

#strict
#include CSTD

static station;//Aktuelle Stationsnummer.

func Initialize()
{
  //Szenario einrichten
  CreateFurniture();
  ScriptGo(true);
  return(1);
}

func CreateFurniture()
{
  //Alle Objekte folglich von links oben nach rechts unten
  //Rampen
  DrawMaterialQuad("Wall-Stripes",220,640,250,630,250,640,235,640,true);
  DrawMaterialQuad("Wall-Stripes",250,630,280,620,280,630,265,630,true);
  DrawMaterialQuad("Wall-Stripes",280,620,310,610,310,620,295,620,true);

  DrawMaterialQuad("Wall-Stripes",440,600,470,590,470,600,455,600,true);
  DrawMaterialQuad("Wall-Stripes",410,610,440,600,440,610,425,610,true);

  DrawMaterialQuad("Wall-Concrete1",1030,290,1060,280,1060,290,1045,290,true);
  DrawMaterialQuad("Wall-Concrete1",1060,280,1090,270,1090,280,1075,280,true);

  DrawMaterialQuad("Wall-Concrete1",1311,280,1281,270,1281,280,1296,280,true);
  DrawMaterialQuad("Wall-Concrete1",1341,290,1311,280,1311,290,1326,290,true);

  DrawMaterialQuad("Wall-Concrete1",1360,290,1390,280,1390,290,1375,290,true);

  DrawMaterialQuad("Wall-Concrete1",1540,420,1570,410,1570,420,1555,420,true);
  DrawMaterialQuad("Wall-Concrete1",1621,420,1591,410,1591,420,1606,420,true);

  DrawMaterialQuad("Wall-Stripes",1980,420,2010,410,2010,420,1995,420,true);
  DrawMaterialQuad("Wall-Stripes",2010,410,2040,400,2040,410,2025,410,true);

  //Benzinfässer
  CreateObject(PBRL, 20, 640, -1);
  CreateObject(PBRL, 40, 640, -1);
  CreateObject(PBRL, 2050, 400, -1);

  //Aufzug
  CreateObject(LFTP, 535, 475, -1);

  //Glasscheiben
  CreateObject(_WIN, 507, 325, -1);
  CreateObject(_WIN, 507, 354, -1);
  CreateObject(_WIN, 507, 383, -1);
  CreateObject(_WIN, 507, 412, -1);
  CreateObject(_WIN, 507, 441, -1);
  CreateObject(_WIN, 507, 470, -1);

  CreateObject(_WIN, 563, 325, -1);
  CreateObject(_WIN, 563, 354, -1);
  CreateObject(_WIN, 563, 383, -1);
  CreateObject(_WIN, 563, 412, -1);
  CreateObject(_WIN, 563, 441, -1);
  CreateObject(_WIN, 563, 470, -1);

  CreateObject(_WIN, 597, 561, -1);
  CreateObject(_WIN, 597, 590, -1);

  //Leitern
  CreateObject(LADR, 1350, 420, -1)->Set(15);
  CreateObject(LADR, 1155, 680, -1)->Set(20);

  //Kisten
  CreateObject(WCR2, 460, 280, -1);
  CreateObject(WCR2, 490, 280, -1);

  //Gasflasche
  CreateObject(GSBO, 570, 280, -1);

  //Munitionskiste (Kugeln)
  CreateObject(ABOX, 580, 590, -1)->Set(AMOC);

  //Satellitenschüssel
  CreateObject(SADH, 395, 470, -1);

  //Flutlichter
  CreateObject(FLGH, 90, 440, -1)->SetRotation(20);
  CreateObject(FLGH, 585, 470, -1)->SetRotation(65);
  CreateObject(FLGH, 675, 630, -1)->SetRotation(85);
  CreateObject(FLGH, 1225, 420, -1)->SetRotation(90);
  CreateObject(FLGH, 1960, 420, -1)->SetRotation(-45);
  CreateObject(FLGH, 2155, 240, -1)->SetRotation(-20);

  //Radarbildschirm
  CreateObject(SCA1, 535, 420, -1);

  //Laborlichter
  CreateObject(LLGH, 370, 555, -1);
  CreateObject(LLGH, 420, 555, -1);
  CreateObject(LLGH, 510, 555, -1);
  CreateObject(LLGH, 565, 555, -1);
  CreateObject(LLGH, 610, 270, -1);

  //Deckenlichter
  CreateObject(CLGH, 45, 615, -1);
  CreateObject(CLGH, 2170, 375, -1);

  //Geländer
  CreateObject(RAI1, 712, 280, -1)->SetRail([1,1,1,1,1,1]);
  CreateObject(RAI1, 890, 210, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1170, 190, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1370, 190, -1)->SetRail([1,1,1]);
  CreateObject(RAI1, 1570, 410, -1)->SetRail([1]);

  //Markierungen
  CreateObject(MSGN, 635, 280, -1);
  CreateObject(MSGN, 715, 280, -1);
  CreateObject(MSGN, 1855, 420, -1);
  CreateObject(MSGN, 1935, 420, -1);

  //Bodenlucke
  CreateObject(HA4K, 1350, 293, -1);

  //Hydrauliktüren
  CreateObject(SLDR, 95, 640, -1);
  CreateObject(SLDR, 2145, 400, -1);

  //Steine
  CreateObject(RO13, 720, 650, -1);
  CreateObject(RO13, 1085, 670, -1);
  CreateObject(RO13, 1175, 435, -1);
  CreateObject(RO13, 1575, 710, -1);
  CreateObject(RO13, 1835, 560, -1);

  //Büsche
  CreateObject(BSH2, 1130, 660, -1);
  CreateObject(BSH2, 1195, 530, -1);
  CreateObject(BSH2, 1230, 600, -1);
  CreateObject(BSH2, 1385, 620, -1);
  CreateObject(BSH2, 1435, 565, -1);
  CreateObject(BSH2, 1655, 650, -1);

  //Lianen
  CreateObject(VINE, 1180, 465, -1);
  CreateObject(VINE, 1205, 635, -1);
  CreateObject(VINE, 1225, 635, -1);
  CreateObject(VINE, 1235, 530, -1);
  CreateObject(VINE, 1270, 530, -1);
  CreateObject(VINE, 1305, 520, -1);
  CreateObject(VINE, 1325, 530, -1);
  CreateObject(VINE, 1365, 480, -1);
  CreateObject(VINE, 1455, 505, -1);
  CreateObject(VINE, 1590, 540, -1);
  CreateObject(VINE, 1625, 490, -1);
  CreateObject(VINE, 1645, 570, -1);
}


/* Script0-X */
func Script10()
{
  SetCrewEnabled(true, GetCrew());
  SetCursor(0, GetCrew(), true, true);
  DoInfoMessage(GetCrew(),"Commander","Willkommen zur erweiterten Ausbildung, Soldat!|*rhabarber* *rhabarber*||<c ffff00>Gehe zum Schießstand.</c> <i><c 555555>\\o/ Welcher Schießstand?</c></i>", PCMK, RGB(80,100,0),1);
}

func Script12()
{
  if((LandscapeWidth() - GetX(GetCursor())) < 40)
    DoInfoMessage(GetCrew(),"Henry","So, hier ist Ende Gelände! =P|<i><c ff0000>Insert coin here.</c></i>", PCMK, RGB(255),1);
  else
    goto(11);
}


/* Stationen */
func Station0(object pClonk)
{
  goto();//zurücksetzen

  pClonk->SetPosition(8,630);
  SetCrewEnabled(false, pClonk);
  SetCommand(pClonk,"MoveTo",0,138,630);
}



/* Respawn */

public func OnClonkEquip(object pClonk)
{
  //... leer
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  iX = 10;
  iY = 10;
}

public func OnClassSelection(object pClonk)
{
  //Station für den Clonk initialisieren. :D
  GameCall(Format("Station%d",station),pClonk);
}