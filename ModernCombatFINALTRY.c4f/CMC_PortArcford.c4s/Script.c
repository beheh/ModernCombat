/*-- Port Arcford --*/

#strict
#include CSTD

static aFlag,aStationary;

func RecommendedGoals()			{return [GOCC, GASS];}	//Spielzielempfehlung
public func AssaultDefenderTeam()	{return 2;}		//Verteidigerteam bei Assault


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetWaitingMusic();
  //Himmelparallaxität
  SetSkyParallax(0,50,15);
  //Bildschirmfärbung
  SetGamma(RGB(7,6,0), RGB(152,147,128), RGB(255,254,236));
  //Flaggen
  aFlag = [];
  //Geschützstellungen
  aStationary = [];
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

  //Rampen
  DrawMaterialQuad("Earth-ROUGH",4440,690,4440,680,4410,690,4440,690,true);
  DrawMaterialQuad("Wall-Concrete3",4470,680,4470,670,4440,680,4470,680,true);
  DrawMaterialQuad("Wall-Concrete3",4500,670,4500,660,4470,670,4500,670,true);

  //Leitern
  CreateObject(LADR, 2220, 750, -1)->Set(18);
  CreateObject(LADR, 2300, 580, -1)->Set(7);
  CreateObject(LADR, 2990, 535, -1)->Set(9);
  CreateObject(LADR, 3970, 668, -1)->Set(5);
  CreateObject(LADR, 4140, 655, -1)->Set(11);
  CreateObject(LADR, 4250, 655, -1)->Set(11);
  CreateObject(LADR, 4440, 655, -1)->Set(11);
  CreateObject(LADR, 4720, 635, -1)->Set(10);
  CreateObject(LADR, 5085, 560, -1)->Set(9);
  CreateObject(LADR, 5015, 480, -1)->Set(3);
  CreateObject(LADR, 5220, 704, -1)->Set(17);

  //Bodenlucken
  CreateObject(HA4K, 2220, 603, -1);
  CreateObject(HA4K, 2220, 683, -1);
  CreateObject(HA4K, 2300, 523, -1);
  CreateObject(HA4K, 3970, 623, -1);
  CreateObject(HA4K, 5220, 563, -1);
  CreateObject(HA4K, 5220, 663, -1);

  //Schutztüren
  CreateObject(GDDR, 4745, 660, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 4785, 660, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 5055, 660, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 5105, 660, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 5175, 560, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 5275, 560, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Holzhütten
  CreateObject(HTB2, 960, 500, -1)->SetR(2);
  CreateObject(HTB2, 3510, 590, -1)->SetR(-2);
  CreateObject(HTB2, 3740, 640, -1)->SetR(2);

  //Benzinfässer
  CreateObject(PBRL, 2730, 561, -1);
  CreateObject(PBRL, 4640, 660, -1)->AutoRespawn();
  CreateObject(PBRL, 4910, 660, -1)->AutoRespawn();

  //Explosivfässer
  CreateObject(XBRL, 4660, 660, -1)->AutoRespawn();
  CreateObject(XBRL, 4920, 660, -1)->AutoRespawn();
  CreateObject(XBRL, 5010, 660, -1)->AutoRespawn();
  CreateObject(XBRL, 5200, 560, -1);

  //Gasflaschen
  CreateObject(GSBL, 2270, 520, -1);
  CreateObject(GSBL, 2270, 750, -1)->AutoRespawn();
  CreateObject(GSBL, 3670, 610, -1)->AutoRespawn();
  CreateObject(GSBL, 3940, 620, -1)->AutoRespawn();
  CreateObject(GSBL, 4230, 561, -1);
  CreateObject(GSBL, 4600, 561, -1);
  CreateObject(GSBL, 4820, 561, -1);
  CreateObject(GSBL, 4980, 660, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 4650, 660, -1)->AutoRespawn();
  CreateObject(XTNK, 5140, 660, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 2170, 600, -1);
  CreateObject(WCR2, 2270, 680, -1);
  CreateObject(WCR2, 3900, 620, -1)->AutoRespawn();
  CreateObject(WCR2, 3920, 602, -1);
  CreateObject(WCR2, 3920, 620, -1);

  //Metallkisten
  CreateObject(MWCR, 2255, 520, -1)->AutoRespawn();
  CreateObject(MWCR, 2300, 600, -1)->AutoRespawn();
  CreateObject(MWCR, 3530, 590, -1)->AutoRespawn();
  CreateObject(MWCR, 4190, 561, -1);
  CreateObject(MWCR, 4210, 561, -1);
  CreateObject(MWCR, 4580, 561, -1);
  CreateObject(MWCR, 4630, 561, -1);
  CreateObject(MWCR, 4690, 660, -1);
  CreateObject(MWCR, 4830, 660, -1)->AutoRespawn();
  CreateObject(MWCR, 5270, 660, -1);

  //Verbandskisten
  CreateObject(BECR, 2290, 750, -1)->AutoRespawn();
  CreateObject(BECR, 3880, 620, -1);
  CreateObject(BECR, 5250, 660, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 2280, 600, -1);
  CreateObject(XWCR, 3995, 620, -1);
  CreateObject(XWCR, 4710, 660, -1)->AutoRespawn();

  //Rohre
  CreateObject(PIPL, 0, 490, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 2000, 740, -1)->Solid()->Right()->Solid()->Down()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Up()->Solid()->Up()->Solid()->Up()->Up()->Solid()->Right()->Solid();
  CreateObject(PIPL, 0, 490, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right();
  CreateObject(PIPL, 2550, 590, -1)->Right();
  CreateObject(PIPL, 2600, 590, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 2650, 590, -1)->Right();
  CreateObject(PIPL, 2700, 590, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 2750, 590, -1)->Right();
  CreateObject(PIPL, 2800, 590, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 2850, 590, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 2950, 590, -1)->Right()->Up()->Solid()->Up()->Solid()->Right()->Solid();
  CreateObject(PIPL, 3050, 490, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 3100, 490, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 3150, 490, -1)->Right()->Right();
  CreateObject(PIPL, 3245, 490, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 3295, 490, -1)->Right()->Right();
  CreateObject(PIPL, 3395, 490, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 3545, 490, -1)->Right()->Right();
  CreateObject(PIPL, 3645, 490, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 3695, 490, -1)->Solid()->Right()->Solid()->Down()->Solid()->Right()->Solid()->Down()->Solid()->Right()->Solid();
  CreateObject(PIPL, 3845, 590, -1)->Right()->Right()->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 4145, 590, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 4245, 590, -1)->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 4445, 590, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 4695, 590, -1)->Right();
  CreateObject(PIPL, 4745, 590, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 4845, 590, -1)->Right()->Right()->Right()->Down()->Down();
  CreateObject(PIPL, 4845, 590, -1)->Right()->Down()->Down();

  //Schutt
  CreateObject(DEB1, 2390, 680, -1);
  CreateObject(DEB1, 4030, 680, -1);

  //Steine
  CreateObject(STNE, 760, 510, -1);
  CreateObject(STNE, 1260, 570, -1)->Set(1);
  CreateObject(STNE, 1290, 550, -1)->Set(2);
  CreateObject(STNE, 1665, 672, -1)->Set(1);
  CreateObject(STNE, 1740, 690, -1)->Set(3);
  CreateObject(STNE, 2765, 680, -1)->Set(4);
  CreateObject(STNE, 3115, 600, -1)->Set(1);
  CreateObject(STNE, 3480, 630, -1)->Set(4);

  //Sandsackbarrieren
  CreateObject(SBBA, 1890, 680, -1);
  CreateObject(SBBA, 3340, 630, -1);
  CreateObject(SBBA, 4320, 690, -1);
  CreateObject(SBBA, 4780, 480, -1);

  //Kran und Konsole
  var cran = CreateObject(CRN1, 5430, 577, -1);
  cran->Set(19);
  CreateObject (CONS, 5240, 555, -1)->Set(cran);

  //Sounds

  //Wind
  CreateObject(SE4K, 1500, 400, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2000, 400, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 3000, 400, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 3000, 400, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 4500, 400, -1)->Set("WindSound*.ogg",775,250);

  //Erdrutsche
  CreateObject(SE4K, 400, 400, -1)->Set("FallingDirt*.ogg",775,250);
  CreateObject(SE4K, 1800, 400, -1)->Set("FallingDirt*.ogg",775,250);

  //Zikaden
  CreateObject(SE4K, 700, 500, -1)->Set("Cicada*",850,250);
  CreateObject(SE4K, 2600, 500, -1)->Set("Cicada*",850,250);

  //Hallen
  CreateObject(SE4K, 2260, 540, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2960, 510, -1)->Set("Interior*.ogg",670,105);

  //Wellen
  CreateObject(SE4K, 5000, 750, -1)->Set("Wave*.ogg",200,75);
  CreateObject(SE4K, 5500, 740, -1)->Set("Wave*.ogg",150,50);
  CreateObject(SE4K, 6000, 730, -1)->Set("Wave*.ogg",100,35);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 1170, 550, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 3020, 690, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 5300, 560, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject (AMCT, 1310, 530, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 2970, 700, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 5330, 560, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 1900, 675);
  PlaceSpawnpoint(MBOX, 4355, 685);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radio
  CreateObject(RDIO, 5520, 560, -1);

  //Leitkegel
  CreateObject(TCCN, 4450, 675, -1)->SetR(-20);
  CreateObject(TCCN, 4470, 670, -1)->SetR(-20);
  CreateObject(TCCN, 4490, 663, -1)->SetR(-20);

  //Bojen
  CreateObject(BUOY, 5390, 733, -1);
  CreateObject(BUOY, 5600, 733, -1);

  //Geländer
  CreateObject(RAI1, 4740, 560, -1)->SetRail([1,1]);
  CreateObject(BART, 2150, 680, -1)->SetRail([1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1]);
  CreateObject(RAI1, 5070, 660, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 5150, 560, -1);
  CreateObject(RAI1, 5280, 560, -1);

  //Flaschen
  CreateObject(BOTL, 2340, 600, -1);
  CreateObject(BOTL, 2350, 600, -1);

  //Monitore
  CreateObject(MNI2, 2190, 600, -1);
  CreateObject(MNI2, 4830, 642, -1);

  //Markierungsschilder
  CreateObject(MSGN, 4745, 560, -1);
  CreateObject(MSGN, 5105, 560, -1);

  //Zäune
  CreateObject(FENC, 2340, 680, -1);
  CreateObject(FENC, 3380, 630, -1);
  CreateObject(FENC, 3440, 630, -1);
  CreateObject(FENC, 3540, 590, -1);
  CreateObject(FENC, 3600, 590, -1);
  CreateObject(FENC, 3880, 620, -1);
  CreateObject(FENC, 3940, 620, -1);
  CreateObject(FENC, 4530, 660, -1)->Set(2);
  CreateObject(FENC, 4590, 660, -1)->Set(2);
  CreateObject(FENC, 4650, 660, -1);
  CreateObject(FENC, 4710, 660, -1);

  //Feuerlöscher
  CreateObject(FIE2, 4765, 655, -1);
  CreateObject(FIE2, 5080, 655, -1);

  //Satellitenschüssel
  CreateObject(SADH, 3440, 630, -1);

  //Notausgangslicht
  CreateObject(ETLT, 5040, 630, -1);

  //Wandlampen
  CreateObject(BLGH, 2230, 565, -1);
  CreateObject(BLGH, 2290, 645, -1);
  CreateObject(BLGH, 5225, 530, -1);

  //Deckenlichter
  CreateObject(CLGH,4840,510,-1);
  CreateObject(CLGH,4940,510,-1);
  CreateObject(CLGH,5025,510,-1);
}

func CreateOptionalFeatures()
{
  //Hintergrund
  CreateObject(BD01,500,900,-1);
}

/* Bei Relaunch */
 
public func OnClassSelection(object pClonk, int iTeam)
{
  //Assault-Spielziel
  if(FindObject(GASS))
  {
   if(GetPlayerTeam(GetOwner(pClonk)) == 1)
   {
    if(GetAssaultTarget(0,1) || GetAssaultTarget(1,1))
    {
     AddEffect("IntPara", pClonk, 1, 1);
     AddEffect("Flying", pClonk, 101, 5);
     Sound("Airstrike2", 0, pClonk);
    }
   }
  }
}
 
global func FxIntParaTimer(object pTarget)
{
  CreateObject(PARA,0,0,GetOwner(pTarget))->Set(pTarget);
  return -1;
}


/* Regelwähler */

public func ChooserFinished()
{
  inherited();

  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Eurocorps.ogg");

  //Teams abfragen
  var aTeams = [false,false,false,false,false];
  for(var i = 0; i < GetPlayerCount(); i++)
   aTeams[GetPlayerTeam(GetPlayerByIndex(i))] = true;

  //OP-Spielziel
  if(FindObject(GOCC))
  {
   //Flaggenposten
   aFlag[0] = CreateObject(OFPL,1610,670,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1085,500);
   aFlag[0] -> AddSpawnPoint(1330,520);
   aFlag[0] -> AddSpawnPoint(1200,470);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2335,520,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(2240,740);
   aFlag[1] -> AddSpawnPoint(2475,700);
   aFlag[1] -> AddSpawnPoint(2160,740);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,3080,600,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(3120,720);
   aFlag[2] -> AddSpawnPoint(3180,730);
   aFlag[2] -> AddSpawnPoint(3380,690);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,3580,590,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(3810,530);
   aFlag[3] -> AddSpawnPoint(3770,700);
   aFlag[3] -> AddSpawnPoint(3930,660);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,4870,660,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(5030,470);
   aFlag[4] -> AddSpawnPoint(5100,550);
   aFlag[4] -> AddSpawnPoint(5255,550);
   if(aTeams[2] == true)
   {
    aFlag[4]->Set("$Flag5$",100,2);
    aFlag[4]->Capture(2,1);
   }
   else
   {
    aFlag[4]->Set("$Flag5$",0,2);
   }

   //Versorgungskiste (Phosphorgranaten)
   var crate = CreateObject(AMCT, 3380, 630, -1);
   crate->Set(PGRN);

   //Grenzen setzen
   CreateObject(BRDR, 700, 0, -1)->Set(0,1);
   CreateObject(BRDR, 5600, 0, -1)->Set(1,1);

   //Hinweisschilder
   CreateObject(SNPT, 430, 461, -1);
   CreateObject(SGNP, 610, 490, -1);

   //Geschützstellungen
   CreateObject(GNET, 1040, 510, -1)->Set(LCAC,0,1);
   CreateObject(GNET, 5225, 480, -1)->Set(LCAC,0,1);

   //MAV-Stationen
   CreateObject(MVSN, 2320, 750, -1)->Set(2190,520,1);
   CreateObject(MVSN, 3725, 640, -1)->Set(3610,590,1);

   //Helikopter und Hinweisschilder
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,910,470,-1),100*21);
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4925,420,-1),100*21);

    var sign = CreateObject(SGNP, 990, 500, -1);
    sign->Set("Helicopter");
    var sign = CreateObject(SGNP, 5035, 480, -1);
    sign->SetMode(1);
    sign->Set("Helicopter");
   }
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 2025, 711, 30*30, 2, "$Target1$", 0, [[[2620, 640], [2680, 610], [2735, 670]], [[1080, -20], [1130, -20], [1180, -20]]]);
   AddAssaultTarget(CMSN, 2250, 600, 30*30, 2, "$Target2$", 1, [[[2620, 640], [2680, 610], [2735, 670]], [[1080, -20], [1130, -20], [1180, -20]]]);
   AddAssaultTarget(CMSN, 3080, 600, 30*30, 2, "$Target3$", 2, [[[3770, 710], [3950, 620], [3930, 670]], [[2160, 750], [2190, 600], [2190, 680]]]);
   AddAssaultTarget(HUT3, 3390, 630, 30*30, 2, "$Target4$", 3, [[[3770, 710], [3950, 620], [3930, 670]], [[2160, 750], [2190, 600], [2190, 680]]]);
   AddAssaultTarget(PMP2, 4385, 600, 30*30, 2, "$Target5$", 4, [[[5255, 560], [5300, 640], [5350, 560]], [[3290, 680], [3450, 630], [3480, 460]]]);
   AddAssaultTarget(PMP2, 4895, 620, 30*30, 2, "$Target5$", 5, [[[5255, 560], [5300, 640], [5350, 560]], [[3290, 680], [3450, 630], [3480, 460]]]);

   //Ziele verbinden
   ConnectAssaultTargets([0, 1]);
   ConnectAssaultTargets([2, 3]);
   ConnectAssaultTargets([4, 5]);

   //Versorgungskiste (Dragnin)
   crate = CreateObject(AMCT, 1435, 650, -1);
   crate->Set(DGNN);

   //Versorgungskiste (Sensorbälle)
   crate = CreateObject(AMCT, 4380, 690, -1);
   crate->Set(SRBL);

   //Versorgungskiste (Sprengfallentaschen)
   crate = CreateObject(AMCT, 5480, 660, -1);
   crate->Set(BTBG);

   //Grenzen setzen
   CreateObject(BRDR, 700, 0, -1)->Set(0,1);
   CreateObject(BRDR, 2850, 0, -1)->Set(1,1);

   //Hinweisschilder
   CreateObject(SNPT, 430, 461, -1);
   CreateObject(SGNP, 610, 490, -1);

   //Geschützstellungen
   aStationary[0] = CreateObject(GNET, 1890, 680, -1);
   aStationary[0] -> Set(SATW,-90);
   aStationary[1] = CreateObject(GNET, 2330, 520, -1);
   aStationary[2] = CreateObject(GNET, 3340, 630, -1);
   aStationary[2] -> Set(0,-90);
   aStationary[3] = CreateObject(GNET, 4320, 690, -1);
   aStationary[3] -> Set(0,-90,1);

   //Selbstschussanlage
   var selfdef = CreateObject(SEGU, 1040, 510, -1);
   selfdef->Set(0,1,1,180,0,1470);
   selfdef->SetTeam(1);

   //Helikopter und Hinweisschilder
   if(!FindObject(NOHC))
   {
    SetupVehicleSpawn([APCE],DIR_Right,CreateObject(VSPW,910,470,-1),50*21);

    var sign = CreateObject(SGNP, 990, 500, -1);
    sign->Set("Helicopter");
   }
  }

  //MR-Spielziel
  if(FindObject(GMNR))
  {
   //Geldsäcke
   AddMoneySpawn(2630, 635, [20]);
   AddMoneySpawn(2995, 695, [20]);
   AddMoneySpawn(3460, 625, [20]);

   //Grenzen setzen
   CreateObject(BRDR, 1700, 0, -1)->Set(0);
   CreateObject(BRDR, 4210, 0, -1)->Set(1);

   //Teamgrenzen
   CreateObject(BRDR, 2490, 0, -1)->Set(0,1,0,1,1);
   CreateObject(BRDR, 3670, 0, -1)->Set(1,1,0,1,2);

   //Hinweisschilder
   CreateObject(SGNP, 1510, 590, -1);
   CreateObject(SGNP, 1610, 670, -1);
   CreateObject(SGNP, 4140, 680, -1);
   CreateObject(SGNP, 4240, 680, -1);

   //Geschützstellungen
   CreateObject(GNET, 2330, 520, -1)->Set(0,90,1);
   CreateObject(GNET, 3340, 630, -1)->Set(0,-90,1);

   //MAV-Stationen
   CreateObject(MVSN, 2320, 750, -1)->Set(2190,520,1);
   CreateObject(MVSN, 3725, 640, -1)->Set(3610,590,1);
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Grenzen setzen
   CreateObject(BRDR, 1700, 0, -1)->Set(0);
   CreateObject(BRDR, 4210, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 1510, 590, -1);
   CreateObject(SGNP, 1610, 670, -1);
   CreateObject(SGNP, 4140, 680, -1);
   CreateObject(SGNP, 4240, 680, -1);

   //Geschützstellungen
   CreateObject(GNET, 2330, 520, -1)->Set(0,90,1);
   CreateObject(GNET, 3340, 630, -1)->Set(0,-90,1);

   //MAV-Stationen
   CreateObject(MVSN, 2320, 750, -1)->Set(2190,520,1);
   CreateObject(MVSN, 3725, 640, -1)->Set(3610,590,1);
  }
}

/* Assault Zerstörung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1 und 2
  if(!iIndex || iIndex == 1)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 700, 0, -1)->Set(0,1);
    CreateObject(BRDR, 4100, 0, -1)->Set(1,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),2500,0,1,1);

    //Spawnpoint entfernen
    RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(909, 469, 2, 2)));

    //Geschützstellungen entfernen
    aStationary[0]->DecoExplode(30);
    aStationary[1]->DecoExplode(30);
   }
  }

  //Ziel 3 und 4
  if(iIndex == 2 || iIndex == 3)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 2160, 0, -1)->Set(0,1);
    CreateObject(BRDR, 5600, 0, -1)->Set(1,1);

    //Teamgrenze setzen
    FindObject(GASS)->CreateTeamBorder(AssaultDefenderTeam(),3850,0,1,1);

    //Geschützstellung entfernen
    aStationary[2]->DecoExplode(30);
   }
  }

  //Ziel 5 und 6
  if(iIndex == 4 || iIndex == 5)
  {
   if(fConnectedDestroyed)
   {
    //Geschützstellung entfernen
    aStationary[3]->DecoExplode(30);
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Startsicht
  if(!g_chooserFinished)
  {
   iX = 2120; iY = 590;
   return(1);
  }

  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //MR/LMS/DM-Spielziel
  if(FindObject(GMNR) || FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    return [[2200, 510], [2200, 590], [2240, 740]];
   }
   if(iTeam == 2)
   {
    return [[3745, 630], [3810, 530], [3790, 700]];
   }
   return(1);
  }
}