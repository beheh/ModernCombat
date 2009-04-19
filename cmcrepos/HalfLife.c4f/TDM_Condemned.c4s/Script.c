#strict
#include CSTD

static aPlrRelaunch;
func Initialize()
{
  SetPlayList("HalfLife.ogg");
  Music("HalfLife",true);
  aPlrRelaunch = [];
  CreateFurniture();
}

func CreateFurniture()
{
  //Türen, Bodenluken und Leitern
  CreateObject (SLDR, 387, 300, -1);
  CreateObject (STDR, 336, 190, -1);
  CreateObject (STDR, 76, 190, -1);
  CreateObject (H24K, 306, 219-20, -1);
  CreateObject (HA4K, 170, 173-20, -1);
  CreateObject (HA4K, 170, 213-20, -1);
  CreateObject (LADR, 991, 457, -1)->Set(12,LADR_Front);
  CreateObject (LADR, 306, 288, -1)->Set(12,LADR_Front);
  CreateObject (LADR, 171, 239, -1)->Set(11,LADR_Front);

  //Möbel und Kisten
  CreateObject (WCR2, 93, 152, -1);
  CreateObject (WCR2, 513, 312, -1);
  CreateObject (CHAR, 124, 249, -1);
  CreateObject (TABL, 212, 264, -1)->SetR(45);
  CreateObject (LCKR, 140, 258, -1)->SetR(90);
  CreateObject (LCKR, 104, 250, -1);
  CreateObject (LCKR, 89, 250, -1);
  CreateObject (DEB1, 598, 482, -1);
  CreateObject (DEB1, 247, 330, -1)->SetDir(DIR_Right);
  CreateObject (SCA2, 230, 183, -1)->SetCon(50);
  CreateObject (SADH, 273, 149, -1);
  CreateObject (FAUD, 167, 373, -1);
  CreateObject (BART, 682, 421, -1)->Set(3,RAIL_Supported);
  CreateObject (STLH, 455, 277, -1);
  var tmp = CreateObject (ABOX, 123, 190, -1);
  tmp->Set(PIAP,3,35*2,35*10);
  tmp->SetGraphics("Normal");
  
  //Schlamm und so...
  CreateObject (_WEB, 320, 178-10, -1)->SetDir(DIR_Right);
  CreateObject (_WEB, 91, 220-10, -1);
  CreateObject (_DCY, 642, 463, -1);
  CreateObject (_DCY, 600, 462, -1);
  CreateObject (_DCY, 783, 462, -1);
  CreateObject (_DCY, 822, 463, -1);
  CreateObject (_DCY, 865, 465, -1);
  CreateObject (_DCY, 944, 463, -1);
  CreateObject (_DCY, 905, 462, -1);
  CreateObject (_DCY, 1026, 461, -1);
  CreateObject (_DCY, 444, 372, -1);
  CreateObject (_DCY, 483, 372, -1);
  CreateObject (_DCY, 527, 372, -1);
  CreateObject (_DCY, 367, 371, -1);
  CreateObject (_DCY, 406, 372, -1);
  CreateObject (_DCY, 327, 374, -1);
  CreateObject (_DCY, 284, 372, -1);
  CreateObject (_DCY, 245, 371, -1);
  CreateObject (_DCY, 28, 371, -1);
  CreateObject (_DCY, 73, 372, -1);
  CreateObject (_SMP, 26, 407, -1);
  CreateObject (_SMP, 613, 497, -1);
  CreateObject (_SMP, 236, 406, -1);
  CreateObject (_SMP, 279, 406, -1);
  CreateObject (_SMP, 318, 406, -1);
  CreateObject (_SMP, 404, 407, -1);
  CreateObject (_SMP, 365, 407, -1);
  CreateObject (_SMP, 447, 407, -1);
  CreateObject (_SMP, 486, 407, -1);
  CreateObject (_SMP, 1011, 497, -1);
  CreateObject (_SMP, 972, 497, -1);
  CreateObject (_SMP, 890, 497, -1);
  CreateObject (_SMP, 929, 497, -1);
  CreateObject (_SMP, 843, 496, -1);
  CreateObject (_SMP, 804, 496, -1);
  CreateObject (_SMP, 916, 363, -1);
  
  //Nebel
  CreateObject (_FOG, 721, 449, -1)->SetCon(140);
  CreateObject (_FOG, 976, 498, -1)->SetCon(150);
  CreateObject (_FOG, 1012, 374, -1)->SetCon(120);
  CreateObject (_FOG, 799, 498, -1)->SetCon(150);
  CreateObject (_FOG, 520, 405, -1)->SetCon(130);
  CreateObject (_FOG, 369, 408, -1)->SetCon(140);
  CreateObject (_FOG, 222, 407, -1)->SetCon(140);
}



//Relaunch

public func RelaunchClonk(int iPlr, object pCursor)
{
  var pClonk, iTeam = GetPlayerTeam(iPlr);
  
  if(iTeam == 1)
  {
    //MetroCop oder Manhack
    if(Random(3))
      pClonk = CreateObject(MCOP, 10, 10, iPlr);
    else
      pClonk = CreateObject(MHCK, 10, 10, iPlr);
  }
  else
  {
    //Zombie, Headcrab oder Giftheadcrab
    if(Random(3))
      pClonk = CreateObject(ZOMB, 10, 10, iPlr);
    else
    {
      if(!Random(2))
        pClonk = CreateObject(HCRB, 10, 10, iPlr);
      else
        pClonk = CreateObject(PCRB, 10, 10, iPlr);
    }
  }
  
  if(pCursor && !FindObject(MCSL) && (GetID(pCursor) == GetID(pClonk)))
    GrabObjectInfo(pCursor, pClonk);
  else
    MakeCrewMember(pClonk, iPlr);

  DoEnergy(+100, pClonk);
  SetCursor(iPlr, pClonk);
  SetPlrView(iPlr, pClonk);
  
  if(GetID(pCursor) == CLNK)
    SilentKill4K(pCursor);

  return(pClonk);
}

public func OnClonkEquip(object pClonk)
{
  if(GetID(pClonk) == MCOP)
  {
    var wpn = CreateContents(H2PW, pClonk);
    wpn->DoAmmo(wpn->GetFMData(FM_AmmoID),wpn->GetFMData(FM_AmmoLoad));
  }
}

public func RelaunchPosition(& iX, & iY, int iTeam)
{
  if(iTeam == 1)
  {
    iX = RandomX(20,40);
    iY = 180;
  }
  else
  {
    iX = RandomX(1000,1020);
    iY = 350;
  }
}

public func RejectRelaunch(int iPlr, int iTeam)
{
  aPlrRelaunch[GetPlayerByIndex(iPlr)]++;
  if(iTeam == 1)
  {
    if(aPlrRelaunch[GetPlayerByIndex(iPlr)] > 3)
      return(true);
  }
  else
  {
    if(aPlrRelaunch[GetPlayerByIndex(iPlr)] > 5)
      return(true);
  }
  return(false);
}

public func RelaunchPlayer(int iPlr, object pCrew, object pKiller, int iTeam, bool bFirst)
{
  // Kein ordentlicher Spieler?
  if(GetOwner(pCrew) == NO_OWNER || iPlr == NO_OWNER || !GetPlayerName(iPlr))
    return();
    
  // Kein Team
  if(!iTeam) iTeam = GetPlayerTeam(iPlr);
  
  // Reject?
  if(!bFirst)
    if(RejectRelaunch(iPlr,iTeam))
      return();
  
  pCrew = RelaunchClonk(iPlr, pCrew);
    
  if(GetPlayerType(iPlr) == C4PT_Script)
    pCrew->~SetupBot4K();
    
  // Zufallsposition
  var iX, iY;
  RelaunchPosition(iX, iY, iTeam);
  
  //Ausrüsten
  if(!FindObject(IGIB))
    OnClonkEquip(pCrew);
  
  if(Contained(pCrew))
    SetPosition(iX, iY, Contained(pCrew));
  else
    SetPosition(iX, iY, pCrew);

  if(!FindObject(MCSL) && !FindObject(CHOS))
    OnClassSelection(pCrew);
}