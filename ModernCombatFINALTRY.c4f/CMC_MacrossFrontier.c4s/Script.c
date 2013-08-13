/*-- Macross Frontier --*/

#strict
#include CSTD

func RecommendedGoals()	{return [GHTF, GTDM];}	//Spielzielempfehlung
func ChooserBlockedTeams(id idGoal)
{
  //Bei CTF: Nur zwei Teams erlauben
  if(idGoal == GCTF)
   return 3;
}


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(1, 50, 15);
  //Einrichtung plazieren
  CreateInterior();
  //Ausrüstung plazieren
  CreateEquipment();
  //Dekoration plazieren
  CreateDecoration();
  //Kein CTF bei mehr als 2 Teams
  if(GetTeamCount() > 2) RemoveAll(GCTF);
  return(1);
}

/* Plazierungslisten */

func CreateInterior()
{
  Log("$CreatingInterior$");

  //Leitern
  CreateObject(LADR, 95, 438, -1)->Set(10);
  CreateObject(LADR, 306, 397, -1)->Set(5);
  CreateObject(LADR, 321, 310, -1)->Set(9);
  CreateObject(LADR, 481, 310, -1)->Set(9);
  CreateObject(LADR, 661, 266, -1)->Set(11);

  CreateObject(LADR, 891, 186, -1)->Set(11);
  CreateObject(LADR, 1071, 230, -1)->Set(9);
  CreateObject(LADR, 1232, 230, -1)->Set(9);
  CreateObject(LADR, 1248, 318, -1)->Set(5);
  CreateObject(LADR, 1457, 358, -1)->Set(10);

  //Bodenluken
  CreateObject(HA4K, 320, 233, -1);
  CreateObject(HA4K, 480, 233, -1);
  CreateObject(HA4K, 660, 173, -1);
  CreateObject(HA4K, 730, 443, -1);
  CreateObject(HA4K, 790, 443, -1);
  CreateObject(HA4K, 810, 363, -1);
  CreateObject(HA4K, 890, 93, -1);
  CreateObject(HA4K, 1070, 153, -1);
  CreateObject(HA4K, 1230, 153, -1);

  //Hydrauliktüren
  var door = CreateObject(SLDR, 55, 440, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR, 855, 500, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR, 1495, 360, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  //Rampen
  DrawMaterialQuad("Wall-Concrete2",360,350,390,340,390,350,375,350,true);
  DrawMaterialQuad("Wall-Concrete2",451,350,421,340,421,350,436,350,true);

  DrawMaterialQuad("Wall-Concrete2",461,470,431,460,431,470,446,470,true);
  DrawMaterialQuad("Wall-Concrete2",491,480,461,470,461,480,476,480,true);
  DrawMaterialQuad("Wall-Concrete2",431,460,401,450,401,460,416,460,true);
  DrawMaterialQuad("Wall-Concrete2",401,450,371,440,371,450,386,450,true);

  DrawMaterialQuad("Wall-Concrete2",490,430,520,420,520,430,505,430,true);
  DrawMaterialQuad("Wall-Concrete2",560,350,590,340,590,350,575,350,true);

  DrawMaterialQuad("Wall-Concrete2",630,460,660,450,660,460,645,460,true);
  DrawMaterialQuad("Wall-Concrete2",660,450,690,440,690,450,675,450,true);
  DrawMaterialQuad("Wall-Concrete2",600,470,630,460,630,470,615,470,true);
  DrawMaterialQuad("Wall-Concrete2",570,480,600,470,600,480,585,480,true);

  DrawMaterialQuad("Wall-Concrete2",951,390,921,380,921,390,936,390,true);
  DrawMaterialQuad("Wall-Concrete2",981,400,951,390,951,400,966,400,true);
  DrawMaterialQuad("Wall-Concrete2",921,380,891,370,891,380,906,380,true);
  DrawMaterialQuad("Wall-Concrete2",891,370,861,360,861,370,876,370,true);

  DrawMaterialQuad("Wall-Concrete2",991,270,961,260,961,270,976,270,true);
  DrawMaterialQuad("Wall-Concrete2",1061,351,1031,341,1031,351,1046,351,true);

  DrawMaterialQuad("Wall-Concrete2",1090,390,1120,380,1120,390,1105,390,true);
  DrawMaterialQuad("Wall-Concrete2",1060,400,1090,390,1090,400,1075,400,true);
  DrawMaterialQuad("Wall-Concrete2",1120,380,1150,370,1150,380,1135,380,true);
  DrawMaterialQuad("Wall-Concrete2",1150,370,1180,360,1180,370,1165,370,true);

  DrawMaterialQuad("Wall-Concrete2",1100,270,1130,260,1130,270,1115,270,true);
  DrawMaterialQuad("Wall-Concrete2",1191,270,1161,260,1161,270,1176,270,true);

  //Rohre
  CreateObject(PIPL, 0, 180, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 355, 420, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 730, 220, -1)->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 1105, 340, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 1550, 120, -1)->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid()->Left()->Solid();

  //Sprungschanzen
  CreateObject (JMPD, 705, 440, -1)->Set(85, -9);
  CreateObject (JMPD, 840, 360, -1)->Set(91, 13);

  //Glasscheiben
  CreateObject(PANE, 115, 340, -1);
  CreateObject(PANE, 115, 370, -1);

  CreateObject(PANE, 578, 420, -1);
  CreateObject(PANE, 972, 340, -1);

  CreateObject(PANE, 1430, 260, -1);
  CreateObject(PANE, 1430, 290, -1);

  //Autos
  var car = CreateObject(CCAR, 40, 60, -1);
  car->SetPhase(1);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);
  var car = CreateObject(CCAR, 1490, 40, -1);
  car->SetPhase(0);
  car->SetSolidMask(GetPhase() * 60, 26, 60, 26);

  //Kisten
  CreateObject(WCR2, 405, 302, -1);
  CreateObject(WCR2, 405, 320, -1);

  CreateObject(WCR2, 445, 230, -1)->AutoRespawn();
  CreateObject(WCR2, 980, 90, -1)->AutoRespawn();

  CreateObject(WCR2, 1145, 222, -1);
  CreateObject(WCR2, 1145, 240, -1);

  //Metallkisten
  CreateObject(MWCR, 305, 440, -1)->AutoRespawn();
  CreateObject(MWCR, 840, 500, -1);
  CreateObject(MWCR, 1240, 360, -1)->AutoRespawn();

  //Verbandskiste
  CreateObject(BECR, 820, 500, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 390, 230, -1)->AutoRespawn();
  CreateObject(XWCR, 1110, 150, -1)->AutoRespawn();

  //Benzinfässer
  CreateObject(PBRL, 465, 460, -1)->AutoRespawn();
  CreateObject(PBRL, 500, 480, -1)->AutoRespawn();
  CreateObject(PBRL, 515, 480, -1)->AutoRespawn();

  CreateObject(PBRL, 1040, 400, -1)->AutoRespawn();
  CreateObject(PBRL, 1055, 400, -1)->AutoRespawn();
  CreateObject(PBRL, 1095, 387, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 950, 90, -1)->AutoRespawn();
  CreateObject(XBRL, 960, 90, -1)->AutoRespawn();

  //Phosphorfass
  CreateObject(HBRL, 720, 500, -1)->AutoRespawn();

  //Benzintanks
  CreateObject(XTNK, 515, 230, -1)->AutoRespawn();
  CreateObject(XTNK, 1190, 150, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 40, 440, -1)->AutoRespawn();
  CreateObject(GSBL, 460, 230, -1)->AutoRespawn();
  CreateObject(GSBL, 1220, 360, -1)->AutoRespawn();

  //Steine
  CreateObject(STNE, 145, 55, -1);
  CreateObject(STNE, 1390, 40, -1);

  //Tore und Konsolen
  var autod = CreateObject (HNG3, 610, 340, -1);
  autod->Open();
  CreateObject (CONS, 470, 340, -1)
  ->Set(autod);
  var autod = CreateObject (HNG3, 940, 260, -1);
  autod->Open();
  CreateObject (CONS, 1080, 260, -1)
  ->Set(autod);

  //Verbundene Räume
  var doorw = CreateObject(GAT1, 776, 310, -1);
  CreateObject(ROOM, 895, 500, -1)->Connect(doorw);

  //Sounds

  //Wind
  CreateObject(SE4K, 585, 150, -1)->Set("WindSound*.ogg",775,250);

  //Rush Hour
  CreateObject(SE4K, 20, 60, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 1510, 40, -1)->Set("Traffic*.ogg",245,70);

  //Hallen
  CreateObject(SE4K, 490, 390, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 1060, 310, -1)->Set("Interior*.ogg",665,105);

  //Lautsprecher
  CreateObject(SE4K, 780, 250, -1)->Set("Announce*.ogg",3000,200);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject (AMCT, 75, 350, -1);
  crate->Set(ABOX);
  var crate = CreateObject (AMCT, 1475, 260, -1);
  crate->Set(ABOX);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 770, 490);

  //Raketen
  PlaceSpawnpoint(MBOX, 760, 375);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radio
  CreateObject(RDIO, 805, 500, -1);

  //Leitkegel
  CreateObject(TCCN, 410, 450, -1)->Light();
  CreateObject(TCCN, 440, 460, -1)->Light();
  CreateObject(TCCN, 625, 340, -1);
  CreateObject(TCCN, 925, 260, -1);
  CreateObject(TCCN, 1110, 380, -1)->Light();
  CreateObject(TCCN, 1140, 370, -1)->Light();

  //Labortische
  CreateObject(LTBL, 185, 440, -1);
  CreateObject(LTBL, 1370, 360, -1);

  //Monitore
  CreateObject(MNI2, 175, 428, -1);
  CreateObject(MNI2, 195, 428, -1);
  CreateObject(MNI2, 1360, 348, -1);
  CreateObject(MNI2, 1380, 348, -1);

  //Apparaturen
  CreateObject(GADG, 210, 440, -1);
  CreateObject(GADG, 560, 420, -1);
  CreateObject(GADG, 920, 500, -1)->Set(1);
  CreateObject(GADG, 990, 340, -1);
  CreateObject(GADG, 1340, 360, -1);

  //Straßenlichter
  CreateObject(SLGH, 440, 230, -1);
  CreateObject(SLGH, 1060, 150, -1);

  //Notausgangslichter
  CreateObject(ETLT, 20, 425, -1);
  CreateObject(ETLT, 895, 460, -1);
  CreateObject(ETLT, 1520, 345, -1);

  //Verkehrsampeln
  CreateObject(TLGH, 90, 60, -1);
  CreateObject(TLGH, 1460, 40, -1);

  //Schilder
  CreateObject(WLSN, 440, 450, -1);
  CreateObject(MSGN, 600, 470, -1);
  CreateObject(MSGN, 630, 460, -1);
  CreateObject(MSGN, 660, 450, -1);
  CreateObject(ESGN, 805, 435, -1);
  CreateObject(MSGN, 890, 370, -1);
  CreateObject(MSGN, 920, 380, -1);
  CreateObject(MSGN, 950, 390, -1);
  CreateObject(WLSN, 1125, 370, -1);

  //Alarmlampen
  CreateObject(ALGH, 710, 170, -1)->TurnOn();
  CreateObject(ALGH, 840, 90, -1)->TurnOn();

  //Radare
  CreateObject(RADR, 575, 170, -1);
  CreateObject(RADR, 925, 90, -1);

  //Zäune
  CreateObject(FENC, 410, 230, -1);
  CreateObject(FENC, 470, 230, -1);
  CreateObject(FENC, 1030, 150, -1);
  CreateObject(FENC, 1090, 150, -1);

  //Regale
  CreateObject(FRAM, 740, 500, -1);
  CreateObject(FRAM, 760, 500, -1);
  CreateObject(FRAM, 780, 500, -1);
  CreateObject(FRAM, 1520, 360, -1);

  //Scheinwerfer
  CreateObject(FLH2, 365, 210, -1)->SetRotation(-30);
  CreateObject(FLH2, 615, 150, -1)->SetRotation(40);
  CreateObject(FLH2, 1135, 130, -1)->SetRotation(50);

  //Glühbirnen
  CreateObject(LBGH, 1025, 190, -1);
  CreateObject(LBGH, 520, 270, -1);

  //Wandlampen
  CreateObject(BLGH, 185, 370, -1);
  CreateObject(BLGH, 1365, 270, -1);

  //Spinde
  CreateObject(LCKR, 130, 440, -1);
  CreateObject(LCKR, 150, 440, -1);
  CreateObject(LCKR, 1405, 360, -1);
  CreateObject(LCKR, 1425, 360, -1);

  //Topfpflanzen
  CreateObject(PLT3, 270, 440, -1);
  CreateObject(PLT3, 1280, 360, -1);

  //Geländer
  CreateObject(RAI1, 1, 60, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI1, 1420, 40, -1)->SetRail([1,1,1,1,1,1,1]);

  //Lüftungsgitter
  CreateObject(ENGT, 520, 360, -1);
  CreateObject(ENGT, 1030, 280, -1);

  //Feuerlöscher
  CreateObject(FIE2, 340, 435, -1);
  CreateObject(FIE2, 790, 435, -1);
  CreateObject(FIE2, 1210, 355, -1);

  //Poster
  CreateObject(PSTR, 760, 437, -1)->Set(3);

  //Bildschirme
  var screen = CreateObject(SCR3, 100, 400, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 270, 400, -1);
  screen->SetClrModulation(RGB(255,255,255));
  CreateObject(SCA2, 713, 325, -1)->SetAction("Scan2");
  CreateObject(SCA2, 838, 325, -1);
  screen = CreateObject(SCR3, 1280, 320, -1);
  screen->SetClrModulation(RGB(255,255,255));
  screen = CreateObject(SCR3, 1450, 320, -1);
  screen->SetClrModulation(RGB(255,255,255));

  //Büsche
  CreateObject(BSH2, 200, 190, -1);
  CreateObject(BSH2, 260, 85, -1);
  CreateObject(BSH2, 745, 200, -1);
  CreateObject(BSH2, 1300, 120, -1);

  //Ventillatoren
  CreateObject(VENT, 350, 335, -1)->SetCon(40);
  CreateObject(VENT, 1200, 260, -1)->SetCon(40);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD08,400,530,-1)->SetClrModulation(RGB(125,125,125));
  CreateObject(BD08,1300,530,-1)->SetClrModulation(RGB(125,125,125));
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Rock Go On.ogg");

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Flaggenposten
   var flag = CreateObject(OFPL, 795,360, -1);
   flag->~Set("$Flag1$");

   //Objekte entfernen
   RemoveAll(JPTP);
   RemoveAll(JMPD);

   //Zusätzliche Munition
   if(!FindObject(NOAM))
   {
    //Versorgungskiste (Kugeln)
    var crate = CreateObject (AMCT, 760, 440, -1);
    crate->Set(ABOX);
   }
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
   //Hinweisschilder
   var sign = CreateObject(SGNP, 560, 480, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   var sign = CreateObject(SGNP, 990, 400, -1);
   sign->SetMode(1);
   sign->Set("Turret");

   //Geldsäcke
   AddMoneySpawn(610, 240, [20]);
   AddMoneySpawn(780, 50, [20]);
   AddMoneySpawn(810, 430, [20]);
   AddMoneySpawn(940, 160, [20]);
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   //Hinweisschilder
   var sign = CreateObject(SGNP, 560, 480, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   var sign = CreateObject(SGNP, 990, 400, -1);
   sign->SetMode(1);
   sign->Set("Turret");

   //Flaggen
   CreateFlag(1,110,440,GetTeamColor(1)); 
   CreateFlag(2,1440,360,GetTeamColor(2)); 
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Hinweisschilder
   var sign = CreateObject(SGNP, 560, 480, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   var sign = CreateObject(SGNP, 990, 400, -1);
   sign->SetMode(1);
   sign->Set("Turret");
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 785; iY = 350;
   return(1);
  }

  //HTF-Spielziel
  if(FindObject(GHTF))
  {
   //Gesonderte Spawnpoints bei 2 Teams
   if(GetActiveTeamCount() == 2)
   {
    if(iTeam == GetTeamByIndex(0))
    {
     return [[220, 200], [220, 430], [250, 140]];
    }
    if(iTeam == GetTeamByIndex(1))
    {
     return [[1265, 85], [1295, 120], [1330, 350]];
    }
    return(1);
   }
   else
   {
    return [[340, 220], [340, 340], [340, 430], [1210, 140], [1210, 260], [1210, 350]];
   }
  }

  //MR/LMS/DM-Spielziel
  if(FindObject(GMNR) || FindObject(GLMS) || FindObject(GTDM))
  {
   //Gesonderte Spawnpoints bei 2 Teams
   if(GetActiveTeamCount() == 2)
   {
    if(iTeam == GetTeamByIndex(0))
    {
     return [[220, 200], [220, 430], [250, 140]];
    }
    if(iTeam == GetTeamByIndex(1))
    {
     return [[1265, 85], [1295, 120], [1330, 350]];
    }
    return(1);
   }
   else
   {
    return [[340, 340], [350, 430], [530, 410], [1020, 330], [1220, 260], [1200, 350]];
   }
  }

  //CTF-Spielziel
  if(FindObject(GCTF))
  {
   if(iTeam == GetTeamByIndex(0))
   {
    return [[220, 200], [250, 140], [365, 200]];
   }
   if(iTeam == GetTeamByIndex(1))
   {
    return [[1135, 120], [1265, 85], [1295, 120]];
   }
   return(1);
  }
}