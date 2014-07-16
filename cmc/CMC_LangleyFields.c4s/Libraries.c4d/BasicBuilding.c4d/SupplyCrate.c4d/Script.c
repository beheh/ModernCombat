/*-- Nachschubkiste --*/

#strict 2

local no,damaged,target;

public func IsBulletTarget() {return true;}
public func ItemLimit() {return 10;}

/* Initialisierung */

protected func Initialize()
{
  //Anfangs geschlossen
  SetAction("Closed");

  //Standardfarbe: Grau
  SetColorDw(HSL(0, 0, 150, 180));
}

/* Icondarstellung */

public func SetIcon(id idIcon)
{
  if(idIcon)
  {
    SetGraphics(0, 0, idIcon, 1, GFXOV_MODE_IngamePicture);

    var w = 8 * 1000 / GetObjHeight(); 
    var h = 8 * 1000 / GetObjHeight();

    SetObjDrawTransform(w, 0, 4000, 0, h, 0, this, 1);
  }
  else
    SetGraphics(0, 0, 0, 1);
}

public func Set(object pTargetBuilding, bool fNoCollect)
{
  no = fNoCollect;
  target = pTargetBuilding;
  return true;
}

/* Öffnen und Schließen */

protected func Open()
{
  if (GetAction() == "Closed")
    return SetAction("Opening");
}

protected func Close()
{
  if (GetAction() == "Open")
    return SetAction("Closing");	    
}

/* Anfassen und Loslassen */

protected func Grabbed(object pClonk, bool fGrab)
{
  if(!CheckGrab())
    return;
}

protected func GrabLost(object pClonk)
{
  CheckGrab();
}

private func CheckGrab()
{
  //Öffnet sich bzw. offen, wenn (mindestens) ein Clonk es anfasst
  if(FindObject2(Find_OCF(OCF_CrewMember), Find_Action("Push"), Find_ActionTarget(this)))
  {
    Open();
    return true;
  }
  Close();
}

protected func Opening() 
{
  Sound("AMCT_Open.ogg", false, this); 
}

protected func Opened() 
{
  if (!CheckGrab())
    return;
}

protected func Closing() 
{
  Sound("AMCT_Close.ogg", false, this);
}

/* Aufnahme */

public func RejectCollect(id idObj)
{
  if(no)
    return true;
    
  if(GetAction() != "Open")
    return true;

  return ContentsCount() > 5;

  return false;
}

protected func ControlUp(object pClonk)
{
  if(RejectContents())
    return true;

  SetCommand(pClonk, "Get", this, 0, 0, 0, 1);
}

protected func ControlDigDouble(object pClonk)
{
  if(Contents())
  {
    if(target)
      return TransferContents(target, pClonk);
                //Kisten für die Rüstkammer
    /*if(Contents()->~IsGrenade() || Contents()->~IsWeapon2() || GetID(Contents()) == C4PA || GetID(Contents()) == RSHL || GetID(Contents()) == BBTP)
      if(FindObject2(Find_ID(B_AR),Find_Distance(300),Find_Or(Find_Owner(GetOwner()),Find_Allied(GetOwner()))))
        return TransferContents(FindObject2(Find_ID(B_AR),Find_Distance(300),Find_Or(Find_Owner(GetOwner()),Find_Allied(GetOwner()))),pClonk);
                 //Kisten für das Munitionslager
    if(Contents()->~IsAmmoPacket())
      if(FindObject2(Find_ID(B_AD),Find_Distance(300),Find_Or(Find_Owner(GetOwner()),Find_Allied(GetOwner()))))
        return TransferContents(FindObject2(Find_ID(B_AD),Find_Distance(300),Find_Or(Find_Owner(GetOwner()),Find_Allied(GetOwner()))),pClonk);
                 //Kisten für das Medi-Center
    if(GetID(Contents()) == DGNB || GetID(Contents()) == FAPK || GetID(Contents()) == DDGN || GetID(Contents()) == CDBT)
      if(FindObject2(Find_ID(B_MC),Find_Distance(300),Find_Or(Find_Owner(GetOwner()),Find_Allied(GetOwner()))))
        return TransferContents(FindObject2(Find_ID(B_MC),Find_Distance(300),Find_Or(Find_Owner(GetOwner()),Find_Allied(GetOwner()))),pClonk);
                 //Kisten ggf. für Dragnin-Dispenser
    if(GetID(Contents()) == DGNB)
      if(FindObject2(Find_ID(B_DD),Find_Distance(300),Find_Or(Find_Owner(GetOwner()),Find_Allied(GetOwner()))))
        return TransferContents(FindObject2(Find_ID(B_DD),Find_Distance(300),Find_Or(Find_Owner(GetOwner()),Find_Allied(GetOwner()))),pClonk);*/
  }
}

public func TransferContents(object pToObj, object pByClonk)
{
  Message("$ContentsMoved$",pByClonk,GetName(pToObj));
  Sound("AMCT_Open.ogg", false, pByClonk); 
  
  for(var i = ContentsCount(); i > 0; i--)
    Enter(pToObj,Contents());
}

protected func RejectContents()
{
  return GetAction() != "Open";
}

/* Kiste leer? Fanta her! */
protected func DoRemove()
{
  if(!ContentsCount())
  {
    CastParticles("MetalCrateSplinter", 4, 50, 0,0, 40,60,GetColorDw(),GetColorDw());
    if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 4, 40, 0,0, 50, 75, RGBa(255,255,255,0), RGBa(255,255,255,0));
    RemoveObject();
  }
}

/* Aufschlag */ 
  
protected func Hit()
{
  Sound("CrateImpact*.ogg");
  return 1;
}

/* Zerstörung */

func Damage(int iChange, int iPlr)
{
  if(!this)
    return;
  SetController(iPlr);
  
  if(GetDamage() < 100) return;
  
  if(Contents()->~IsAmmoPacket())
    FakeExplode(40,iPlr+1);
  
  CastParticles("MetalCrateSplinter", 4, 50, 0,0, 40,60,GetColorDw(),GetColorDw());
  if(GetEffectData(EFSM_ExplosionEffects) > 1) CastParticles("MetalSplinter", 4, 40, 0,0, 50, 75, RGBa(255,255,255,0), RGBa(255,255,255,0));
  RemoveObject();
}
