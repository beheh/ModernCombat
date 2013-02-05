/*-- Base Jumping --*/

#strict
#include CSTD

static aFlag;

public func SpecificEquipment()	{return [[PPAR, 1]];}	//Zusatzausr�stung: Fallschirmrucksack
func RecommendedGoals()		{return [GOCC,GMNR];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxit�t
  SetSkyParallax(1, 40, 40, 0, 0, SkyPar_Keep, SkyPar_Keep);
  //Flaggen
  aFlag = [];
  //Einrichtung plazieren
  CreateInterior();
  //Ausr�stung plazieren
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
  CreateObject(LADR, 410, 938, -1)->Set(10);
  CreateObject(LADR, 490, 938, -1)->Set(10);
  CreateObject(LADR, 520, 1706, -1)->Set(26);
  CreateObject(LADR, 700, 1570, -1)->Set(9);
  CreateObject(LADR, 740, 434, -1)->Set(22);
  CreateObject(LADR, 890, 1706, -1)->Set(16);
  CreateObject(LADR, 1365, 1099, -1)->Set(19);
  CreateObject(LADR, 1380, 926, -1)->Set(11);
  CreateObject(LADR, 1185, 1697, -1)->Set(15);
  CreateObject(LADR, 1425, 1697, -1)->Set(15);
  CreateObject(LADR, 1455, 1519, -1)->Set(60);
  CreateObject(LADR, 1625, 1519, -1)->Set(60);
  CreateObject(LADR, 1655, 1697, -1)->Set(15);
  CreateObject(LADR, 1700, 926, -1)->Set(11);
  CreateObject(LADR, 1715, 1099, -1)->Set(19);
  CreateObject(LADR, 1895, 1697, -1)->Set(15);
  CreateObject(LADR, 2190, 1706, -1)->Set(16);
  CreateObject(LADR, 2340, 434, -1)->Set(22);
  CreateObject(LADR, 2380, 1570, -1)->Set(9);
  CreateObject(LADR, 2560, 1706, -1)->Set(26);
  CreateObject(LADR, 2590, 938, -1)->Set(10);
  CreateObject(LADR, 2670, 938, -1)->Set(10);

  //Bodenluken
  CreateObject(HA4K, 410, 853, -1);
  CreateObject(HA4K, 490, 853, -1);
  CreateObject(HA4K, 520, 1493, -1);
  CreateObject(HA4K, 700, 1493, -1);
  CreateObject(HA4K, 740, 253, -1);
  CreateObject(HA4K, 890, 1573, -1);
  CreateObject(HA4K, 1380, 833, -1);

  CreateObject(HA4K, 1700, 833, -1);
  CreateObject(HA4K, 2190, 1573, -1);
  CreateObject(HA4K, 2340, 253, -1);
  CreateObject(HA4K, 520, 1493, -1);
  CreateObject(HA4K, 2380, 1493, -1);
  CreateObject(HA4K, 2560, 1493, -1);
  CreateObject(HA4K, 2590, 853, -1);
  CreateObject(HA4K, 2670, 853, -1);

  //Rampen
  DrawMaterialQuad("Wall-Concrete3",1480,930,1480,920,1450,930,1480,930,true);
  DrawMaterialQuad("Wall-Concrete3",1450,940,1450,930,1420,940,1450,940,true);
  DrawMaterialQuad("Wall-Concrete3",1600,930,1600,920,1630,930,1600,930,true);
  DrawMaterialQuad("Wall-Concrete3",1630,940,1630,930,1660,940,1630,940,true);

  //Kisten
  CreateObject(WCR2, 570, 940, -1)->AutoRespawn();
  CreateObject(WCR2, 920, 1570, -1)->AutoRespawn();
  CreateObject(WCR2, 2160, 1570, -1)->AutoRespawn();
  CreateObject(WCR2, 2510, 940, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 440, 940, -1)->AutoRespawn();
  CreateObject(MWCR, 640, 250, -1)->AutoRespawn();
  CreateObject(MWCR, 850, 440, -1)->AutoRespawn();
  CreateObject(MWCR, 2230, 440, -1)->AutoRespawn();
  CreateObject(MWCR, 2440, 250, -1)->AutoRespawn();
  CreateObject(MWCR, 2640, 940, -1)->AutoRespawn();

  //Verbandskisten
  CreateObject(BECR, 460, 940, -1)->AutoRespawn();
  CreateObject(BECR, 1095, 1570, -1)->AutoRespawn();
  CreateObject(BECR, 1470, 1710, -1);
  CreateObject(BECR, 1540, 920, -1)->AutoRespawn();
  CreateObject(BECR, 1610, 1710, -1);
  CreateObject(BECR, 1985, 1570, -1)->AutoRespawn();
  CreateObject(BECR, 2620, 940, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 355, 1490, -1)->AutoRespawn();
  CreateObject(GSBL, 790, 1570, -1);
  CreateObject(GSBL, 1200, 920, -1);
  CreateObject(GSBL, 1880, 920, -1);
  CreateObject(GSBL, 2290, 1570, -1);
  CreateObject(GSBL, 2725, 1490, -1)->AutoRespawn();

  //Container
  CreateObject(CON1, 665, 1490, -1)->SetPerspective(2);
  CreateObject(CON1, 2415, 1490, -1)->SetPerspective(2);

  //Schutzt�ren
  CreateObject(GDDR, 595, 1570, -1);
  CreateObject(GDDR, 755, 1570, -1);
  CreateObject(GDDR, 2325, 1570, -1);
  CreateObject(GDDR, 2485, 1570, -1);

  //Stahlbr�cken
  CreateObject(_HBR, 314, 1502, -1);
  CreateObject(_HBR, 515, 1722, -1);
  CreateObject(_HBR, 885, 1722, -1);
  CreateObject(_HBR, 1015, 1582, -1)->SwitchMode();
  CreateObject(_HBR, 1297, 342, -1);
  CreateObject(_HBR, 1335, 952, -1);
  CreateObject(_HBR, 1435, 1722, -1);
  CreateObject(_HBR, 1505, 1722, -1);
  CreateObject(_HBR, 1575, 1722, -1);
  CreateObject(_HBR, 1645, 1722, -1);
  CreateObject(_HBR, 1745, 952, -1);
  CreateObject(_HBR, 1784, 342, -1);
  CreateObject(_HBR, 2065, 1582, -1)->SwitchMode();
  CreateObject(_HBR, 2190, 1722, -1);
  CreateObject(_HBR, 2560, 1722, -1);
  CreateObject(_HBR, 2767, 1502, -1);

  //Steine
  CreateObject(STNE, 260, 1850, -1);
  CreateObject(STNE, 1200, 1810, -1);
  CreateObject(STNE, 2000, 1850, -1);
  CreateObject(STNE, 2490, 1800, -1);

  //Explosive Kisten
  CreateObject(XWCR, 160, 1360, -1)->AutoRespawn();
  CreateObject(XWCR, 915, 1420, -1)->AutoRespawn();
  CreateObject(XWCR, 2165, 1420, -1)->AutoRespawn();
  CreateObject(XWCR, 2910, 1360, -1)->AutoRespawn();

  //Explosivf�sser
  CreateObject(XBRL, 390, 850, -1)->AutoRespawn();
  CreateObject(XBRL, 1444, 930, -1);
  CreateObject(XBRL, 1636, 930, -1);
  CreateObject(XBRL, 2690, 850, -1)->AutoRespawn();

  //Benzinf�sser
  CreateObject(PBRL, 250, 1370, -1)->AutoRespawn();
  CreateObject(PBRL, 1120, 1570, -1)->AutoRespawn();
  CreateObject(PBRL, 1960, 1570, -1)->AutoRespawn();
  CreateObject(PBRL, 2840, 1370, -1)->AutoRespawn();

  //Phosphorf�sser
  CreateObject(HBRL, 230, 1370, -1)->AutoRespawn();
  CreateObject(HBRL, 280, 1370, -1)->AutoRespawn();
  CreateObject(HBRL, 2810, 1370, -1)->AutoRespawn();
  CreateObject(HBRL, 2860, 1370, -1)->AutoRespawn();

  //Ger�ste
  CreateObject(SFFG, 890, 1470, -1)->Set(4);
  CreateObject(SFFG, 890, 1520, -1)->Set(3);
  CreateObject(SFFG, 890, 1570, -1)->Set(5);
  CreateObject(SFFG, 2190, 1470, -1)->Set(4);
  CreateObject(SFFG, 2190, 1520, -1)->Set(2);
  CreateObject(SFFG, 2190, 1570, -1)->Set(5);

  //Sandsackbarrieren
  CreateObject(SBBA, 770, 1490, -1)->Right();
  CreateObject(SBBA, 1450, 800, -1);
  CreateObject(SBBA, 1490, 920, -1);
  CreateObject(SBBA, 1501, 1540, -1);
  CreateObject(SBBA, 1580, 1540, -1)->Right();
  CreateObject(SBBA, 1590, 920, -1)->Right();
  CreateObject(SBBA, 1630, 800, -1)->Right();
  CreateObject(SBBA, 2311, 1490, -1);

  //Explosivtanks
  CreateObject(XTNK, 690, 250, -1)->AutoRespawn();
  CreateObject(XTNK, 1330, 1110, -1)->AutoRespawn();
  CreateObject(XTNK, 1750, 1110, -1)->AutoRespawn();
  CreateObject(XTNK, 2390, 250, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 810, 1570, -1)->AutoRespawn();
  CreateObject(PTNK, 1540, 1540, -1)->AutoRespawn();
  CreateObject(PTNK, 2270, 1570, -1)->AutoRespawn();

  //Verbundene R�ume
  var doorw = CreateObject(GAT1, 770, 420, -1);
  CreateObject(ROOM, 665, 1570, -1)->Connect(doorw);
  var doorw = CreateObject(GAT1, 2310, 420, -1);
  CreateObject(ROOM, 2415, 1570, -1)->Connect(doorw);

  //Sonne umplazieren
  FindObject(LENS)->SetPosition(1570,390);

  //Sendemast
  var tower = CreateObject(AATR, 1540, 780, -1);
  tower->SwitchMode();
  tower->AddNode(932, 400, 0, CreateObject(REHR, 925, 410, -1), 0, 1);
  tower->AddNode(622, 925, 3, CreateObject(REHR, 615, 950, -1));
  tower->AddNode(1300, 825, 1, CreateObject(REHR, 1295, 830, -1), -45, 2);
  tower->AddNode(1785, 825, 1, CreateObject(REHR, 1785, 830, -1), 45, 2);
  tower->AddNode(2460, 925, 3, CreateObject(REHR, 2465, 950, -1));
  tower->AddNode(2148, 400, 0, CreateObject(REHR, 2155, 410, -1), 0, 1);

  //Sounds

  //Lava
  CreateObject(SE4K, 460, 1710, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 950, 1710, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 1540, 1710, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 2130, 1710, -1)->Set("Lava*",1200,350);
  CreateObject(SE4K, 2620, 1710, -1)->Set("Lava*",1200,350);

  //Wind
  CreateObject(SE4K, 60, 300, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 290, 650, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 1540, 330, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 1540, 1230, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 2790, 650, -1)->Set("WindSound*.ogg",245,105);
  CreateObject(SE4K, 3000, 100, -1)->Set("WindSound*.ogg",245,105);

  //Hallen
  CreateObject(SE4K, 665, 1525, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1540, 890, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2415, 1525, -1)->Set("Interior*.ogg",665,105);

  //Erdrutsche
  CreateObject(SE4K, 460, 1180, -1)->Set("FallingDirt*.ogg",850,250);
  CreateObject(SE4K, 610, 440, -1)->Set("FallingDirt*.ogg",850,250);
  CreateObject(SE4K, 2470, 440, -1)->Set("FallingDirt*.ogg",850,250);
  CreateObject(SE4K, 2620, 1180, -1)->Set("FallingDirt*.ogg",850,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 735, 1570, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 1400, 940, -1);
  crate->Set(ABOX);
  crate = CreateObject (AMCT, 2345, 1570, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject (AMCT, 1470, 1570, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 1680, 940, -1);
  crate->Set(GBOX);

  //Versorgungskisten (Raketen)
  crate = CreateObject (AMCT, 600, 250, -1);
  crate->Set(MBOX);
  crate = CreateObject (AMCT, 1610, 1570, -1);
  crate->Set(MBOX);
  crate = CreateObject (AMCT, 2480, 250, -1);
  crate->Set(MBOX);

  //Versorgungskiste (APW)
  crate = CreateObject (AMCT, 810, 440, -1);
  crate->Set(ATWN);
  crate = CreateObject (AMCT, 2270, 440, -1);
  crate->Set(ATWN);

  //Versorgungskiste (Dragnin)
  crate = CreateObject (AMCT, 1540, 1540, -1);
  crate->Set(DGNN);

  //Gesch�tzstellungen
  CreateObject(GNET, 1000, 440, -1)->Set(SATW,0,1);
  CreateObject(GNET, 1300, 1110, -1)->Set(SATW,-90,1);
  CreateObject(GNET, 1780, 1110, -1)->Set(SATW,90,1);
  CreateObject(GNET, 2080, 440, -1)->Set(SATW,0,1);

  //Artilleriebatterien
  CreateObject(ATBY,320,850,-1)->SetRotation(60);
  CreateObject(ATBY,798,360,-1)->SetRotation(60);
  CreateObject(ATBY,950,1570,-1)->SetRotation(50);
  CreateObject(ATBY,2130,1570,-1)->SetRotation(-50);
  CreateObject(ATBY,2285,360,-1)->SetRotation(-60);
  CreateObject(ATBY,2760,850,-1)->SetRotation(-60);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Denkm�ler
  CreateObject(MUMT, 1210, 330, -1);
  CreateObject(MUMT, 1870, 330, -1);

  //Hinweisschilder
  CreateObject(WLSN, 665, 1540, -1)->SetPhase(3);
  CreateObject(WLSN, 1110, 1490, -1)->SetPhase(3);
  CreateObject(WLSN, 1110, 1520, -1)->SetPhase(2);
  CreateObject(WLSN, 1970, 1490, -1)->SetPhase(4);
  CreateObject(WLSN, 1970, 1520, -1)->SetPhase(2);
  CreateObject(WLSN, 2415, 1540, -1)->SetPhase(4);

  //Verbrannte Zauberwerkstatt
  CreateObject(BMWS, 1300, 835, -1);

  //Radare
  CreateObject(RADR, 1105, 1300, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 740, 1490, -1);
  CreateObject(RADR, 1965, 1300, -1)->SetClrModulation(RGB(125,125,125));
  CreateObject(RADR, 2340, 1490, -1);

  //Satellitensch�sseln
  CreateObject(SADH,770, 250, -1);
  CreateObject(SADH,2330, 250, -1);

  //Orientierungslichter
  CreateObject(OLGH, 440, 1490, -1)->Set(6, 15, 1, 1, 40);
  CreateObject(OLGH, 685, 250, -1)->Set(6, 15, 1, 1, 40);
  CreateObject(OLGH, 2395, 250, -1)->Set(6, 15, 1, 1, 40);
  CreateObject(OLGH, 2640, 1490, -1)->Set(6, 15, 1, 1, 40);

  //Radios
  CreateObject(RDIO, 680, 440, -1);
  CreateObject(RDIO, 2400, 440, -1);

  //Leitkegel
  CreateObject(TCCN, 1295, 940, -1);
  CreateObject(TCCN, 1375, 940, -1);
  CreateObject(TCCN, 1705, 940, -1);
  CreateObject(TCCN, 1785, 940, -1);

  //Risse
  CreateObject(FAUD, 560, 1350, -1);
  CreateObject(FAUD, 590, 1370, -1);
  CreateObject(FAUD, 1090, 720, -1);
  CreateObject(FAUD, 1100, 650, -1);
  CreateObject(FAUD, 1100, 690, -1);
  CreateObject(FAUD, 1720, 1420, -1);
  CreateObject(FAUD, 1710, 1410, -1);
  CreateObject(FAUD, 2220, 870, -1);
  CreateObject(FAUD, 2240, 850, -1);

  //Ventillatoren
  CreateObject(VENT, 740, 1350, -1)->SetCon(30);
  CreateObject(VENT, 2340, 1350, -1)->SetCon(30);

  //L�ftungsgitter
  CreateObject(ENGT, 450, 940, -1);
  CreateObject(ENGT, 2630, 940, -1);

  //Bildschirme
  CreateObject(SCA2, 460, 1425, -1)->SetAction("Scan2");
  CreateObject(SCA1, 1465, 915, -1)->SetAction("Grenade");
  CreateObject(SCA1, 1615, 915, -1)->SetAction("Medic");
  CreateObject(SCA2, 2620, 1425, -1)->SetAction("Scan");

  //Stra�enlichter
  CreateObject(SLGH, 400, 1490, -1)->TurnOff();
  CreateObject(SLGH, 575, 1490, -1)->TurnOff();
  CreateObject(SLGH, 1170, 940, -1)->TurnOff();
  CreateObject(SLGH, 1910, 940, -1)->TurnOff();
  CreateObject(SLGH, 2505, 1490, -1)->TurnOff();
  CreateObject(SLGH, 2680, 1490, -1)->TurnOff();

  //Notausgangslichter
  CreateObject(ETLT, 730, 1555, -1);
  CreateObject(ETLT, 2350, 1555, -1);

  //Automaten
  CreateObject(VGMN, 1270, 940, -1)->SetPhase(10);
  CreateObject(VGMN, 1810, 940, -1)->Set(3);

  //Regale
  CreateObject(FRAM, 392, 910, -1);
  CreateObject(FRAM, 392, 925, -1);
  CreateObject(FRAM, 392, 940, -1);
  CreateObject(FRAM, 2688, 910, -1);
  CreateObject(FRAM, 2688, 925, -1);
  CreateObject(FRAM, 2688, 940, -1);

  //Flaschen
  CreateObject(BOTL, 570, 922, -1);
  CreateObject(BOTL, 660, 1462, -1);
  CreateObject(BOTL, 2420, 1462, -1);
  CreateObject(BOTL, 2510, 922, -1);

  //Monitore
  CreateObject(MNI2, 440, 922, -1);
  CreateObject(MNI2, 1095, 1552, -1);
  CreateObject(MNI2, 1540, 902, -1);
  CreateObject(MNI2, 1989, 1552, -1);
  CreateObject(MNI2, 2640, 922, -1);

  //Z�une
  CreateObject(FENC, 410, 850, -1)->Set(2);
  CreateObject(FENC, 470, 850, -1);
  CreateObject(FENC, 850, 440, -1);
  CreateObject(FENC, 2230, 440, -1);
  CreateObject(FENC, 2610, 850, -1);
  CreateObject(FENC, 2670, 850, -1)->Set(2);

  //Gel�nder
  CreateObject(RAI1, 349, 1490, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 579, 250, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 711, 250, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1068, 470, -1);
  CreateObject(RAI1, 1181, 330, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1289, 1110, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1429, 1570, -1);
  CreateObject(RAI1, 1626, 1570, -1);
  CreateObject(RAI1, 1709, 1110, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1819, 330, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 1988, 470, -1);
  CreateObject(RAI1, 2289, 250, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 2421, 250, -1)->SetRail([1,1,1,1]);
  CreateObject(RAI1, 2470, 1490, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1]);

  //B�sche
  CreateObject(BSH2, 590, 950, -1);
  CreateObject(BSH2, 700, 300, -1);
  CreateObject(BSH2, 1260, 880, -1);
  CreateObject(BSH2, 1515, 790, -1);
  CreateObject(BSH2, 1570, 790, -1);
  CreateObject(BSH2, 1820, 880, -1);
  CreateObject(BSH2, 2380, 300, -1);
  CreateObject(BSH2, 2490, 950, -1);
}

func CreateOptionalFeatures()
{
  //Hintergr�nde
  CreateObject(BD03,1200,700,-1);
  CreateObject(BD03,1500,1000,-1);
  CreateObject(BD07,-100,1720,-1);
  CreateObject(BD07,700,1720,-1);
  CreateObject(BD07,1000,1720,-1);
  CreateObject(BD07,2000,1720,-1);
}

/* Bei Relaunch */
 
public func OnClassSelection(object pClonk)
{
  //LMS/MR-Spielziel
  if (FindObject(GLMS) || FindObject(GMNR))
  {
   AddEffect("IntPara", pClonk, 1, 1);
   Sound("Airstrike2", 0, pClonk);
  }
}
 
global func FxIntParaTimer(object pTarget)
{
  CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
  return -1;
}

/* Regelw�hler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Offensive.ogg");

  //Verz�gerte Hinweisnachricht ausgeben
  if(!FindObject(FDMG))
    Schedule("EventInfo4K(0,Format(\"$MsgParachute$\"),PPAR, 0, 0, 0, \"PriorityInfo.ogg\");", 100);

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Helikopter und Hinweisschilder
  if(!FindObject(NOHC))
  {
   SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,440,1460,-1),100*21);
   SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,2640,1460,-1),100*21);

   CreateObject(SNPT, 575, 1490, -1)->SetAction("Sign3");
   CreateObject(SNPT, 2505, 1490, -1)->SetAction("Sign3");
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,450,850,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(390, 930);
   aFlag[0] -> AddSpawnPoint(540, 930);
   aFlag[0] -> AddSpawnPoint(610, 940);
   aFlag[0]->Set("$Flag1$",0,2);

   aFlag[1] = CreateObject(OFPL,840,1570,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(740, 1480);
   aFlag[1] -> AddSpawnPoint(575, 1480);
   aFlag[1] -> AddSpawnPoint(615, 1560);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,1210,330,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(720,280);
   aFlag[2] -> AddSpawnPoint(760,240);
   aFlag[2] -> AddSpawnPoint(990,430);
   if(aTeams[1] == true)
   {
    aFlag[2]->Set("$Flag3$",100,2);
    aFlag[2]->Capture(1,1);
   }
   else
   {
    aFlag[2]->Set("$Flag3$",0,2);
   }

   aFlag[3] = CreateObject(OFPL,1540,780,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(1290, 930);
   aFlag[3] -> AddSpawnPoint(1510, 910);
   aFlag[3] -> AddSpawnPoint(1570, 910);
   aFlag[3] -> AddSpawnPoint(1790, 930);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,1870,330,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(2090,430);
   aFlag[4] -> AddSpawnPoint(2360,280);
   aFlag[4] -> AddSpawnPoint(2320,240);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   aFlag[5] = CreateObject(OFPL,2240,1570,NO_OWNER);
   aFlag[5] -> AddSpawnPoint(2340, 1480);
   aFlag[5] -> AddSpawnPoint(2505, 1480);
   aFlag[5] -> AddSpawnPoint(2465, 1560);
   aFlag[5]->Set("$Flag6$",0,2);

   aFlag[6] = CreateObject(OFPL,2630,850,NO_OWNER);
   aFlag[6] -> AddSpawnPoint(2470, 940);
   aFlag[6] -> AddSpawnPoint(2540, 930);
   aFlag[6] -> AddSpawnPoint(2690, 930);
   aFlag[6]->Set("$Flag7$",0,2);
  }

  //HTF-Spielziel
  if (FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 1540,780, -1);
   flag->~Set("$Flag4$");

   //Zus�tzliche Munition
   if(!FindObject(NOAM))
   {
    //Raketen
    PlaceSpawnpoint(MBOX, 1570, 915);
   }
  }

  //MR-Spielziel
  if (FindObject(GMNR))
  {
   //Gelds�cke
   AddMoneySpawn(475, 330, [15]);
   AddMoneySpawn(770, 1560, [20]);
   AddMoneySpawn(1430, 400, [15]);
   AddMoneySpawn(1540, 770, [15]);
   AddMoneySpawn(1640, 400, [15]);
   AddMoneySpawn(2310, 1560, [20]);
   AddMoneySpawn(2605, 330, [15]);
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX =1540; iY = 490;
   return(1);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   if(iTeam == 1)
   {
    return [[600, 1480], [740, 1480], [860, 1560]];
   }
   if(iTeam == 2)
   {
    return [[2480, 1480], [2340, 1480], [2220, 1560]];
   }
   return(1);
  }

  //MR/LMS-Spielziel
  if(FindObject(GMNR) || FindObject(GLMS))
  {
   if(iTeam == 1)
   {
    return [[685, 0], [920, 0], [1205, 0]];
   }
   if(iTeam == 2)
   {
    return [[2395, 0], [2150, 0], [1865, 0]];
   }
   return(1);
  }
}