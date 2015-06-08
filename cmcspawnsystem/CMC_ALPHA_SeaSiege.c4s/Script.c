/*-- Tropical Storm --*/

#strict
#include CSTD

static aFlag,aSelfDefense,aStationary,aTowerInterior, doorw, pRoom;

func RecommendedGoals()			{return [GOCC, GASS];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0), RGB(152,147,128), RGB(255,254,236) );
  //Flaggen
  aFlag = [];
  //Geschützstellungen
  aStationary = [];
  //Selbstschussanlagen
  aSelfDefense = [];
  //Turmobjekte
  aTowerInterior = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  return(1); 
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Leitern
  CreateObject(LADR, 2340, 1010, -1)->Set(4);
  CreateObject(LADR, 2450, 1010, -1)->Set(4);
  CreateObject(LADR, 3310, 926, -1)->Set(6);
  CreateObject(LADR, 4970, 1002, -1)->Set(18);
  CreateObject(LADR, 5080, 1002, -1)->Set(18);
  CreateObject(LADR, 6740, 926, -1)->Set(6);
  CreateObject(LADR, 7600, 1010, -1)->Set(4);
  CreateObject(LADR, 7710, 1010, -1)->Set(4);

  //Bodenluken
  CreateObject(HA4K, 2340, 973, -1);
  CreateObject(HA4K, 2450, 973, -1);
  CreateObject(HA4K, 3310, 873, -1);
  CreateObject(HA4K, 4970, 853, -1);
  CreateObject(HA4K, 4970, 943, -1);
  CreateObject(HA4K, 5080, 853, -1);
  CreateObject(HA4K, 5080, 943, -1);
  CreateObject(HA4K, 6740, 873, -1);
  CreateObject(HA4K, 7600, 973, -1);
  CreateObject(HA4K, 7710, 973, -1);

  //Kisten
  CreateObject(WCR2, 2360, 970, -1)->AutoRespawn();
  CreateObject(WCR2, 7690, 970, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 3340, 870, -1)->AutoRespawn();
  CreateObject(MWCR, 6710, 870, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 2380, 970, -1)->AutoRespawn();
  CreateObject(BECR, 7670, 970, -1)->AutoRespawn();

/*
  //Explosive Kisten
  CreateObject(XWCR, 1845, 1170, -1)->AutoRespawn();
  CreateObject(XWCR, 3600, 868, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 1620, 1260, -1)->AutoRespawn();
  CreateObject(PBRL, 3660, 850, -1)->AutoRespawn();
  CreateObject(PBRL, 3670, 292, -1);
  CreateObject(PBRL, 4505, 938, -1)->AutoRespawn();
*/
  //Explosivfässer
  CreateObject(XBRL, 2260, 970, -1)->AutoRespawn();
  CreateObject(XBRL, 4900, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 5150, 800, -1)->AutoRespawn();
  CreateObject(XBRL, 7790, 970, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 3390, 930, -1)->AutoRespawn();
  CreateObject(GSBL, 6660, 930, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 4815, 862, -1)->SwitchMode();
  CreateObject(_HBR, 4905, 862, -1)->SwitchMode();
  CreateObject(_HBR, 5025, 862, -1)->SwitchMode();
  CreateObject(_HBR, 5145, 862, -1)->SwitchMode();
  CreateObject(_HBR, 5235, 862, -1)->SwitchMode();

  //Gerüste
  CreateObject(SFFG, 4920, 700, -1)->Set(5);
  CreateObject(SFFG, 4920, 750, -1)->Set(3);
  CreateObject(SFFG, 4920, 800, -1)->Set(2);
  CreateObject(SFFG, 4920, 850, -1)->Set(4);
  CreateObject(SFFG, 5130, 700, -1)->Set(5);
  CreateObject(SFFG, 5130, 750, -1)->Set(2);
  CreateObject(SFFG, 5130, 800, -1)->Set(3);
  CreateObject(SFFG, 5130, 850, -1)->Set(4);

/*
  CreateObject(SFFG, 5470, 1280, -1)->Set(4);
  var plat = CreateObject(SFFG, 5470, 1330, -1);
  plat->Set(5);
  plat->SetClrModulation(RGB(125,125,250));
  CreateObject(SFFG, 5530, 1280, -1)->Set(4);
  plat = CreateObject(SFFG, 5530, 1330, -1);
  plat->Set(5);
  plat->SetClrModulation(RGB(125,125,250));
*/

  //Haie
  CreateObject(SHRK, 200, 1200, -1)->AutoRespawn();
  CreateObject(SHRK, 2830, 1100, -1)->AutoRespawn();
  CreateObject(SHRK, 2930, 1100, -1)->AutoRespawn();
  CreateObject(SHRK, 3980, 1200, -1)->AutoRespawn();
  CreateObject(SHRK, 6070, 1200, -1)->AutoRespawn();
  CreateObject(SHRK, 7120, 1100, -1)->AutoRespawn();
  CreateObject(SHRK, 7220, 1100, -1)->AutoRespawn();
  CreateObject(SHRK, 9850, 1200, -1)->AutoRespawn();

  //Piranha
  CreateObject(PIRA, 1430, 1100, -1)->AutoRespawn();
  CreateObject(PIRA, 2490, 1200, -1)->AutoRespawn();
  CreateObject(PIRA, 3560, 1300, -1)->AutoRespawn();
  CreateObject(PIRA, 6490, 1300, -1)->AutoRespawn();
  CreateObject(PIRA, 7560, 1200, -1)->AutoRespawn();
  CreateObject(PIRA, 8620, 1100, -1)->AutoRespawn();

/*
  //Container
  CreateObject(CON1, 1780, 1171, -1)->SetGraphics("5");
  var container = CreateObject(CON1, 1820, 1171, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
  container = CreateObject(CON1, 4530, 940, -1);
  container->SetPerspective(2);
  container->SetGraphics("5");
*/
  //Explosivtanks
  CreateObject(XTNK, 3415, 930, -1)->AutoRespawn();
  CreateObject(XTNK, 6635, 930, -1)->AutoRespawn();
/*
  //Tore und Konsolen
  var autod = CreateObject (HNG3, 1433, 1110, -1);
  autod->Open();
  CreateObject (CONS, 1500, 1105, -1)->Set(autod);

  autod = CreateObject (HNG3, 1637, 1110, -1);
  autod->Open();
  CreateObject (CONS, 1570, 1105, -1)->Set(autod);

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject(SEGU, 1310, 1142, -1);
    aSelfDefense[0]->Arm(MISA);
    aSelfDefense[0]->SetR(180);
    CreateObject(CONS, 1420, 1025, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 2340, 1142, -1);
    aSelfDefense[1]->Arm(MISA);
    aSelfDefense[1]->SetR(180);
    CreateObject(CONS, 1650, 1025, -1)->Set(aSelfDefense[1]);

*/
  //Seegras und Muscheln entfernen
  for(var obj in FindObjects(Find_Or(Find_ID(SWD1), Find_ID(SWD2), Find_ID(SHEL)), Find_InRect(2250, 800, 5550, 180)))
    RemoveObject(obj);

  //Sonne umplazieren
  FindObject(LENS)->SetPosition(5025,145);

  //Sendemast
  var tower = CreateObject(AATR, 5025, 940, -1);
  tower->AddNode(4410, 1185, 2, CreateObject(REHR, 4405, 1190, -1), 0, 2);
  tower->AddNode(4748, 1135, 0, CreateObject(REHR, 4750, 1140, -1), 0, 2);
  tower->AddNode(5298, 1135, 0, CreateObject(REHR, 5300, 1140, -1), 0, 2);
  tower->AddNode(5640, 1185, 2, CreateObject(REHR, 5645, 1190, -1), 0, 2);

  //Verbundene Räume
  /*
  var doorw2 = CreateObject(GAT1, 2395, 935, -1);
  CreateObject(ROOM, 3500, 940, -1)->Connect(doorw2);
  doorw2 = CreateObject(GAT1, 7655, 935, -1);
  CreateObject(ROOM, 6550, 940, -1)->Connect(doorw2);
  */
  doorw = CreateObject(GAT1, 5025, 490, -1);
  pRoom = CreateObject(ROOM, 5025, 940, -1);
  pRoom->Connect(doorw);

  //Sounds

  //Wind
  CreateObject(SE4K, 1100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 2100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 3100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 4100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 5100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 6100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 7100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 8100, 300, -1)->Set("WindSound*.ogg",3000,105);
  CreateObject(SE4K, 9100, 300, -1)->Set("WindSound*.ogg",3000,105);

  //Hallen
  CreateObject(SE4K, 2390, 850, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 3500, 840, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 5025, 740, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 6550, 840, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 7660, 850, -1)->Set("Interior*.ogg",665,105);

  //Möven
  CreateObject(SE4K, 500, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 1300, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 3880, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 6170, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 8750, 950, -1)->Set("SeaSounds*.ogg",140,35);
  CreateObject(SE4K, 9550, 950, -1)->Set("SeaSounds*.ogg",140,35);

  //Erdrutsche
  CreateObject(SE4K, 1065, 980, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 8990, 980, -1)->Set("FallingDirt*.ogg",950,250);

  //Wellen
  CreateObject(SE4K, 500, 1000, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 1300, 1000, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 2890, 1000, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 4270, 1000, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5025, 1000, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 5770, 1000, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 7160, 1000, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 8760, 1000, -1)->Set("Wave*.ogg",105,35);
  CreateObject(SE4K, 9580, 1000, -1)->Set("Wave*.ogg",105,35);

  //Jungel
  CreateObject(SE4K, 1520, 950, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 1950, 780, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 8120, 780, -1)->Set("Jungle*.ogg",140,70);
  CreateObject(SE4K, 8525, 950, -1)->Set("Jungle*.ogg",140,70);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 2280, 970, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 4950, 940, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 7770, 970, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject(AMCT, 2320, 970, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 5100, 940, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 7730, 970, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 5025, 845);

  //Geschützstellungen
  CreateObject(GNET, 2040, 870, -1)->Set(0,90,1);
  CreateObject(GNET, 2475, 970, -1)->Set(0,90,1);
  CreateObject(GNET, 3765, 960, -1)->Set(0,90);
  CreateObject(GNET, 6285, 960, -1)->Set(0,-90);
  CreateObject(GNET, 7575, 970, -1)->Set(0,-90,1);
  CreateObject(GNET, 8010, 870, -1)->Set(0,-90,1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");
/*
  //Radio
  CreateObject(RDIO, 1460, 980, -1);

  //Labortische
  CreateObject(LTBL, 1450, 1110, -1);
  CreateObject(LTBL, 1620, 1110, -1);

  //Monitore
  CreateObject(MNI2, 1440, 1098, -1);
  CreateObject(MNI2, 1460, 1098, -1);
  CreateObject(MNI2, 1610, 1098, -1)->Off();
  CreateObject(MNI2, 1630, 1098, -1);
  CreateObject(MNI2, 1690, 1152, -1);

  //Bildschirm
  CreateObject(SCA2, 1535, 1095, -1);
*/
  //Jetwrack
  CreateObject(_JW1, 3070, 1360, -1)->SetR(60);
  CreateObject(_JW2, 3160, 1375, -1)->SetR(20);

  //Kaputte U-Boote
  CreateObject(SUBB, 440, 1340, -1)->SetR(-20);
  CreateObject(SUBB, 7100, 1220, -1)->SetR(20);

  //Schiffswracks
  CreateObject(_WRK, 1630, 1360, -1);
  CreateObject(_WRK, 8380, 1360, -1);

  //Truhen
  CreateObject(CHST, 1510, 1340, -1);
  CreateObject(CHST, 8175, 1320, -1);

  //Kaputte Boote
  CreateObject(SLBB, 2240, 1310, -1)->SetR(40);
  CreateObject(SLBB, 7410, 1310, -1)->SetR(-30);

/*
  //Regale
  CreateObject(FRAM, 1440, 1170, -1);
  CreateObject(FRAM, 1630, 1170, -1);
  CreateObject(FRAM, 1650, 1170, -1);

*/
  //Radare
  CreateObject(RADR, 2395, 850, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 7650, 850, -1)->SetClrModulation(RGB(125,125,125));
/*
  //Schilder
  CreateObject(MSGN, 1288, 1260, -1);
  CreateObject(MSGN, 1428, 1260, -1);
  CreateObject(ESGN, 1500, 1025, -1);
  CreateObject(MSGN, 1710, 1170, -1);
  CreateObject(MSGN, 1745, 1170, -1);
  CreateObject(MSGN, 1776, 1260, -1);
  CreateObject(MSGN, 1870, 1170, -1);
  CreateObject(MSGN, 1905, 1170, -1);
  CreateObject(MSGN, 2030, 1170, -1);
  CreateObject(MSGN, 2065, 1170, -1);
  CreateObject(MSGN, 2190, 1170, -1);
  CreateObject(MSGN, 2225, 1170, -1);
  CreateObject(MSGN, 2376, 1260, -1);

  //Apparaturen
  CreateObject(GADG, 4690, 1240, -1)->Set(1);
  CreateObject(GADG, 4700, 1240, -1)->Set(1);
  CreateObject(GADG, 4710, 1240, -1)->Set(1);

  //Orientierungslichter
  CreateObject(OLGH, 1310, 1140, -1)->Set(2, 10, 1, 1, 30);
  CreateObject(OLGH, 1540, 980, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 1960, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2120, 1170, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2340, 1140, -1)->Set(2, 10, 1, 1, 30);

  //Büsche
  CreateObject(BSH2, 3430, 990, -1);
  CreateObject(BSH2, 3780, 1080, -1);
  CreateObject(BSH2, 3880, 1070, -1);
  CreateObject(BSH2, 3870, 720, -1);
  CreateObject(BSH2, 3980, 545, -1);
  CreateObject(BSH2, 4000, 540, -1);
  CreateObject(BSH2, 4020, 540, -1);
  CreateObject(BSH2, 4040, 545, -1);
  CreateObject(BSH2, 4300, 670, -1);
  CreateObject(BSH2, 4365, 1010, -1);
  CreateObject(BSH2, 4390, 1020, -1);
  CreateObject(BSH2, 4430, 1040, -1);
  CreateObject(BSH2, 4600, 1000, -1);
  CreateObject(BSH2, 4760, 1100, -1);
  CreateObject(BSH2, 5445, 1250, -1);
  CreateObject(BSH2, 5520, 1255, -1);
*/
  //Steine
  CreateObject(STNE, 305, 1260, -1);
  CreateObject(STNE, 1920, 1310, -1);
  CreateObject(STNE, 2770, 1320, -1);
  CreateObject(STNE, 4410, 1220, -1);
  CreateObject(STNE, 6890, 1370, -1);
  CreateObject(STNE, 8550, 1340, -1);
  CreateObject(STNE, 9400, 1320, -1);
/*
  //Spind
  CreateObject(LCKR, 1435, 1170, -1);

  //Rohre
  CreateObject(PIPL, 1290, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1320, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1950, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 1980, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2110, 1325, -1)->Up()->Up();
  CreateObject(PIPL, 2140, 1325, -1)->Up()->Up();
*/
  //Geländer
  CreateObject(RAI1, 3241, 870, -1)->SetRail([3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3]);
  CreateObject(RAI1, 6671, 870, -1)->SetRail([3,3,3,3,3,3,3,3,1,3,1,3,1,3,1,3,1,3,3,3,3,3,3,3,3]);

  //Lüftungsgitter
  CreateObject(ENGT, 2395, 900, -1);
  CreateObject(ENGT, 3500, 890, -1);
  CreateObject(ENGT, 6550, 890, -1);
  CreateObject(ENGT, 7655, 900, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  var back = CreateObject(BD03,1200,400,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD03,3300,300,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD03,5400,350,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD05,1550,1200,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD08,400,1300,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGB(125,125,125));
  back = CreateObject(BD08,1300,1300,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGB(125,125,125));
  back = CreateObject(BD05,3650,1130,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGBa(50,50,50,5));
  back = CreateObject(BD08,3700,1300,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGB(125,125,125));
  back = CreateObject(BD08,4600,1300,-1);
  if(FindObject(STRM))
   back->SetClrModulation(RGB(125,125,125));
}

/* Bei Turmzusammenfall */

func OnTowerCollapse()
{
  EventInfo4K(0,Format("$MsgCollapse$"),AATR, 0, 0, 0, "Info_Event.ogg");

  //Türverbindung entfernen
  doorw->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  RemoveObject(doorw, true);
  pRoom->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  pRoom->Lock();
  pRoom->SetAction("Idle");
  var pContent;
  while(pContent = Contents(0, pRoom))
    pRoom->Exit(pContent);

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten verschieben
   aFlag[2]->MoveFlagpost(5025,940,0,"$Flag6$");
  }
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{

}

/* Sturmerstellung */

func FormStorm()
{
  //Sturm erstellen
  CreateObject(STRM);

  //Dunkelheit erhöhen
  if(GetDarkness() < 3)
   FadeDarkness(3,60);

  //Hintergründe verdunkeln
  var back = FindObjects(Find_Func("IsDeco"));
  for (var pObj in back)
   pObj->SetClrModulation(RGBa(50,50,50,5));
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Blue Ressort.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Blue Ressort.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Zeitverzögertes Gewitter
  Schedule("GameCall(\"FormStorm\")", RandomX(6000,10000));

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,2430,970,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1800,890);
   aFlag[0] -> AddSpawnPoint(1980,830);
   aFlag[0] -> AddSpawnPoint(2030,960);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,3590,930,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(3260,860);
   aFlag[1] -> AddSpawnPoint(3270,950);
   aFlag[1] -> AddSpawnPoint(3355,950);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,5025,564,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(4780,840);
   aFlag[2] -> AddSpawnPoint(4950,840);
   aFlag[2] -> AddSpawnPoint(5100,840);
   aFlag[2] -> AddSpawnPoint(5270,840);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,6460,930,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(6695,950);
   aFlag[3] -> AddSpawnPoint(6780,950);
   aFlag[3] -> AddSpawnPoint(6790,860);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,7620,970,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(8020,960);
   aFlag[4] -> AddSpawnPoint(8070,830);
   aFlag[4] -> AddSpawnPoint(8245,890);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag4$",0,2);
   }

/*
   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
*/

   //Grenzen setzen
   CreateObject(BRDR, 1115, 0, -1)->Set(0);
   CreateObject(BRDR, 8930, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 1130, 1010, -1);
   CreateObject(SGNP, 8920, 1010, -1);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2340,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2450,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3350,960,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4900,940,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,5150,940,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,6690,960,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,7600,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,7710,1009,-1),50*21);
  }

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Strukturen
   AddAssaultTarget(CCP2, 2860, 990, 300, 1, "$Flag1$", 0, [[1800,890], [1980,830], [2030,960]]);
   AddAssaultTarget(CMSN, 4270, 990, 300, 1, "$Flag2$", 1, [[3260,860], [3270,950], [3355,950]]);
   AddAssaultTarget(CCP2, 5780, 990, 300, 2, "$Flag5$", 0, [[6695,950], [6780,950], [6790,860]]);
   AddAssaultTarget(CMSN, 7190, 990, 300, 2, "$Flag4$", 1, [[8020, 960], [8070, 830], [8245, 890]]);

   //Sprengsatz-Spawn
   SetupBombSpawnpoint(5025, 900);

   //Grenzen setzen
   CreateObject(BRDR, 1115, 0, -1)->Set(0);
   CreateObject(BRDR, 8930, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 1130, 1010, -1);
   CreateObject(SGNP, 8920, 1010, -1);

   //Selbstschussanlagen
   var selfdef = CreateObject(SEGU, 3500, 810, -1);
   selfdef->Set(0,1,1,180);
   selfdef->SetTeam(1);
   selfdef = CreateObject(SEGU, 6550, 810, -1);
   selfdef->Set(0,1,1,180);
   selfdef->SetTeam(2);

   //Patrouillenboote
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2340,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,2450,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,3350,960,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,4900,940,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Right,CreateObject(VSPW,5150,940,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,6690,960,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,7600,1009,-1),50*21);
   SetupVehicleSpawn([PBOT],DIR_Left,CreateObject(VSPW,7710,1009,-1),50*21);
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1 und 2
  if (!iIndex || iIndex == 1)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 910, 0, -1)->Set(0,1);
    CreateObject(BRDR, 4580, 0, -1)->Set(1,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3760,0,1,1);

    //Geschützstellungen entfernen
    aStationary[1]->DecoExplode(30);
    aStationary[2]->DecoExplode(30);
   }
  }

  //Ziel 3 und 4
  if (iIndex == 2 || iIndex == 3)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 910, 0, -1)->Set(0,1);
    CreateObject(BRDR, 5970, 0, -1)->Set(1,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),4370,0,1,1);

    //Spawnpoints entfernen
    RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(1969, 1139, 2, 2)));
    RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(3609,1279, 2, 2)));

    //Geschützstellung entfernen
    aStationary[3]->DecoExplode(30);
   }
  }

  //Ziel 5, 6 und 7
  if (iIndex == 4 || iIndex == 5 || iIndex == 6)
  {
   if(fConnectedDestroyed)
   {
    //Geschützstellung entfernen
    aStationary[4]->DecoExplode(30);
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 5025; iY = 760;
   return(1);
  }

  //HTF/MR/CTF/LMS/DM-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GCTF) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[3150, 1270], [3315, 1210], [3380, 1270], [3420, 1120], [3550, 1270]];
   }
   if(iTeam == 2)
   {
    return [[4720, 1230], [4810, 1200], [4820, 1090], [4890, 1160]];
   }
   return(1);
  }
}