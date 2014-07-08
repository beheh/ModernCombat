/*-- Lobby --*/

#strict 2

#include TSYS

public func UsedRules() { return [NOFD, NOFF, NOAM]; } 
public func SectionName() { return "Lobby"; } 
public func HideInMenu() { return true; } 

public func Initialize()
{
	SetPlayList("CMC_Base Groove.ogg;CMC_Moving Squad.ogg");
  Music("CMC_Base Groove.ogg");
  FakeScriptGo(true);
  
  return _inherited(...);
}

public func InitializePlayer(int iPlr)
{
	if(GetType(arRespawn[iPlr]) != C4V_Array)
		arRespawn[iPlr] = [];
	
	SetRespawnPosition(RandomX(120, 180), 280, iPlr);
	
	SetArrow(265, 265, iPlr);
	
	return _inherited(iPlr, ...);
}

public func CreateInterior()
{
	AutoRespawn(1500,0,CreateObject(PBRL,705,53,-1));
  AutoRespawn(1500,0,CreateObject(TRGT,728,51,-1));
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
  
  return true;
}

public func CreateEquipment()
{
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
  
  return true;
}

public func Script1()
{
	TutorialMessage("$TxtWelcome$");
  Sound("EI4K_NewMessage.ogg");
  Sound("RadioConfirm*.ogg");
  FakeScriptGo(0);
  
  return true;
}

