/*-- Twin Peaks --*/

#strict 2
#include CSTD

static aFlag,aSelfDefense,aTowerInterior,doorw1,doorw2,pRoom1,pRoom2;

public func SpecificEquipment()	{return [[PPAR, 1]];}	//Zusatzausrüstung: Fallschirmrucksack
func RecommendedGoals()		{return [GOCC,GHTF];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Flaggen
  aFlag = [];
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
  return 1;
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Rampen
  DrawMaterialQuad("Wall-Stripes",1190,730,1190,700,1160,730,1190,730,true);
  DrawMaterialQuad("Wall-Stripes",1171,1210,1171,1240,1201,1210,1171,1210,true);
  DrawMaterialQuad("Wall-Stripes",1241,730,1241,700,1271,730,1241,730,true);
  DrawMaterialQuad("Wall-Stripes",1289,1370,1289,1360,1259,1370,1289,1370,true);
  DrawMaterialQuad("Wall-Concrete3",2021,840,2021,820,1981,840,2021,840,true);
  DrawMaterialQuad("Wall-Concrete3",2531,1490,2531,1480,2551,1490,2531,1490,true);
  DrawMaterialQuad("Wall-Concrete3",3201,1490,3201,1480,3181,1490,3201,1490,true);
  DrawMaterialQuad("Wall-Concrete3",3711,840,3711,820,3751,840,3711,840,true);
  DrawMaterialQuad("Wall-Stripes",4441,1370,4441,1360,4471,1370,4441,1370,true);
  DrawMaterialQuad("Wall-Stripes",4490,730,4490,700,4460,730,4490,730,true);
  DrawMaterialQuad("Wall-Stripes",4570,1210,4570,1240,4540,1210,4570,1210,true);
  DrawMaterialQuad("Wall-Stripes",4541,730,4541,700,4571,730,4541,730,true);

  //Sprungschanzen
  CreateObject(JMPD, 2565, 1490, -1)->Set(200, -10);
  CreateObject(JMPD, 3165, 1490, -1)->Set(200, 10);

  //Glasscheiben
  CreateObject(PANE, 1802, 884, -1);
  CreateObject(PANE, 1802, 913, -1);
  CreateObject(PANE, 1802, 942, -1);
  CreateObject(PANE, 3928, 884, -1);
  CreateObject(PANE, 3928, 913, -1);
  CreateObject(PANE, 3928, 942, -1);

  //Leitern
  CreateObject(LADR, 1215, 971, -1)->Set(33);
  CreateObject(LADR, 1325, 1354, -1)->Set(57);
  CreateObject(LADR, 1485, 1354, -1)->Set(57);
  CreateObject(LADR, 1615, 971, -1)->Set(23);
  CreateObject(LADR, 1725, 745, -1)->Set(5);
  CreateObject(LADR, 1775, 990, -1)->Set(27);

  CreateObject(LADR, 3955, 990, -1)->Set(27);
  CreateObject(LADR, 4005, 745, -1)->Set(5);
  CreateObject(LADR, 4115, 971, -1)->Set(23);
  CreateObject(LADR, 4245, 1354, -1)->Set(57);
  CreateObject(LADR, 4405, 1354, -1)->Set(57);
  CreateObject(LADR, 4515, 971, -1)->Set(33);

  //Große Bodenluken
  CreateObject(H24K, 1215, 708, -1);
  CreateObject(H24K, 1215, 788, -1);
  CreateObject(H24K, 1215, 898, -1);
  CreateObject(H24K, 1325, 898, -1);
  CreateObject(H24K, 1485, 898, -1);
  CreateObject(H24K, 1615, 788, -1);
  CreateObject(H24K, 1615, 898, -1);
  CreateObject(H24K, 4115, 788, -1);
  CreateObject(H24K, 4115, 898, -1);
  CreateObject(H24K, 4245, 898, -1);
  CreateObject(H24K, 4405, 898, -1);
  CreateObject(H24K, 4515, 708, -1);
  CreateObject(H24K, 4515, 788, -1);
  CreateObject(H24K, 4515, 898, -1);

  //Gerüst
  CreateObject(SFFG, 2865, 1310, -1)->Set(5);

  //Schutztüren
  CreateObject(GDDR, 1165, 780, -1);
  CreateObject(GDDR, 1265, 780, -1);
  CreateObject(GDDR, 4465, 780, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 4565, 780, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Sandsackbarrieren
  CreateObject(SBBA, 1390, 780, -1);
  CreateObject(SBBA, 1420, 780, -1)->Right();
  CreateObject(SBBA, 1810, 700, -1)->Right();
  CreateObject(SBBA, 2495, 1480, -1)->Right();
  CreateObject(SBBA, 3235, 1480, -1);
  CreateObject(SBBA, 3921, 700, -1);
  CreateObject(SBBA, 4310, 780, -1);
  CreateObject(SBBA, 4340, 780, -1)->Right();

  //Metallkisten
  CreateObject(MWCR, 1695, 980, -1)->AutoRespawn();
  CreateObject(MWCR, 4035, 980, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 1110, 890, -1)->AutoRespawn();
  CreateObject(HBRL, 4620, 890, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1510, 890, -1)->AutoRespawn();
  CreateObject(GSBL, 4220, 890, -1)->AutoRespawn();

  //Stahlbrücken
  CreateObject(_HBR, 1345, 792, -1)->SwitchMode();
  CreateObject(_HBR, 1405, 902, -1)->SwitchMode();
  CreateObject(_HBR, 1465, 792, -1)->SwitchMode();
  CreateObject(_HBR, 1925, 852, -1)->SwitchMode();

  CreateObject(_HBR, 2314, 1492, -1);
  CreateObject(_HBR, 2405, 1492, -1)->SwitchMode();
  CreateObject(_HBR, 2645, 1442, -1)->SwitchMode();
  CreateObject(_HBR, 3085, 1442, -1)->SwitchMode();
  CreateObject(_HBR, 3325, 1492, -1)->SwitchMode();
  CreateObject(_HBR, 3417, 1492, -1);

  CreateObject(_HBR, 3805, 852, -1)->SwitchMode();
  CreateObject(_HBR, 4265, 792, -1)->SwitchMode();
  CreateObject(_HBR, 4325, 902, -1)->SwitchMode();
  CreateObject(_HBR, 4385, 792, -1)->SwitchMode();

  //Grenzen
  CreateObject(BRDR, 400, 0, -1)->Set(0);
  CreateObject(BRDR, 5330, 0, -1)->Set(1);
  CreateObject(BRDR, 0, 1650, -1)->Set(3,0,1);

  //Selbstschussanlagen
  aSelfDefense[0] = CreateObject(SEGU, 1174, 715, -1);
  aSelfDefense[0]->Set(0,1,1,-225);
  CreateObject(CONS, 1170, 885, -1)->Set(aSelfDefense[0]);

  aSelfDefense[1] = CreateObject(SEGU, 1192, 1241, -1);
  aSelfDefense[1]->Set(0,0,1,-45);
  CreateObject(CONS, 1235, 975, -1)->Set(aSelfDefense[1]);

  aSelfDefense[2] = CreateObject(SEGU, 1257, 715, -1);
  aSelfDefense[2]->Set(0,1,1,-135);
  CreateObject(CONS, 1190, 885, -1)->Set(aSelfDefense[2]);

  aSelfDefense[3] = CreateObject(SEGU, 1828, 770, -1);
  aSelfDefense[3]->Set(0,0,1,-90);
  CreateObject(CONS, 1640, 885, -1)->Set(aSelfDefense[3]);

  aSelfDefense[4] = CreateObject(SEGU, 3902, 770, -1);
  aSelfDefense[4]->Set(0,0,1,90);
  CreateObject(CONS, 4090, 885, -1)->Set(aSelfDefense[4]);

  aSelfDefense[5] = CreateObject(SEGU, 4474, 715, -1);
  aSelfDefense[5]->Set(0,1,1,-225);
  CreateObject(CONS, 4540, 885, -1)->Set(aSelfDefense[5]);

  aSelfDefense[6] = CreateObject(SEGU, 4557, 715, -1);
  aSelfDefense[6]->Set(0,1,1,-135);
  CreateObject(CONS, 4560, 885, -1)->Set(aSelfDefense[6]);

  aSelfDefense[7] = CreateObject(SEGU, 4550, 1241, -1);
  aSelfDefense[7]->Set(0,0,1,45);
  CreateObject(CONS, 4490, 975, -1)->Set(aSelfDefense[7]);

  //Sendemast
  var tower = CreateObject(AATR, 2865, 1310, -1);
  tower->AddNode(1760, 1060, 1, CreateObject(REHR, 1765, 1070, -1),90,2);
  tower->AddNode(2160, 845, 0, CreateObject(REHR, 2165, 855, -1),90,2);
  tower->AddNode(3570, 840, 0, CreateObject(REHR, 3565, 855, -1),-90,2);
  tower->AddNode(3970, 1055, 1, CreateObject(REHR, 3965, 1070, -1),-90,2);

  //Verbundene Räume
  var doorw = CreateObject(GAT1, 1260, 960, -1);
  CreateObject(ROOM, 1245, 1370, -1)->Connect(doorw);
  doorw1 = CreateObject(GAT1, 2865, 900, -1);
  pRoom1 = CreateObject(ROOM, 2865, 1470, -1);
  pRoom1->Connect(doorw1);
  doorw2 = CreateObject(GAT1, 2865, 700, -1);
  pRoom2 = CreateObject(ROOM, 2865, 933, -1);
  pRoom2->Connect(doorw2);
  var doorw = CreateObject(GAT1, 4470, 960, -1);
  CreateObject(ROOM, 4485, 1370, -1)->Connect(doorw);

  //Sounds

  //Wind
  CreateObject(SE4K, 500, 400, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2865, 400, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 5200, 400, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 1405, 900, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 4325, 900, -1)->Set("Interior*.ogg",670,105);

  //Eulen
  CreateObject(SE4K, 1050, 800, -1)->Set("Owl.wav",1400,35);
  CreateObject(SE4K, 2865, 1210, -1)->Set("Owl.wav",1400,35);
  CreateObject(SE4K, 4680, 800, -1)->Set("Owl.wav",1400,35);

  //Erdrutsche
  CreateObject(SE4K, 1330, 1600, -1)->Set("FallingDirt*.ogg",950,250);
  CreateObject(SE4K, 2865, 1600, -1)->Set("FallingDirt*.ogg",850,250);
  CreateObject(SE4K, 4400, 1600, -1)->Set("FallingDirt*.ogg",850,250);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 1240, 890, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 4490, 890, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject(AMCT, 1590, 890, -1);
  crate->Set(GBOX);
  crate = CreateObject(AMCT, 4140, 890, -1);
  crate->Set(GBOX);

  //Versorgungskisten (Dragnin)
  crate = CreateObject(AMCT, 1565, 890, -1);
  crate->Set(DGNN);
  crate = CreateObject(AMCT, 4165, 890, -1);
  crate->Set(DGNN);

  //Versorgungskisten (APW)
  crate = CreateObject(AMCT, 1300, 780, -1);
  crate->Set(ATWN);
  crate = CreateObject(AMCT, 4430, 780, -1);
  crate->Set(ATWN);

  //Raketen
  PlaceSpawnpoint(MBOX, 1355, 885);
  PlaceSpawnpoint(MBOX, 4375, 885);

  //Automaten
  var store = CreateObject(WPVM,1560, 980,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(BWTH,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(ATWN,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  store->AddWare(SRBL,-1);
  store = CreateObject(WPVM,4170, 980,-1);
  store->AddWare(C4PA,-1);
  store->AddWare(FAPK,-1);
  store->AddWare(CDBT,-1);
  store->AddWare(BWTH,-1);
  store->AddWare(RSHL,-1);
  store->AddWare(ATWN,-1);
  store->AddWare(FGRN,-1);
  store->AddWare(FRAG,-1);
  store->AddWare(PGRN,-1);
  store->AddWare(SGRN,-1);
  store->AddWare(STUN,-1);
  store->AddWare(SRBL,-1);

  //Geschützstellungen
  CreateObject(GNET, 1065, 890, -1)->Set(0,-90);
  CreateObject(GNET, 1725, 980, -1)->Set(SATW,90);
  CreateObject(GNET, 1775, 700, -1)->Set(LCAC);
  CreateObject(GNET, 2165, 890, -1)->Set(0,90);
  CreateObject(GNET, 2830, 1350, -1)->Set(0,-90);
  aTowerInterior[0] = CreateObject(GNET, 2835, 933, -1);
  aTowerInterior[0] -> Set(SATW,-90,1);
  aTowerInterior[1] = CreateObject(GNET, 2895, 933, -1);
  aTowerInterior[1] -> Set(SATW,90,1);
  CreateObject(GNET, 2900, 1350, -1)->Set(0,90);
  CreateObject(GNET, 3565, 890, -1)->Set(0,-90);
  CreateObject(GNET, 3955, 700, -1)->Set(LCAC);
  CreateObject(GNET, 4005, 980, -1)->Set(SATW,-90);
  CreateObject(GNET, 4665, 890, -1)->Set(0,90);

  //MAV-Stationen
  CreateObject(MVSN, 1680, 890, -1)->Set(1680,780,1);
  CreateObject(MVSN, 4045, 890, -1)->Set(4045,780,1);

  //Artilleriebatterie
  aTowerInterior[2] = CreateObject(ATBY,2865,751,-1);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Leitkegel
  CreateObject(TCCN, 2370, 1480, -1)->Light();
  CreateObject(TCCN, 2400, 1480, -1);
  CreateObject(TCCN, 2430, 1480, -1)->Light();
  CreateObject(TCCN, 2460, 1480, -1);
  CreateObject(TCCN, 2835, 1310, -1);
  CreateObject(TCCN, 2895, 1310, -1);
  CreateObject(TCCN, 3270, 1480, -1);
  CreateObject(TCCN, 3300, 1480, -1)->Light();
  CreateObject(TCCN, 3330, 1480, -1);
  CreateObject(TCCN, 3360, 1480, -1)->Light();

  //Geländer
  CreateObject(RAI3, 1195, 700, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1755, 990, -1)->SetRail([1,1]);
  CreateObject(RAI1, 1510, 780, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2018, 820, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2509, 1480, -1);
  CreateObject(RAI1, 2581, 1430, -1);
  CreateObject(RAI1, 3125, 1430, -1);
  CreateObject(RAI1, 3199, 1480, -1);
  CreateObject(RAI1, 3578, 820, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI1, 3980, 780, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 3935, 990, -1)->SetRail([1,1]);
  CreateObject(RAI3, 4495, 700, -1)->SetRail([1,1]);

  //Bildschirme
  CreateObject(SCA2, 1155, 873, -1)->SetAction("News");
  CreateObject(SCA2, 4575, 873, -1)->SetAction("News");

  //Verbrannte Burgen
  CreateObject(CSB2, 1465, 1270, -1)->SetR(-2);
  CreateObject(CSB2, 4240, 1270, -1)->SetR(2);

  //Zäune
  CreateObject(FENC, 1345, 890, -1);
  CreateObject(FENC, 1405, 890, -1);
  CreateObject(FENC, 1465, 890, -1);
  CreateObject(FENC, 4265, 890, -1);
  CreateObject(FENC, 4325, 890, -1);
  CreateObject(FENC, 4385, 890, -1);

  //Feuerlöscher
  CreateObject(FIE2, 1300, 885, -1);
  CreateObject(FIE2, 1590, 975, -1);
  CreateObject(FIE2, 4140, 975, -1);
  CreateObject(FIE2, 4430, 885, -1);

  //Schilder
  CreateObject(MSGN, 1295, 1360, -1);
  CreateObject(MSGN, 1495, 1360, -1);
  CreateObject(MSGN, 4235, 1360, -1);
  CreateObject(MSGN, 4435, 1360, -1);

  //Orientierungslichter
  CreateObject(OLGH, 1215, 580, -1)->Set(1, 12, 1, 1, 60);
  CreateObject(OLGH, 1400, 1360, -1)->Set(7, 14, 1, 1, 30);
  CreateObject(OLGH, 1615, 660, -1)->Set(1, 12, 1, 1, 60);
  CreateObject(OLGH, 2005, 710, -1)->Set(1, 12, 1, 1, 60);
  CreateObject(OLGH, 2075, 820, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 2565, 1490, -1)->Set(1, 13, 1, 1, 50);
  CreateObject(OLGH, 3165, 1490, -1)->Set(1, 13, 1, 1, 50);
  CreateObject(OLGH, 3650, 820, -1)->Set(4, 15, 1, 1, 30);
  CreateObject(OLGH, 3725, 710, -1)->Set(1, 12, 1, 1, 60);
  CreateObject(OLGH, 4115, 660, -1)->Set(1, 12, 1, 1, 60);
  CreateObject(OLGH, 4335, 1360, -1)->Set(7, 14, 1, 1, 30);
  CreateObject(OLGH, 4515, 580, -1)->Set(1, 12, 1, 1, 60);

  //Straßenlichter
  CreateObject(SLGH, 1300, 780, -1);
  CreateObject(SLGH, 1405, 780, -1);
  CreateObject(SLGH, 1510, 780, -1);
  CreateObject(SLGH, 1880, 840, -1);
  CreateObject(SLGH, 2580, 1430, -1);
  CreateObject(SLGH, 3150, 1430, -1);
  CreateObject(SLGH, 3850, 840, -1);
  CreateObject(SLGH, 4220, 780, -1);
  CreateObject(SLGH, 4325, 780, -1);
  CreateObject(SLGH, 4430, 780, -1);

  //Notausgangslichter
  CreateObject(ETLT, 1590, 850, -1);
  CreateObject(ETLT, 4140, 850, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD03,1250,580,-1);
  CreateObject(BD03,1250,1500,-1);
  CreateObject(BD03,1400,1550,-1);
  CreateObject(BD03,2000,1450,-1);
  CreateObject(BD03,2700,580,-1);
  CreateObject(BD03,3000,1600,-1);
  CreateObject(BD02,1750,1720,-1)->SetClrModulation(RGB(150,150,150));
  CreateObject(BD01,1400,1720,-1)->SetClrModulation(RGB(150,150,150));
}

/* Bei Turmzusammenfall */

func OnTowerCollapse()
{
  EventInfo4K(0,Format("$MsgCollapse$"),AATR, 0, 0, 0, "Info_Event.ogg");

  //Geschützstellungen entfernen
  if(aTowerInterior[0]) aTowerInterior[0]->DecoExplode(30);
  if(aTowerInterior[1]) aTowerInterior[1]->DecoExplode(30);
  if(aTowerInterior[2]) aTowerInterior[2]->DecoExplode(30);

  //Türverbindung entfernen
  doorw1->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  RemoveObject(doorw1, true);
  pRoom1->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  pRoom1->Lock();
  pRoom1->SetAction("Idle");
  pRoom1->SetClrModulation(RGBa(100,100,100,5));
  var pContent;
  while(pContent = Contents(0, pRoom1))
   pRoom1->Exit(pContent);
  doorw2->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  RemoveObject(doorw2, true);
  pRoom2->CastSmoke("Smoke3",12,15,0,5,150,250,RGBa(255,255,255,100),RGBa(255,255,255,100));
  RemoveObject(pRoom2, true);
}

/* Bei Flaggenübernahme */

func FlagCaptured(object pPoint, int iTeam)
{
  if(pPoint == aFlag[0])
  {
   aSelfDefense[0]->SetTeam(iTeam);
   aSelfDefense[2]->SetTeam(iTeam);
  }

  if(pPoint == aFlag[1])
  {
   aSelfDefense[1]->SetTeam(iTeam);
   aSelfDefense[3]->SetTeam(iTeam);
  }

  if(pPoint == aFlag[3])
  {
   aSelfDefense[4]->SetTeam(iTeam);
   aSelfDefense[7]->SetTeam(iTeam);
  }

  if(pPoint == aFlag[4])
  {
   aSelfDefense[5]->SetTeam(iTeam);
   aSelfDefense[6]->SetTeam(iTeam);
  }
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Rock Go On.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Helikopter und Hinweisschilder
  if(!FindObject(NOBH))
  {
   SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,1395,1330,-1),70*21);
   SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,2096,790,-1),70*21);
   SetupVehicleSpawn([APCE],DIR_Left,CreateObject(VSPW,3636,790,-1),70*21);
   SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4335,1330,-1),70*21);

   var sign = CreateObject(SNPT, 1405, 1360, -1);
   sign->SetAction("Sign3");
   sign->Light();
   sign = CreateObject(SNPT, 4325, 1360, -1);
   sign->SetAction("Sign3");
   sign->Light();
   sign = CreateObject(SGNP, 1875, 840, -1);
   sign->SetMode(1);
   sign->Set("Helicopter");
   sign = CreateObject(SGNP, 3855, 840, -1);
   sign->SetMode(1);
   sign->Set("Helicopter");
  }

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,1405,780,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1180,770);
   aFlag[0] -> AddSpawnPoint(1180,880);
   aFlag[0] -> AddSpawnPoint(1270,880);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2075,820,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(1560,970);
   aFlag[1] -> AddSpawnPoint(1650,880);
   aFlag[1] -> AddSpawnPoint(1650,970);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,2865,1310,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(2730,1450);
   aFlag[2] -> AddSpawnPoint(2840,1460);
   aFlag[2] -> AddSpawnPoint(2890,1460);
   aFlag[2] -> AddSpawnPoint(3000,1450);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,3655,820,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(4080,970);
   aFlag[3] -> AddSpawnPoint(4080,880);
   aFlag[3] -> AddSpawnPoint(4170,970);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,4325,780,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(4460,880);
   aFlag[4] -> AddSpawnPoint(4550,880);
   aFlag[4] -> AddSpawnPoint(4550,770);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   //Versorgungskiste (APW)
   var crate = CreateObject(AMCT, 2865, 1350, -1);
   crate->Set(ATWN);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1); aSelfDefense[2]->SetTeam(1); aSelfDefense[3]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[4]->SetTeam(2); aSelfDefense[5]->SetTeam(2); aSelfDefense[6]->SetTeam(2); aSelfDefense[7]->SetTeam(2);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
   aSelfDefense[2]->TurnOn();
   aSelfDefense[3]->TurnOn();
   aSelfDefense[4]->TurnOn();
   aSelfDefense[5]->TurnOn();
   aSelfDefense[6]->TurnOn();
   aSelfDefense[7]->TurnOn();
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 2865, 1310, -1);
   flag->~Set("$Flag3$");

   //Teamgrenzen
   CreateObject(BRDR, 2180, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 3550, 0, -1)->Set(1,1,0,1,2);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Kugeln
    PlaceSpawnpoint(ABOX, 2840, 1465);

    //Gewehrgranaten
    PlaceSpawnpoint(GBOX, 2890, 1465);
   }

   //SSA Besitzer setzen
   if(aTeams[1] == true)
    aSelfDefense[3]->SetTeam(1);
   if(aTeams[2] == true)
    aSelfDefense[4]->SetTeam(2);

   //SSA aktivieren
   aSelfDefense[3]->TurnOn();
   aSelfDefense[4]->TurnOn();

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[1]);
   RemoveObject(aSelfDefense[2]);
   RemoveObject(aSelfDefense[5]);
   RemoveObject(aSelfDefense[6]);
   RemoveObject(aSelfDefense[7]);
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
   //Geldsäcke
   AddMoneySpawn(2200, 885, [15]);
   AddMoneySpawn(2740, 1465, [20]);
   AddMoneySpawn(2990, 1465, [20]);
   AddMoneySpawn(3530, 885, [15]);

   //Teamgrenzen
   CreateObject(BRDR, 1820, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 3910, 0, -1)->Set(1,1,0,1,2);

   //Automat
   var store = CreateObject(WPVM,2865, 1310,-1);
   store->AddWare(C4PA,-1);
   store->AddWare(FAPK,-1);
   store->AddWare(CDBT,-1);
   store->AddWare(BWTH,-1);
   store->AddWare(RSHL,-1);
   store->AddWare(ATWN,-1);
   store->AddWare(FGRN,-1);
   store->AddWare(FRAG,-1);
   store->AddWare(PGRN,-1);
   store->AddWare(SGRN,-1);
   store->AddWare(STUN,-1);
   store->AddWare(SRBL,-1);

   //Versorgungskiste (APW)
   var crate = CreateObject(AMCT, 2865, 1350, -1);
   crate->Set(ATWN);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
    aSelfDefense[3]->SetTeam(1);
   if(aTeams[2] == true)
    aSelfDefense[4]->SetTeam(2);

   //SSA aktivieren
   aSelfDefense[3]->TurnOn();
   aSelfDefense[4]->TurnOn();

   //Objekte entfernen
   RemoveObject(aSelfDefense[0]);
   RemoveObject(aSelfDefense[1]);
   RemoveObject(aSelfDefense[2]);
   RemoveObject(aSelfDefense[5]);
   RemoveObject(aSelfDefense[6]);
   RemoveObject(aSelfDefense[7]);
  }

  //LMS-Spielziel
  if(FindObject(GLMS))
  {
   //Automat
   var store = CreateObject(WPVM,2865, 1310,-1);
   store->AddWare(C4PA,-1);
   store->AddWare(FAPK,-1);
   store->AddWare(CDBT,-1);
   store->AddWare(BWTH,-1);
   store->AddWare(RSHL,-1);
   store->AddWare(ATWN,-1);
   store->AddWare(FGRN,-1);
   store->AddWare(FRAG,-1);
   store->AddWare(PGRN,-1);
   store->AddWare(SGRN,-1);
   store->AddWare(STUN,-1);
   store->AddWare(SRBL,-1);

   //Versorgungskiste (APW)
   var crate = CreateObject(AMCT, 2865, 1350, -1);
   crate->Set(ATWN);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1); aSelfDefense[1]->SetTeam(1); aSelfDefense[2]->SetTeam(1); aSelfDefense[3]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[4]->SetTeam(2); aSelfDefense[5]->SetTeam(2); aSelfDefense[6]->SetTeam(2); aSelfDefense[7]->SetTeam(2);}

   //SSA aktivieren
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();
   aSelfDefense[2]->TurnOn();
   aSelfDefense[3]->TurnOn();
   aSelfDefense[4]->TurnOn();
   aSelfDefense[5]->TurnOn();
   aSelfDefense[6]->TurnOn();
   aSelfDefense[7]->TurnOn();
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 2865; iY = 1135;
   return 1;
  }

  //HTF/MR/LMS-Spielziel
  if(FindObject(GHTF) || FindObject(GMNR) || FindObject(GLMS))
  {
   if(iTeam == 1)
   {
    return [[1140, 880], [1270, 880], [1655, 970]];
   }
   if(iTeam == 2)
   {
    return [[4070, 970], [4460, 880], [4590, 880]];
   }
   return 1;
  }
}