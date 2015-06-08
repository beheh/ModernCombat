/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules()		{return [NOFD, NOFF, NOAM];}
public func SectionName()	{return "Lobby";}
public func HideInMenu()	{return true;}


/* Initialisierung */

public func Initialize()
{
  //Starttitel und Musikliste zusammenstellen
  SetPlayList("CMC_Base Groove.ogg;CMC_Firehawk.ogg;CMC_Friendly Unit.ogg;CMC_Getaway.ogg;CMC_Deep Universe.ogg;CMC_Eurocorps.ogg;CMC_Moving Squad.ogg;CMC_Offensive.ogg;CMC_Rock Go On.ogg;CMC_Showtime.ogg;CMC_Slow Motion.ogg;CMC_Striking Force.ogg;CMC_No Good.ogg;CMC_Obsession.ogg;CMC_Your Eyes.ogg");
  Music("CMC_Base Groove.ogg");

  //Script starten
  FakeScriptGo(true);

  return _inherited(...);
}

/* Plazierungslisten */

public func CreateInterior()
{
  //Wahlraum
  var exit = CreateObject(EXIT,1855,510,-1);
  //exit->AddSectionID(TAT1);

  //Aufzüge
  CreateObject(LFTP, 1935, 445, -1);
  CreateObject(LFTP, 2585, 285, -1);

  //Leitern
  CreateObject(LADR, 1855, 750, -1)->Set(19);
  CreateObject(LADR, 2525, 750, -1)->Set(39);

  //Große Bodenluken
  CreateObject(H24K, 1855, 598, -1);
  CreateObject(H24K, 2525, 438, -1);

  //Schutztüren
  CreateObject(GDDR, 1690, 590, -1);
  CreateObject(GDDR, 2100, 590, -1);
  CreateObject(GDDR, 2550, 430, -1);
  CreateObject(GDDR, 2750, 430, -1);

  //Kisten
  CreateObject(WCR2, 2020, 510, -1)->AutoRespawn();
  CreateObject(WCR2, 2090, 510, -1)->AutoRespawn();
  CreateObject(WCR2, 2670, 350, -1)->AutoRespawn();

  //Metallkisten
  CreateObject(MWCR, 1800, 510, -1);
  CreateObject(MWCR, 2690, 350, -1);

  //Verbandskisten
  CreateObject(BECR, 2090, 492, -1);
  CreateObject(BECR, 2730, 350, -1)->AutoRespawn();

  //Glasscheiben
  CreateObject(_WIN, 1788, 481, -1);
  CreateObject(_WIN, 1788, 510, -1);
  CreateObject(_WIN, 2102, 481, -1);
  CreateObject(_WIN, 2102, 510, -1);

  CreateObject(_WIN, 2548, 321, -1);
  CreateObject(_WIN, 2548, 350, -1);
  CreateObject(_WIN, 2752, 321, -1);
  CreateObject(_WIN, 2752, 350, -1);

  //Sandsackbarriere
  CreateObject(SBBA, 1300, 590, -1);

  //Stahlbrücken
  CreateObject(_HBR, 1355, 602, -1)->SwitchMode();
  CreateObject(_HBR, 1465, 602, -1)->SwitchMode();
  CreateObject(_HBR, 1575, 602, -1)->SwitchMode();
  CreateObject(_HBR, 2235, 442, -1)->SwitchMode();
  CreateObject(_HBR, 2345, 442, -1)->SwitchMode();

  //Steine
  CreateObject(STNE, 365, 590, -1);
  CreateObject(STNE, 830, 610, -1);
  CreateObject(STNE, 2375, 660, -1);
  CreateObject(STNE, 2990, 440, -1);
  CreateObject(STNE, 3200, 450, -1);

  //Explosivtanks
  CreateObject(XTNK, 2040, 430, -1)->AutoRespawn();
  CreateObject(XTNK, 2120, 430, -1)->AutoRespawn();

  //Phosphortank
  CreateObject(PTNK, 2445, 430, -1)->AutoRespawn();

  //Grenzen
  CreateObject(BRDR, 1060, 0, -1)->Set(0);
  CreateObject(BRDR, 3270, 0, -1)->Set(1);

  //Verbundene Räume
  var doorw = CreateObject(ROM2, 1990, 510, -1);
  CreateObject(ROM2, 2640, 350, -1)->Connect(doorw);

  var doorw = CreateObject(ROM2, 1990, 590, -1);
  CreateObject(ROM2, 2640, 430, -1)->Connect(doorw);

  //Sounds

  //Wind
  CreateObject(SE4K, 1430, 370, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 2200, 220, -1)->Set("WindSound*.ogg",775,250);
  CreateObject(SE4K, 3200, 160, -1)->Set("WindSound*.ogg",775,250);

  //Hallen
  CreateObject(SE4K, 1980, 480, -1)->Set("Interior*.ogg",665,105);
  CreateObject(SE4K, 2650, 320, -1)->Set("Interior*.ogg",670,105);

  //Lautsprecher
  CreateObject(SE4K, 2250, 330, -1)->Set("Announce*.ogg",3000,500);

/*
  AutoRespawn(1500,0,CreateObject(PBRL,705,53,-1));
  CreateObject(LBDR,96,90,-1);
  CreateObject(LADR,241,121,-1)->Set(4);
  CreateObject(HA4K,241,91,-1);
  CreateObject(LBDR,296,90,-1);
  CreateObject(SBBA,349,89,-1)->Right();
  CreateObject(RAI3,302,90,-1);
  CreateObject(RAI3,331,90,-1);
  AutoRespawn(1500,0,CreateObject(PBRL,731,61,-1));
  CreateObject(LLGH,66,119,-1);
  CreateObject(FAUD,180,172,-1);
  CreateObject(_HBR,285,142,-1);
  AutoRespawn(1500,0,CreateObject(TRGT,719,144,-1));
  CreateObject(LBDR,96,170,-1);
  AutoRespawn(1500,0,CreateObject(XTNK,193,169,-1));
  AutoRespawn(1500,0,CreateObject(PTNK,153,169,-1));
  CreateObject(LBDR,326,170,-1);
  CreateObject(RAI3,332,171,-1);
  CreateObject(RAI3,390,171,-1);
  CreateObject(RAI3,361,171,-1);
  CreateObject(SBBA,407,169,-1)->Right();
  AutoRespawn(1500,0,CreateObject(XBRL,626,185,-1));
  AutoRespawn(1500,0,CreateObject(XBRL,637,185,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,642,185,-1));
  CreateObject(LLGH,65,208,-1);
  CreateObject(LBDR,96,290,-1);
  CreateObject(LLGH,65,287,-1);
  CreateObject(CLVM,135,290,-1);
  CreateObject(PLT2,188,292,-1);
  CreateObject(TABL,169,288,-1);
  CreateObject(SCA2,185,286,-1);
  CreateObject(TABL,207,288,-1);
  CreateObject(CCP1,232,290,-1);
  CreateObject(ETLT,260,264,-1);
  CreateObject(EXIT,260,291,-1);
  AutoRespawn(1500,0,CreateObject(WCR2,549,293,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,724,267,-1));
  CreateObject(LBDR,96,350,-1);
  AutoRespawn(1500,0,CreateObject(WCR2,135,352,-1));
  AutoRespawn(1500,0,CreateObject(WCR2,152,352,-1));
  CreateObject(ROM2,173,349,-1)->Connect(CreateObject(ROM2,291,170,-1));
  CreateObject(ROM2,191,349,-1)->Connect(CreateObject(ROM2,271,90,-1));
  AutoRespawn(1500,0,CreateObject(PBRL,204,349,-1));
  AutoRespawn(1500,0,CreateObject(PBRL,213,349,-1));
  CreateObject(CCP1,240,350,-1);
  AutoRespawn(1500,0,CreateObject(PBRL,274,349,-1));
  AutoRespawn(1500,0,CreateObject(XTNK,270,349,-1));
  CreateObject(ETLT,335,329,-1);
  CreateObject(SLDR,361,350,-1);
  CreateObject(RAI3,368,352,-1);
  CreateObject(RAI3,397,352,-1);
  CreateObject(ATBY,493,330,-1);
  AutoRespawn(1500,0,CreateObject(TRGT,599,332,-1));
  AutoRespawn(1500,0,CreateObject(HBRL,631,351,-1));
  AutoRespawn(1500,0,CreateObject(HBRL,644,346,-1));
  AutoRespawn(1500,0,CreateObject(HBRL,658,344,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,690,345,-1));
*/
  
  return true;
}

public func CreateEquipment()
{
  //Zielscheiben
  AutoRespawn(1500,0,CreateObject(TRGT,2340,490,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,2370,610,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,2410,540,-1));

  AutoRespawn(1500,0,CreateObject(TRGT,3090,230,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,3100,300,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,3150,350,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,3200,410,-1));

  //Versorgungskiste (Sturmgewehr)
  var crate = CreateObject(AMCT, 2200, 590, -1);
  crate->Set(ASTR);

  //Versorgungskiste (Schrotflinte)
  crate = CreateObject(AMCT, 2890, 420, -1);
  crate->Set(PPGN);

  //Versorgungskisten (Dragnin)
  crate = CreateObject(AMCT, 1990, 430, -1);
  crate->Set(DGNN);
  crate = CreateObject(AMCT, 2640, 270, -1);
  crate->Set(DGNN);

/*
  PlaceSpawnpoint(PSTL,220,280);

  PlaceSpawnpoint(SMGN,120,160);
  PlaceSpawnpoint(SGST,150,160);
  PlaceSpawnpoint(PPGN,180,160);
  PlaceSpawnpoint(ASTR,210,160);

  PlaceSpawnpoint(MNGN,130,80);
  PlaceSpawnpoint(RTLR,180,80);
  PlaceSpawnpoint(MIAP,260,80);

  CreateObject(AMCT, 260, 130, -1)->Set(ABOX);
  CreateObject(AMCT, 300, 130, -1)->Set(GBOX);
*/
  
  return true;
}

func CreateDecoration()
{
  //Denkmal
  CreateObject(MUMT, 1735, 510, -1);

  //Radio
  CreateObject(RDIO, 1700, 510, -1);

  //Radare
  CreateObject(RADR, 1935, 330, -1);
  CreateObject(RADR, 2585, 170, -1);

  return true;
}

/* Spielerinitialisierung */

public func InitializePlayer(int iPlr)
{
  if(GetType(arRespawn[iPlr]) != C4V_Array)
    arRespawn[iPlr] = [];

  SetRespawnPosition(RandomX(1740, 1820), 585, iPlr);

  SetArrow(1865, 485, iPlr);

  return _inherited(iPlr, ...);
}

/* Script */

public func Script1()
{
  TutorialMessage("$TxtWelcome$");
  Sound("EI4K_NewMessage.ogg");
  Sound("RadioConfirm*.ogg");
  FakeScriptGo(0);
  
  return true;
}
