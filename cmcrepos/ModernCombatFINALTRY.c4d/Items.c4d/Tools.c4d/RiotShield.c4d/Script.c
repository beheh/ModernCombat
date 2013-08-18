/*-- Einsatzschild --*/

#strict 2

local pShield, pUser;
local iPrevDir, fAiming, iAimingAngle;
local fTumbling, szLastAction, fLastShieldActive;

public func IsDrawable()	{return true;}	//Wird sichtbar getragen
public func HandY()		{return -1200;}
public func HandR()		{return 45;}
public func HandSize()		{return 850;}

public func CanAim()		{return true;}
public func IsEquipment()	{return true;}
public func NoArenaRemove()	{return true;}


/* Initialisierung */

protected func Initialize()
{
  pShield = 0;
  pUser = 0;
}

/* Besitzerkontrolle */

public func SetUser(object pNewUser)
{
  pUser = pNewUser;
  SetOwner(GetOwner(pUser));
  SetController(GetController(pUser));
}

public func GetUser()
{
  return pUser;
}

public func CheckChange()
{
  if(GetUser())
  {
    //Anti-Umfall-Zwischenspeicher-Hack
    if(!fTumbling)
    {
      if(GetAction(GetUser()) == "Tumble" && this == Contents(0, GetUser()))
      {
      	if(fLastShieldActive)
      		return;

        ObjectSetAction(GetUser(), szLastAction);
        if(szLastAction == "ScaleLadder")
          GetUser()->GrabLadder();
        SetDir(iPrevDir, GetUser());
        if(fAiming)
        {
          GetUser()->~StartSquatAiming();
          GetUser()->~SetAiming(iAimingAngle);
        }
      }
      iPrevDir = GetDir(GetUser());
      fAiming = false;
      if(GetUser()->~IsAiming())
        fAiming = true;
      iAimingAngle = Abs(GetUser()->~AimAngle());
      if(Contents(0,pUser) != this)
        RemoveShield();
    }
    else
      if(GetAction(GetUser()) != "Tumble")
      {
        fTumbling = false;
      }
    szLastAction = GetAction(GetUser());
    if(pShield)
    	fLastShieldActive = ActIdle(pShield);
    else
    	fLastShieldActive = false;
  }
  else
  {
    RemoveShield();
  }
  return 1;
}

/* Steuerung */

public func ControlThrow(caller)
{
  //Nicht bei Anwahlverzögerung
  if(GetEffect("IntSelection", this))
    return true;

  if(pShield && (WildcardMatch(GetAction(caller), "*Armed*") || caller->~IsAiming()))
  {
    pShield->ExecShove();
    return true;
  }
  else
    return !GetPlrDownDouble(GetController(caller));
}

public func ControlDigDouble(object pByObj)
{
  return Activate(pByObj);
}

public func Activate(object pByObj)
{
  if((pByObj->GetAction() != "WalkArmed") && !pByObj->~IsAiming())
    return true;

  //Schildgestell aufstellen
  var pTemp = CreateObject(SDSD,0,10,Contained()->GetOwner());
  pTemp->SetAim(Contained()->~AimAngle());
  pTemp->SetDir(Contained()->GetDir());
  pByObj->SetAction("Push",pTemp);

  Sound("RSHL_Deploy.ogg");

  return RemoveObject(this);
}

/* Ablegen */

public func Departure()
{
  SetUser();
  RemoveShield();
  pShield = 0;
  pUser = 0;
  SetObjDrawTransform();
}

/* Aufnehmen */

public func Entrance(object pContainer)
{
  //Träger gleich Besitzer
  SetUser(pContainer);

  //Position setzen
  SetR();
  SetRDir();

  //Schild erstellen wenn aktiv gewählt
  if(Contents(0, pUser) == this)
    if(GetOCF(pContainer) & OCF_CrewMember)
      CreateShield();
}

public func RejectEntrance(object pObj)
{
  if(GetOCF(pObj) & OCF_Living)
  {
    if(ContentsCount(GetID(),pObj))
      return true;
  }
  return false;
}

/* Schilderstellung und -entfernung */

private func CreateShield()
{
  if(!GetUser()) return;

  SetAction("Invis");

  if(pShield)
  {
    pShield->Set(GetUser(),this);
  }
  else
  {
    pShield = CreateObject(RSLH,0,0,GetOwner(GetUser()));
    pShield->Set(GetUser(),this);
    Sound("RSHL_Deploy.ogg");
  }
}

private func RemoveShield()
{
  //Anwahlverzögerung entfernen sofern vorhanden
  if(GetEffect("IntSelection", this))
    RemoveEffect("IntSelection", this);

  //Nichts unternehmen wenn kein Schild
  if(!pShield) return;

  SetAction("Idle");
  Sound("RSHL_Hide.ogg");

  //Schild entfernen
  RemoveObject(pShield);
}

/* Sonstiges */

protected func Hit()
{
  Sound("MetalHit*.ogg");
}

/* Schildhandling */

public func Selection()
{
  //Schild erstellen
  CreateShield();

  //Anwahlverzögerung
  AddEffect("IntSelection", this, 1, 20, this);

  if(GetAction(GetUser()) == "Tumble")
    fTumbling = true;
}

public func Deselection()
{
  //Schild entfernen
  RemoveShield();
}

public func Destruction()
{
  RemoveShield();
}

/* Liegen */

public func AimStart()
{
  if(Contained()->~IsCrawling() && Contained()->~AimAngle() > 60)
  {
    Contained()->SetAiming(60);
  }
}
