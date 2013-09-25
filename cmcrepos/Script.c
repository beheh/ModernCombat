/*-- Ground Zero --*/

#strict
#include CSTD

static aFlag,aSelfDefense;

func RecommendedGoals()	{return [GBAS];}	//Spielzielempfehlung


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0, 45, 15);
  //Flaggen
  aFlag = [];
  //Selbstschussanlagen
  aSelfDefense = [];
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

  //Aufzüge
  CreateObject(LFTP, 465, 500, -1);
  CreateObject(LFTP, 915, 380, -1);
  CreateObject(LFTP, 1325, 390, -1);
  var liftp = CreateObject (LFTP, 1405, 185, -1);
  liftp->DisableVertical();
  CreateObject(LFTP, 2075, 400, -1);
  CreateObject(LFTP, 2485, 405, -1);
  CreateObject(LFTP, 2935, 435, -1);

  //Leitern
  CreateObject(LADR, 340, 534, -1)->Set(17);
  CreateObject(LADR, 835, 320, -1)->Set(7);
  CreateObject(LADR, 995, 320, -1)->Set(7);
  CreateObject(LADR, 1135, 580, -1)->Set(16);

  CreateObject(LADR, 1325, 313, -1)->Set(17);
  CreateObject(LADR, 1505, 520, -1)->Set(9);
  CreateObject(LADR, 1895, 520, -1)->Set(9);
  CreateObject(LADR, 2075, 310, -1)->Set(17);

  CreateObject(LADR, 2265, 580, -1)->Set(16);
  CreateObject(LADR, 2405, 320, -1)->Set(7);
  CreateObject(LADR, 2565, 320, -1)->Set(7);
  CreateObject(LADR, 3060, 534, -1)->Set(17);

  //Bodenluken
  CreateObject(HA4K, 340, 393, -1);
  CreateObject(HA4K, 3060, 393, -1);

  //Große Bodenluken
  CreateObject(H24K, 1325, 178, -1);
  CreateObject(H24K, 2075, 178, -1);

  //Hydrauliktüren
  var door = CreateObject(SLDR, 320, 390, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  door = CreateObject(SLDR, 3080, 390, -1);
  door->Lock();
  door->SetMaxDamage(-1);

  //Stahlbrücken
  CreateObject(_HBR, 604, 352, -1);
  CreateObject(_HBR, 2797, 352, -1);

  //Glasscheiben
  CreateObject(PANE, 1212, 320, -1);
  CreateObject(PANE, 1212, 370, -1);
  CreateObject(PANE, 1212, 420, -1);
  CreateObject(PANE, 1222, 260, -1);
  CreateObject(PANE, 1282, 170, -1);
  CreateObject(PANE, 1428, 260, -1);
  CreateObject(PANE, 1438, 320, -1);
  CreateObject(PANE, 1438, 370, -1);
  CreateObject(PANE, 1438, 420, -1);

  CreateObject(PANE, 1962, 320, -1);
  CreateObject(PANE, 1962, 370, -1);
  CreateObject(PANE, 1962, 420, -1);
  CreateObject(PANE, 1972, 260, -1);
  CreateObject(PANE, 2118, 170, -1);
  CreateObject(PANE, 2178, 260, -1);
  CreateObject(PANE, 2188, 320, -1);
  CreateObject(PANE, 2188, 370, -1);
  CreateObject(PANE, 2188, 420, -1);

  //Explosivtanks
  CreateObject(XTNK, 10, 120, -1);
  CreateObject(XTNK, 110, 120, -1);
  CreateObject(XTNK, 210, 120, -1);
  CreateObject(XTNK, 310, 120, -1);
  CreateObject(XTNK, 600, 390, -1)->AutoRespawn();
  CreateObject(XTNK, 2800, 390, -1)->AutoRespawn();
  CreateObject(XTNK, 3085, 120, -1);
  CreateObject(XTNK, 3185, 120, -1);
  CreateObject(XTNK, 3285, 120, -1);
  CreateObject(XTNK, 3385, 120, -1);

  //Kisten
  CreateObject(WCR2, 1415, 420, -1)->AutoRespawn();
  CreateObject(WCR2, 1630, 630, -1)->AutoRespawn();
  CreateObject(WCR2, 1780, 630, -1)->AutoRespawn();
  CreateObject(WCR2, 1985, 420, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 810, 320, -1);
  CreateObject(MWCR, 952, 390, -1);
  CreateObject(MWCR, 2590, 320, -1);
  CreateObject(MWCR, 2450, 390, -1);

  //Verbandskisten
  CreateObject(BECR, 525, 390, -1)->AutoRespawn();
  CreateObject(BECR, 2875, 390, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 1455, 320, -1)->AutoRespawn();
  CreateObject(XWCR, 1945, 320, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 635, 280, -1)->AutoRespawn();
  CreateObject(XBRL, 725, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 1060, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 2335, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 2675, 450, -1)->AutoRespawn();
  CreateObject(XBRL, 2765, 280, -1)->AutoRespawn();

  //Phosphorfässer
  CreateObject(HBRL, 740, 450, -1)->AutoRespawn();
  CreateObject(HBRL, 2660, 450, -1)->AutoRespawn();

  //Gasflaschen
  CreateObject(GSBL, 1270, 370, -1)->AutoRespawn();
  CreateObject(GSBL, 2130, 370, -1)->AutoRespawn();

  //Benzintanks
  CreateObject(XTNK, 620, 280, -1)->AutoRespawn();
  CreateObject(XTNK, 2780, 280, -1)->AutoRespawn();

  //Phosphortanks
  CreateObject(PTNK, 750, 450, -1)->AutoRespawn();
  CreateObject(PTNK, 2655, 450, -1)->AutoRespawn();

  //Sandsackbarrieren
  CreateObject(SBBA, 725, 290, -1)->Right();
  CreateObject(SBBA, 1645, 420, -1);
  CreateObject(SBBA, 1755, 420, -1)->Right();
  CreateObject(SBBA, 2675, 290, -1);

  //Schutztüren
  CreateObject(GDDR, 530, 290, -1);
  CreateObject(GDDR, 650, 390, -1);
  CreateObject(GDDR, 650, 460, -1);

  CreateObject(GDDR, 830, 450, -1);
  CreateObject(GDDR, 840, 390, -1);
  CreateObject(GDDR, 880, 320, -1);
  CreateObject(GDDR, 950, 320, -1);
  CreateObject(GDDR, 990, 390, -1);
  CreateObject(GDDR, 1000, 450, -1);

  CreateObject(GDDR, 1290, 260, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 1290, 370, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 1360, 260, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 1360, 370, -1)->SetColorDw(HSL(0, 0, 120, 127));

  CreateObject(GDDR, 2040, 260, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 2040, 370, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 2110, 260, -1)->SetColorDw(HSL(0, 0, 120, 127));
  CreateObject(GDDR, 2110, 370, -1)->SetColorDw(HSL(0, 0, 120, 127));

  CreateObject(GDDR, 2400, 450, -1);
  CreateObject(GDDR, 2410, 390, -1);
  CreateObject(GDDR, 2450, 320, -1);
  CreateObject(GDDR, 2520, 320, -1);
  CreateObject(GDDR, 2560, 390, -1);
  CreateObject(GDDR, 2570, 450, -1);

  CreateObject(GDDR, 2750, 390, -1);
  CreateObject(GDDR, 2750, 460, -1);
  CreateObject(GDDR, 2870, 290, -1);

  //Container
  CreateObject(CON1, 70, 358, -1)->SetPerspective(3);
  CreateObject(CON1, 70, 390, -1)->SetPerspective(3);
  CreateObject(CON1, 167, 358, -1)->SetPerspective(2);
  CreateObject(CON1, 165, 390, -1)->SetPerspective(2);
  CreateObject(CON1, 240, 390, -1);
  CreateObject(CON1, 3150, 390, -1)->SetPerspective(2);
  CreateObject(CON1, 3240, 358, -1);
  CreateObject(CON1, 3240, 390, -1);
  CreateObject(CON1, 3325, 358, -1)->SetPerspective(3);
  CreateObject(CON1, 3325, 390, -1)->SetPerspective(3);

  //Selbstschussanlagen und Konsolen
  aSelfDefense[0] = CreateObject(SEGU, 715, 329, -1);
    aSelfDefense[0]->Arm(MISA);
    CreateObject(CONS, 550, 385, -1)->Set(aSelfDefense[0]);
  
  aSelfDefense[1] = CreateObject(SEGU, 2685, 329, -1);
    aSelfDefense[1]->Arm(MISA);
    CreateObject(CONS, 2850, 385, -1)->Set(aSelfDefense[1]);

  //Tore und Konsolen
  var autod = CreateObject (HNG3, 1700, 610, -1);
  autod->Open();
  CreateObject (CONS, 1570, 605, -1)
  ->Set(autod);
  var autod = CreateObject (HNG3, 1700, 520, -1);
  autod->Open();
  CreateObject (CONS, 1830, 605, -1)
  ->Set(autod);

  //Sounds

  //Wind
  CreateObject(SE4K, 1100, 160, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2290, 160, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 490, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 915, 340, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 1325, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2075, 340, -1)->Set("Interior*.ogg",670,105);
  CreateObject(SE4K, 2485, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2910, 340, -1)->Set("Interior*.ogg",670,105);

  //Rush Hour
  CreateObject(SE4K, 1120, 390, -1)->Set("Traffic*.ogg",245,70);
  CreateObject(SE4K, 2270, 390, -1)->Set("Traffic*.ogg",245,70);

  //Sirenen
  CreateObject(SE4K, 750, 350, -1)->Set("Siren*.ogg",4000,140);
  CreateObject(SE4K, 2650, 350, -1)->Set("Siren*.ogg",4000,140);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Kugeln
  PlaceSpawnpoint(ABOX, 430, 285);
  PlaceSpawnpoint(ABOX, 1400, 250);
  PlaceSpawnpoint(ABOX, 2000, 250);
  PlaceSpawnpoint(ABOX, 2970, 285);

  //Gewehrgranaten
  PlaceSpawnpoint(GBOX, 1400, 615);
  PlaceSpawnpoint(GBOX, 1880, 615);

  //Raketen
  PlaceSpawnpoint(MBOX, 1250, 250);
  PlaceSpawnpoint(MBOX, 2150, 250);

  //Artilleriebatterien
  CreateObject(ATBY,1025,320,-1)->SetRotation(20);
  CreateObject(ATBY,2373,320,-1)->SetRotation(-20);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radios
  CreateObject(RDIO, 580, 540, -1);
  CreateObject(RDIO, 2820, 540, -1);

  //Leitkegel
  CreateObject(TCCN, 1215, 450, -1)->Light();
  CreateObject(TCCN, 1275, 460, -1)->Light();
  CreateObject(TCCN, 1465, 460, -1)->Light();
  CreateObject(TCCN, 1515, 440, -1)->Light();
  CreateObject(TCCN, 1885, 440, -1)->Light();
  CreateObject(TCCN, 1935, 460, -1)->Light();
  CreateObject(TCCN, 2225, 460, -1)->Light();
  CreateObject(TCCN, 2285, 450, -1)->Light();

  //Geländer
  CreateObject(RAI1, -3, 120, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 660, 390, -1)->SetRail([1]);
  CreateObject(RAI1, 660, 460, -1)->SetRail([1]);
  CreateObject(RAI1, 1630, 420, -1)->SetRail([1,1,1,1,1,1,1]);
  CreateObject(RAI1, 2715, 390, -1)->SetRail([1]);
  CreateObject(RAI1, 2715, 460, -1)->SetRail([1]);
  CreateObject(RAI1, 3037, 120, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1]);

  //Radare
  CreateObject(RADR, 470, 220, -1);
  CreateObject(RADR, 2930, 220, -1);

  //Automaten
  CreateObject(VGMN, 525, 460, -1);
  CreateObject(VGMN, 2870, 460, -1)->Set(2);

  //Glastische
  CreateObject(GTBL, 600, 460, -1);
  CreateObject(GTBL, 2800, 460, -1);

  //Wandlampen
  CreateObject(BLGH, 165, 340, -1);
  CreateObject(BLGH, 1240, 310, -1);
  CreateObject(BLGH, 1410, 310, -1);
  CreateObject(BLGH, 1990, 305, -1);
  CreateObject(BLGH, 2160, 305, -1);
  CreateObject(BLGH, 3235, 340, -1);

  //Büsche
  CreateObject(BSH2, 1530, 450, -1);
  CreateObject(BSH2, 1545, 450, -1);
  CreateObject(BSH2, 1850, 450, -1);
  CreateObject(BSH2, 1865, 450, -1);

  //Bäume
  CreateObject(TRE3, 1600, 433, -1);
  CreateObject(TRE3, 1800, 433, -1);

  //Lüftungsgitter
  CreateObject(ENGT, 465, 510, -1);
  CreateObject(ENGT, 2935, 510, -1);

  //Topfpflanzen
  CreateObject(PLNT, 500, 460, -1);
  CreateObject(PLT3, 560, 290, -1);
  CreateObject(PLT3, 880, 390, -1);
  CreateObject(PLNT, 1310, 310, -1);
  CreateObject(PLNT, 2090, 310, -1);
  CreateObject(PLT3, 2530, 390, -1);
  CreateObject(PLT3, 2840, 290, -1);
  CreateObject(PLNT, 2900, 460, -1);

  //Scheinwerfer
  CreateObject(FLH2, 1670, 420, -1);
  CreateObject(FLH2, 1730, 420, -1);

  //Flaschen
  CreateObject(BOTL, 590, 448, -1);
  CreateObject(BOTL, 2810, 448, -1);

  //Monitore
  CreateObject(MNI2, 610, 448, -1);
  CreateObject(MNI2, 2785, 448, -1);

  //Schilder
  CreateObject(WLSN, 500, 380, -1);
  CreateObject(WLSN, 2900, 380, -1);

  //Zäune
  CreateObject(FENC, 930, 260, -1);
  CreateObject(FENC, 2470, 260, -1);

  //Regale
  CreateObject(FRAM, 855, 440, -1);
  CreateObject(FRAM, 975, 440, -1);
  CreateObject(FRAM, 1285, 420, -1);
  CreateObject(FRAM, 1285, 440, -1);
  CreateObject(FRAM, 1365, 420, -1);
  CreateObject(FRAM, 1365, 440, -1);

  CreateObject(FRAM, 2035, 420, -1);
  CreateObject(FRAM, 2035, 440, -1);
  CreateObject(FRAM, 2115, 420, -1);
  CreateObject(FRAM, 2115, 440, -1);
  CreateObject(FRAM, 2425, 440, -1);
  CreateObject(FRAM, 2545, 440, -1);

  //Feuerlöscher
  CreateObject(FIE2, 1365, 455, -1);
  CreateObject(FIE2, 2035, 455, -1);

  //Straßenlichter
  CreateObject(SLGH, 60, 120, -1);
  CreateObject(SLGH, 160, 120, -1);
  CreateObject(SLGH, 260, 120, -1);
  CreateObject(SLGH, 360, 120, -1);
  CreateObject(SLGH, 420, 220, -1);
  CreateObject(SLGH, 2980, 220, -1);
  CreateObject(SLGH, 3040, 120, -1);
  CreateObject(SLGH, 3140, 120, -1);
  CreateObject(SLGH, 3240, 120, -1);
  CreateObject(SLGH, 3340, 120, -1);

  //Notausgangslichter
  CreateObject(ETLT, 625, 430, -1);
  CreateObject(ETLT, 2775, 430, -1);
}

func CreateOptionalFeatures()
{
  //Hintergründe
  CreateObject(BD04,520,550,-1)->Local()=50;
  CreateObject(BD04,1027,550,-1)->Local()=50;
  CreateObject(BD04,1534,550,-1)->Local()=50;
  CreateObject(BD10,300,650,-1)->SetClrModulation(RGB(130,130,130));
  CreateObject(BD10,1690,650,-1)->SetClrModulation(RGB(130,130,130));
}

/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Offensive.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //Base Assault-Spielziel
  if(FindObject(GBAS))
  {
   //Strukturen
   AddAssaultTarget(CCP2, 400, 390, 400, 1, "$Flag1$", 0, [[430, 540], [500, 540], [560, 560]]);
   AddAssaultTarget(CMSN, 1290, 520, 300, 1, "$Flag2$", 1, [[1230, 320], [1420, 320], [1300, 170]]);

   AddAssaultTarget(CCP2, 3000, 390, 400, 2, "$Flag5$", 0, [[2970, 540], [2900, 540], [2840, 560]]);
   AddAssaultTarget(CMSN, 2110, 520, 300, 2, "$Flag4$", 1, [[2170, 320], [1980, 320], [2100, 170]]);

   //Sprengsatz-Spawn
   SetupBombSpawnpoint(1700, 410);

   //Grenzen setzen
   CreateObject(BRDR, 320, 0, -1)->Set(0);
   CreateObject(BRDR, 3080, 0, -1)->Set(1);

   //SSA Besitzer setzen
   if(aTeams[1] == true)
   {aSelfDefense[0]->SetTeam(1);}
   if(aTeams[2] == true)
   {aSelfDefense[1]->SetTeam(2);}

   //SSA anschalten
   aSelfDefense[0]->TurnOn();
   aSelfDefense[1]->TurnOn();

   //Hinweisschilder
   var sign = CreateObject(SGNP, 780, 450, -1);
   sign->SetMode(1);
   sign->Set("Turret");
   sign = CreateObject(SGNP, 2620, 450, -1);
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
   iX = 1700; iY = 300;
   return(1);
  }
}