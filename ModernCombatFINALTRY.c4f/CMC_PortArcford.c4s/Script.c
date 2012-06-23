/*-- Port Arcford --*/

#strict
#include CSTD

static aFlag,aStationary;


/* Initialisierung */

func Initialize()
{
  //Wartemusik einstellen
  SetPlayList("CMC_WaitingforSpawn.ogg");
  Music("CMC_WaitingforSpawn.ogg",1);
  //Hintergrundbewegung
  SetSkyParallax(0,45,24,0,0,0,0);
  //Bildschirmf�rbung
  SetGamma(RGB(7,6,0), RGB(152,147,128), RGB(255,254,236) );
  //Flaggen
  aFlag = [];
  //Gesch�tzstellungen
  aStationary = [];
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

  //Rampen
  DrawMaterialQuad("Earth-ROUGH",4440,490,4440,480,4410,490,4440,490,true);
  DrawMaterialQuad("Wall-Concrete3",4470,480,4470,470,4440,480,4470,480,true);
  DrawMaterialQuad("Wall-Concrete3",4500,470,4500,460,4470,470,4500,470,true);

  //Leitern
  CreateObject(LADR, 2220, 550, -1)->Set(18);
  CreateObject(LADR, 2300, 380, -1)->Set(7);
  CreateObject(LADR, 2990, 335, -1)->Set(9);
  CreateObject(LADR, 3970, 468, -1)->Set(5);
  CreateObject(LADR, 4140, 455, -1)->Set(11);
  CreateObject(LADR, 4250, 455, -1)->Set(11);
  CreateObject(LADR, 4440, 455, -1)->Set(11);
  CreateObject(LADR, 4720, 435, -1)->Set(10);
  CreateObject(LADR, 5085, 360, -1)->Set(9);
  CreateObject(LADR, 5015, 280, -1)->Set(3);
  CreateObject(LADR, 5220, 504, -1)->Set(17);

  //Bodenlucken
  CreateObject(HA4K, 2220, 403, -1);
  CreateObject(HA4K, 2220, 483, -1);
  CreateObject(HA4K, 2300, 323, -1);
  CreateObject(HA4K, 3970, 423, -1);
  CreateObject(HA4K, 5220, 363, -1);
  CreateObject(HA4K, 5220, 463, -1);

  //Schutzt�ren
  CreateObject(GDDR, 4745, 460, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 4785, 460, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 5055, 460, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 5105, 460, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 5175, 360, -1)->SetColorDw(HSL(145, 210, 100, 127));
  CreateObject(GDDR, 5275, 360, -1)->SetColorDw(HSL(145, 210, 100, 127));

  //Holzh�tten
  CreateObject(HTB2, 960, 300, -1)->SetR(2);
  CreateObject(HTB2, 3510, 390, -1)->SetR(-2);
  CreateObject(HTB2, 3740, 440, -1)->SetR(2);

  //Benzinf�sser
  CreateObject(PBRL, 2730, 361, -1);
  CreateObject(PBRL, 4640, 460, -1)->AutoRespawn();
  CreateObject(PBRL, 4910, 460, -1)->AutoRespawn();

  //Explosivf�sser
  CreateObject(XBRL, 4660, 460, -1)->AutoRespawn();
  CreateObject(XBRL, 4920, 460, -1)->AutoRespawn();
  CreateObject(XBRL, 5010, 460, -1)->AutoRespawn();
  CreateObject(XBRL, 5200, 360, -1);

  //Gasflaschen
  CreateObject(GSBL, 2270, 320, -1);
  CreateObject(GSBL, 2270, 550, -1)->AutoRespawn();
  CreateObject(GSBL, 3670, 410, -1)->AutoRespawn();
  CreateObject(GSBL, 3940, 420, -1)->AutoRespawn();
  CreateObject(GSBL, 4230, 361, -1);
  CreateObject(GSBL, 4600, 361, -1);
  CreateObject(GSBL, 4820, 361, -1);
  CreateObject(GSBL, 4980, 460, -1)->AutoRespawn();

  //Explosivtanks
  CreateObject(XTNK, 4650, 460, -1)->AutoRespawn();
  CreateObject(XTNK, 5140, 460, -1)->AutoRespawn();

  //Kisten
  CreateObject(WCR2, 2170, 400, -1);
  CreateObject(WCR2, 2270, 480, -1);
  CreateObject(WCR2, 3900, 420, -1)->AutoRespawn();
  CreateObject(WCR2, 3920, 402, -1);
  CreateObject(WCR2, 3920, 420, -1);

  //Metallkisten
  CreateObject(MWCR, 2255, 320, -1)->AutoRespawn();
  CreateObject(MWCR, 2300, 400, -1)->AutoRespawn();
  CreateObject(MWCR, 3530, 390, -1)->AutoRespawn();
  CreateObject(MWCR, 4190, 361, -1);
  CreateObject(MWCR, 4210, 361, -1);
  CreateObject(MWCR, 4580, 361, -1);
  CreateObject(MWCR, 4630, 361, -1);
  CreateObject(MWCR, 4690, 460, -1);
  CreateObject(MWCR, 4830, 460, -1)->AutoRespawn();
  CreateObject(MWCR, 5270, 460, -1);

  //Verbandskisten
  CreateObject(BECR, 2290, 550, -1)->AutoRespawn();
  CreateObject(BECR, 3880, 420, -1);
  CreateObject(BECR, 5250, 460, -1)->AutoRespawn();

  //Explosive Kisten
  CreateObject(XWCR, 2280, 400, -1);
  CreateObject(XWCR, 3995, 420, -1);
  CreateObject(XWCR, 4710, 460, -1)->AutoRespawn();

  //Rohre
  CreateObject(PIPL, 0, 290, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Down()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 2000, 540, -1)->Solid()->Right()->Solid()->Down()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Up()->Solid()->Up()->Solid()->Up()->Up()->Solid()->Right()->Solid();
  CreateObject(PIPL, 0, 290, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Right();
  CreateObject(PIPL, 2550, 390, -1)->Right();
  CreateObject(PIPL, 2600, 390, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 2650, 390, -1)->Right();
  CreateObject(PIPL, 2700, 390, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 2750, 390, -1)->Right();
  CreateObject(PIPL, 2800, 390, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 2850, 390, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 2950, 390, -1)->Right()->Up()->Solid()->Up()->Solid()->Right()->Solid();
  CreateObject(PIPL, 3050, 290, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 3100, 290, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 3150, 290, -1)->Right()->Right();
  CreateObject(PIPL, 3245, 290, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 3295, 290, -1)->Right()->Right();
  CreateObject(PIPL, 3395, 290, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 3545, 290, -1)->Right()->Right();
  CreateObject(PIPL, 3645, 290, -1)->Solid()->Right()->Solid();
  CreateObject(PIPL, 3695, 290, -1)->Solid()->Right()->Solid()->Down()->Solid()->Right()->Solid()->Down()->Solid()->Right()->Solid();
  CreateObject(PIPL, 3845, 390, -1)->Right()->Right()->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 4145, 390, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 4245, 390, -1)->Right()->Right()->Right()->Right();
  CreateObject(PIPL, 4445, 390, -1)->Solid()->Right()->Solid()->Right()->Solid()->Right()->Solid()->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 4695, 390, -1)->Right();
  CreateObject(PIPL, 4745, 390, -1)->Solid()->Right()->Solid()->Right()->Solid();
  CreateObject(PIPL, 4845, 390, -1)->Right()->Right()->Right()->Down()->Down();
  CreateObject(PIPL, 4845, 390, -1)->Right()->Down()->Down();

  //Schutt
  CreateObject(DEB1, 2390, 480, -1);
  CreateObject(DEB1, 4030, 480, -1);

  //Steine
  CreateObject(STNE, 760, 310, -1);
  CreateObject(STNE, 1260, 370, -1)->Set(1);
  CreateObject(STNE, 1290, 350, -1)->Set(2);
  CreateObject(STNE, 1665, 472, -1)->Set(1);
  CreateObject(STNE, 1740, 490, -1)->Set(3);
  CreateObject(STNE, 2765, 480, -1)->Set(4);
  CreateObject(STNE, 3115, 400, -1)->Set(1);
  CreateObject(STNE, 3480, 430, -1)->Set(4);

  //Sandsackbarrieren
  CreateObject(SBBA, 1890, 480, -1);
  CreateObject(SBBA, 3340, 430, -1);
  CreateObject(SBBA, 4320, 490, -1);
  CreateObject(SBBA, 4780, 280, -1);

  //Kran und Konsole
  var cran = CreateObject(CRN1, 5430, 377, -1);
  cran->Set(19);
  CreateObject (CONS, 5240, 355, -1)->Set(cran);

  //Sounds

  //Wind
  CreateObject(SE4K, 1500, 200, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2000, 200, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 3000, 200, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 3000, 200, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 4500, 200, -1)->Set("WindSound*.ogg",775,250);

  //Erdrutsche
  CreateObject(SE4K, 400, 200, -1)->Set("FallingDirt*.ogg",775,250);
  CreateObject(SE4K, 1800, 200, -1)->Set("FallingDirt*.ogg",775,250);

  //Zikaden
  CreateObject(SE4K, 700, 300, -1)->Set("Cicada*",850,250);
  CreateObject(SE4K, 2600, 300, -1)->Set("Cicada*",850,250);

  //Hallen
  CreateObject(SE4K, 2260, 340, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2960, 310, -1)->Set("Interior*.ogg",670,105);

  //Wellen
  CreateObject(SE4K, 5000, 550, -1)->Set("Wave*.ogg",200,75);
  CreateObject(SE4K, 5500, 540, -1)->Set("Wave*.ogg",150,50);
  CreateObject(SE4K, 6000, 530, -1)->Set("Wave*.ogg",100,35);
}

func CreateEquipment()
{
  Log("$CreatingEquipment$");

  //Versorgungskisten (Kugeln)
  var crate = CreateObject(AMCT, 1170, 350, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 3020, 490, -1);
  crate->Set(ABOX);
  crate = CreateObject(AMCT, 5300, 360, -1);
  crate->Set(ABOX);

  //Versorgungskisten (Gewehrgranaten)
  crate = CreateObject (AMCT, 1310, 330, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 2970, 500, -1);
  crate->Set(GBOX);
  crate = CreateObject (AMCT, 5330, 360, -1);
  crate->Set(GBOX);

  //Raketen
  PlaceSpawnpoint(MBOX, 1900, 475);
  PlaceSpawnpoint(MBOX, 4355, 485);
}

func CreateDecoration()
{
  Log("$CreatingDecoration$");

  //Radio
  CreateObject(RDIO, 5520, 360, -1);

  //Leitkegel
  CreateObject(TCCN, 4450, 475, -1)->SetR(-20);
  CreateObject(TCCN, 4470, 470, -1)->SetR(-20);
  CreateObject(TCCN, 4490, 463, -1)->SetR(-20);

  //Bojen
  CreateObject(BUOY, 5390, 533, -1);
  CreateObject(BUOY, 5600, 533, -1);

  //Gel�nder
  CreateObject(RAI1, 4740, 360, -1)->SetRail([1,1]);
  CreateObject(BART, 2150, 480, -1)->SetRail([1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1]);
  CreateObject(RAI1, 5070, 460, -1)->SetRail([1,1,1,1,1,1,1,1,1,1,1,1]);
  CreateObject(RAI1, 5150, 360, -1);
  CreateObject(RAI1, 5280, 360, -1);

  //Flaschen
  CreateObject(BOTL, 2340, 400, -1);
  CreateObject(BOTL, 2350, 400, -1);

  //Monitore
  CreateObject(MNI2, 2190, 400, -1);
  CreateObject(MNI2, 4830, 442, -1);

  //Markierungsschilder
  CreateObject(MSGN, 4760, 360, -1);
  CreateObject(MSGN, 5110, 360, -1);

  //Z�une
  CreateObject(FENC, 2340, 480, -1);
  CreateObject(FENC, 3380, 430, -1);
  CreateObject(FENC, 3440, 430, -1);
  CreateObject(FENC, 3540, 390, -1);
  CreateObject(FENC, 3600, 390, -1);
  CreateObject(FENC, 3880, 420, -1);
  CreateObject(FENC, 3940, 420, -1);
  CreateObject(FENC, 4530, 460, -1)->Set(2);
  CreateObject(FENC, 4590, 460, -1)->Set(2);
  CreateObject(FENC, 4650, 460, -1);
  CreateObject(FENC, 4710, 460, -1);

  //Satellitensch�ssel
  CreateObject(SADH, 3440, 430, -1);

  //Notausgangslicht
  CreateObject(ETLT, 5040, 430, -1);

  //Wandlampen
  CreateObject(BLGH, 2230, 365, -1);
  CreateObject(BLGH, 2290, 445, -1);
  CreateObject(BLGH, 5225, 330, -1);

  //Deckenlichter
  CreateObject(CLGH,4840,310,-1);
  CreateObject(CLGH,4940,310,-1);
  CreateObject(CLGH,5025,310,-1);
}

func CreateOptionalFeatures()
{
  //Hintergrund
  CreateObject(BD01,500,700,-1);
}

/* Bei Relaunch */
 
public func OnClassSelection(object pClonk, int iTeam)
{
  //Assault-Spielziel
  if (FindObject(GASS))
  {
   if(GetPlayerTeam(GetOwner(pClonk)) == 1)
   {
    if(GetAssaultTarget(0,1) || GetAssaultTarget(1,1))
    {
     AddEffect("IntPara", pClonk, 1, 1);
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


/* Regelw�hler */

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
   aFlag[0] = CreateObject(OFPL,1610,470,NO_OWNER);
   aFlag[0] -> AddSpawnPoint(1085,300);
   aFlag[0] -> AddSpawnPoint(1310,320);
   aFlag[0] -> AddSpawnPoint(1200,270);
   if(aTeams[1] == true)
   {
    aFlag[0]->Set("$Flag1$",100,2);
    aFlag[0]->Capture(1,1);
   }
   else
   {
    aFlag[0]->Set("$Flag1$",0,2);
   }

   aFlag[1] = CreateObject(OFPL,2335,320,NO_OWNER);
   aFlag[1] -> AddSpawnPoint(2240,540);
   aFlag[1] -> AddSpawnPoint(2475,500);
   aFlag[1] -> AddSpawnPoint(2160,540);
   aFlag[1]->Set("$Flag2$",0,2);

   aFlag[2] = CreateObject(OFPL,3080,400,NO_OWNER);
   aFlag[2] -> AddSpawnPoint(3120,520);
   aFlag[2] -> AddSpawnPoint(3180,530);
   aFlag[2] -> AddSpawnPoint(3380,490);
   aFlag[2]->Set("$Flag3$",0,2);

   aFlag[3] = CreateObject(OFPL,3580,390,NO_OWNER);
   aFlag[3] -> AddSpawnPoint(3810,330);
   aFlag[3] -> AddSpawnPoint(3770,500);
   aFlag[3] -> AddSpawnPoint(3930,460);
   aFlag[3]->Set("$Flag4$",0,2);

   aFlag[4] = CreateObject(OFPL,4870,460,NO_OWNER);
   aFlag[4] -> AddSpawnPoint(5030,270);
   aFlag[4] -> AddSpawnPoint(5100,350);
   aFlag[4] -> AddSpawnPoint(5255,350);
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
   var crate = CreateObject(AMCT, 3380, 430, -1);
   crate->Set(PGRN);

   //Grenzen setzen
   CreateObject(BRDR, 700, 0, -1)->Set(0,1);
   CreateObject(BRDR, 5600, 0, -1)->Set(1,1);

   //Hinweisschilder
   CreateObject(SNPT, 430, 261, -1);
   CreateObject(SGNP, 610, 290, -1);

   //Gesch�tzstellungen
   CreateObject(GNET, 1040, 310, -1)->Set(0,0,1);
   CreateObject(GNET, 5225, 280, -1)->Set(0,0,1);

   //Blackhawks und Hinweisschilder
   if(!FindObject(NOBH))
   {
    SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,910,270,-1),60*21,300);
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4925,220,-1),60*21,300);

    var sign = CreateObject(SGNP, 840, 300, -1);
    sign->SetPhase(2);
    var sign = CreateObject(SGNP, 4990, 250, -1);
    sign->SetPhase(2);
    sign->SetMode(1);
   }
  }

  //Assault-Spielziel
  if (FindObject(GASS))
  {
   //Zielobjekte
   AddAssaultTarget(CMSN, 2025, 511, 150, 2, "$Target1$", 0, [[[2315, 550], [2470, 510], [2600, 440]], [[850, -20], [900, -20], [950, -20]]]);
   AddAssaultTarget(CMSN, 2255, 400, 150, 2, "$Target2$", 1, [[[2315, 550], [2470, 510], [2600, 440]], [[850, -20], [900, -20], [950, -20]]]);

   AddAssaultTarget(HUT3, 3390, 430, 150, 2, "$Target3$", 2, [[[3770, 510], [3810,340], [3930, 470]], [[2160, 550], [2190, 400], [2190, 480]]]);
   AddAssaultTarget(HUT2, 3580, 390, 150, 2, "$Target4$", 3, [[[3770, 510], [3810,340], [3930, 470]], [[2160, 550], [2190, 400], [2190, 480]]]);

   AddAssaultTarget(PMP2, 4895, 420, 150, 2, "$Target5$", 4, [[[5255, 360], [5300, 440], [5350, 360]], [[3240, 460], [3350, 510], [3450, 430]]]);
   AddAssaultTarget(PMP2, 4995, 420, 150, 2, "$Target5$", 5, [[[5255, 360], [5300, 440], [5350, 360]], [[3240, 460], [3350, 510], [3450, 430]]]);

   //Ziele verbinden
   ConnectAssaultTargets([0, 1]);
   ConnectAssaultTargets([2, 3]);
   ConnectAssaultTargets([4, 5]);

   //Versorgungskiste (Dragnin)
   crate = CreateObject (AMCT, 1435, 450, -1);
   crate->Set(DGNN);

   //Versorgungskiste (Sensorb�lle)
   crate = CreateObject (AMCT, 4380, 490, -1);
   crate->Set(SRBL);

   //Versorgungskiste (Sprengfallen)
   crate = CreateObject (AMCT, 5480, 460, -1);
   crate->Set(BBTP);

   //Grenzen setzen
   CreateObject(BRDR, 700, 0, -1)->Set(0,1);
   CreateObject(BRDR, 2700, 0, -1)->Set(1,1);

   //Hinweisschilder
   CreateObject(SNPT, 430, 261, -1);
   CreateObject(SGNP, 610, 290, -1);

   //Gesch�tzstellungen
   aStationary[0] = CreateObject(GNET, 1890, 480, -1);
   aStationary[0] -> Set(SATW,-90);
   aStationary[1] = CreateObject(GNET, 2330, 320, -1);
   aStationary[2] = CreateObject(GNET, 3340, 430, -1);
   aStationary[2] -> Set(0,-90);
   aStationary[3] = CreateObject(GNET, 4320, 490, -1);
   aStationary[3] -> Set(0,-90);

   //Blackhawks und Hinweisschilder
   if(!FindObject(NOBH))
   {
    SetupVehicleSpawn([BKHK],DIR_Right,CreateObject(VSPW,910,270,-1),60*21,300);
    SetupVehicleSpawn([BKHK],DIR_Left,CreateObject(VSPW,4925,220,-1),60*21,300);

    var sign = CreateObject(SGNP, 840, 300, -1);
    sign->SetPhase(2);
    var sign = CreateObject(SGNP, 4990, 250, -1);
    sign->SetPhase(2);
    sign->SetMode(1);
   }
  }

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   //Grenzen setzen
   CreateObject(BRDR, 1700, 0, -1)->Set(0);
   CreateObject(BRDR, 4210, 0, -1)->Set(1);

   //Hinweisschilder
   CreateObject(SGNP, 1510, 390, -1);
   CreateObject(SGNP, 1610, 470, -1);
   CreateObject(SGNP, 4140, 480, -1);
   CreateObject(SGNP, 4240, 480, -1);

   //Gesch�tzstellungen
   CreateObject(GNET, 2330, 320, -1)->Set(0,90,1);
   CreateObject(GNET, 3340, 430, -1)->Set(0,-90,1);
  }
}

/* Assault Zerst�rung */

public func OnAssaultTargetDestruction(object pTarget, int iTeam, int iIndex, bool fConnectedDestroyed)
{
  //Ziel 1 und 2
  if (!iIndex || iIndex == 1)
  {
   if(fConnectedDestroyed)
   {
    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 700, 0, -1)->Set(0,1);
    CreateObject(BRDR, 4100, 0, -1)->Set(1,1);

    //Spawnpoint entfernen
    RemoveObject(FindObject2(Find_ID(VSPW),Find_InRect(909, 269, 2, 2)));

    //Gesch�tzstellungen entfernen
    aStationary[0]->DecoExplode(30);
    aStationary[1]->DecoExplode(30);
   }
  }

  //Ziel 3 und 4
  if (iIndex == 2 || iIndex == 3)
  {
   if(fConnectedDestroyed)
   {
    //Ticketabzug anpassen
    SetTicketSubtractionTime(50);

    //Grenze neu setzen
    RemoveAll(BRDR);
    CreateObject(BRDR, 2160, 0, -1)->Set(0,1);
    CreateObject(BRDR, 5600, 0, -1)->Set(1,1);

    //Gesch�tzstellung entfernen
    aStationary[2]->DecoExplode(30);
   }
  }

  //Ziel 5 und 6
  if (iIndex == 4 || iIndex == 5)
  {
   if(fConnectedDestroyed)
   {
    //Gesch�tzstellung entfernen
    aStationary[3]->DecoExplode(30);
   }
  }
}

/* Relaunch */

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  //Assault-Spielziel
  if(FindObject(GASS))
  {if(FindObject(GASS)->GetRespawnPoint(iX, iY, iTeam)) return 1;}

  //LMS/DM-Spielziel
  if(FindObject(GLMS) || FindObject(GTDM))
  {
   if(iTeam == 1)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 2200; iY = 390; }
    if(!--rand)
     { iX = 2240; iY = 540; }
   }
   if(iTeam == 2)
   {
    var rand = Random(2);
    if(!rand)
     { iX = 3810; iY = 330; }
    if(!--rand)
     { iX = 3790; iY = 500; }
   }
   return(1);
  }

  //Startsicht
  iX = 2240; iY = 390;
}