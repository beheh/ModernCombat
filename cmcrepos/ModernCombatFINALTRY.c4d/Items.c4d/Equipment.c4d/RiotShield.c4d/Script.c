/*-- Abwehrschild --*/

#strict 2

local pShield, pUser, iHits;
local iPrevDir, fAiming, iAimingAngle;

public func IsDrawable()	{return true;}
public func HandX()		{}
public func HandY()		{return -1200;}
public func HandR()		{return 45;}
public func HandSize()		{return 850;}

public func CanAim()		{return true;}
func IsEquipment()		{return true;}
public func NoArenaRemove()	{return true;}


/* Initalisierung */

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
	if(GetUser()) {
		//Anti-Umfall-Zwischenspeicher-Hack
		if(GetAction(GetUser()) == "Tumble") {
			ObjectSetAction(GetUser(), "Walk");
			SetDir(iPrevDir, GetUser());
			if(fAiming) {
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
  else {
    RemoveShield();  
  }
  return 1;
}

/* Steuerung */

public func ControlThrow(caller)
{
  if(pShield)
  {
   pShield->ExecShove();
   return 1;
  }
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
  if(pContainer != GetUser()) iHits = 0;

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
  //Kein Schild, kein entfernen
  if(!pShield) return;

  SetAction("Idle");
  Sound("RSHL_Hide.ogg");

  //Schild weg
  RemoveObject(pShield);
}

/* Sonstiges */

public func DoHit(int iHit)
{
  DoAchievementProgress(iHit, AC06, GetController());
  iHits += iHit;
  if(iHits >= 120)
  {
    iHits = 0;
    DoPlayerPoints(BonusPoints("Protection"), RWDS_BattlePoints, GetOwner(Contained()), Contained(), IC16);
  }
}

protected func Hit()
{
  Sound("BlockOff*.ogg");
  return 1;
}

/* Schildhandling */

public func Selection()
{
  //Schild erstellen
  CreateShield();
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
   {Contained()->SetAiming(60);}
}
